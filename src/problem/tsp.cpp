/*****************************************************************************
 *   Copyright (C) 2004-2013 The PaGMO development team,                     *
 *   Advanced Concepts Team (ACT), European Space Agency (ESA)               *
 *   http://apps.sourceforge.net/mediawiki/pagmo                             *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers  *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits     *
 *   act@esa.int                                                             *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

//#include <boost/graph/graph_concepts.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include "../exceptions.h"
#include "../types.h"
#include "tsp.h"
#include <boost/graph/graphviz.hpp>

namespace pagmo { namespace problem {

    tsp::tsp() : base_tsp(), m_weights(graph2matrix(get_graph()))
    {
    }

    tsp::tsp(const std::vector<std::vector<double> >& weights) : base_tsp(matrix2graph(weights)), m_weights(weights)
    {
    }

    tsp::tsp(const tsp_graph& graph) : base_tsp(graph), m_weights(graph2matrix(get_graph())) {
    }

    /// Clone method.
    base_ptr tsp::clone() const
    {
        return base_ptr(new tsp(*this));
    }

    const std::vector<std::vector<double> >& tsp::get_weights() const 
    {
        return m_weights;
    }
    
    tsp_graph tsp::matrix2graph(const std::vector<std::vector<double> >& matrix) {
        tsp_graph retval;
        tsp_edge_map_weight weights = boost::get(boost::edge_weight_t(), retval);
        tsp_vertex from, to;
        tsp_edge link;
        
        // add vertices first 
        /* Checking if a vertex exists with no vertices inserted causes segfault
         * so we have to iterate 1st to get total number of vertices
         * then iterate again to insert them ... bummer
         * Couldn't figure out how to do it all in 2 for loops
         */
        size_t no_vertices = matrix.size();
        for (size_t v = 0; v < no_vertices; ++v)
            boost::add_vertex(v, retval);
        
        // add edges and weights
        for (size_t i = 0; i < no_vertices; ++i) {

            /* uncomment this and it's segfault
             * don't do this check and the logic is wrong
             */ 
            from = boost::vertex(i, retval);
//                if (from == tsp_graph::null_vertex())
//                    from = boost::add_vertex(i, the_graph);
            
            for (size_t j = 0 ; j < no_vertices; ++j) {
                // we don't allow connections to self
                if(i == j) continue;
                
                to = boost::vertex(j, retval);
                // create destination vertex only if not existent
                // for some reason this works, but is not enough
//                    if (to == tsp_graph::null_vertex())
//                        to = boost::add_vertex(j, the_graph);
                
                // create an edge connecting those two vertices
                link = (boost::add_edge(from, to, retval)).first;
                // add weight property to the edge
                weights[link] = matrix.at(i).at(j);
            }
        }
        return retval;
    }
    
    std::vector<std::vector<double> > tsp::graph2matrix(const tsp_graph & graph) {
        size_t n_vertex = boost::num_vertices(graph);
        std::vector<double> row(n_vertex,0.0);
        std::vector<std::vector<double> > retval(n_vertex,row);
        tsp_vertex_map_const_index vtx_idx = boost::get(boost::vertex_index_t(), graph);
        tsp_edge_map_const_weight weights = boost::get(boost::edge_weight_t(), graph);
        tsp_edge_range_t e_it = boost::edges(graph);
        
        for (e_it = boost::edges(graph); e_it.first != e_it.second; ++e_it.first) {
            int i = vtx_idx[boost::source(*e_it.first, graph)];
            int j = vtx_idx[boost::target(*e_it.first, graph)];
            retval[i][i] = 0;
            retval[i][j] = weights[*e_it.first];
        }
        return retval;
    }  


    /// Computes the index 
    /** 
     * Returns the index in the decision vector corresponding to the concatenated 
     * rows of a square matrix, containing the edges between vertices.
     * @param[in] i row
     * @param[in] j column
     * @param[in] n total number of vertices
     */
    size_t tsp::compute_idx(size_t const i, size_t const j, size_t const n) {
        //int idx_row = i*(n-1)   + j         - (j>i? 1:0);
        //int idx_col = i         + j*(n-1)   - (j>i? 1:0);
        return i*(n-1) + j - (j>i? 1:0);
    };
    
    /// Implementation of the objective function
    /**
     * Computes the fitness vector associated to a decision vector.
     * The fitness is defined as Sum_ij(w_ij * x_ij) 
     * where w_ij are the weights defining the distances between the cities
     * The decision vector x_ij is a concatenated binary adjacency matrix, 
     * with the diagonal elements skipped since they're always zero because
     * in a route you can't go from one vertex to itself.
     * @param[out] f fitness vector
     * @param[in] x decision vector``
     */
    void tsp::objfun_impl(fitness_vector &f, decision_vector const& x) const {
        size_t n = get_n_vertices();        
        
        pagmo_assert(f.size() == 1);
        pagmo_assert(x.size() == get_dimension() && x.size() == n * (n - 1));
        
        f[0]= 0;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if(i==j) continue;
                f[0] += m_weights[i][j] * x[compute_idx(i, j, n)];
            }
        }       
    }

    /// Constraint computation.
    /**
     * The sum for rows and columns of the binary adjacency matrix
     * has to be 0 for all rows and columns to be valid
     * @param[out] c constraint_vector
     * @param[in] x decision_vector
     */
    void tsp::compute_constraints_impl(constraint_vector &c, decision_vector const& x) const {
        size_t n = get_n_vertices();
        size_t ceq = (n-1)*(n-2);
        
        for (size_t i = 0; i < n; i++) {
           for (size_t j = 0; j < n; j++) {
               if(i==j) continue; // ignoring main diagonal
               
               decision_vector::size_type rows = compute_idx(i, j, n);
               decision_vector::size_type cols = compute_idx(j, i, n);
               
               // equalities
               c[i] += x[rows];
               c[i+n] += x[cols];
               
               // inequalities ( ignoring first row & column)
               if(i != 0 && j != 0) 
                   c[ceq++] = (i+1) - (j+1) + n * x[rows] - n;
           }
        }
        
        // subtracting -1 from equalities, and 1 for equalities < 0
        for (size_t i = 0; i < c.size(); ++i)
            if (i < 2*n-1) c[i]-=1;
//            else // inequalities
//                if (c[i] < 0) c[i] = 0;
    }

    /// Extra human readable info for the problem.
    /**
     * Will return a std::string containing a list of vertices and edges
     */
    std::string tsp::human_readable_extra() const {
        tsp_graph const the_graph = get_graph();
        
        std::ostringstream oss;
        oss << "The Boost Graph (Adjacency List): \n";// << the_graph << std::endl;
//        boost::write_graphviz(oss, m_graph, boost::make_edge_attributes_writer( boost::get(boost::edge_weight_t(), m_graph) ) );
        
        tsp_vertex_map_const_index vtx_idx = boost::get(boost::vertex_index_t(), the_graph);
        tsp_edge_map_const_weight weights = boost::get(boost::edge_weight_t(), the_graph);

        oss << "Vertices = { ";
        
        tsp_vertex_range_t v_it;
        for (v_it = boost::vertices(the_graph); v_it.first != v_it.second; ++v_it.first)
                oss << vtx_idx[*v_it.first] <<  " ";
        oss << "}" << std::endl;
        
        oss << "Edges (Source, Target) = Weight : " << std::endl;
        
        tsp_edge_range_t e_it = boost::edges(the_graph);
        for (e_it = boost::edges(the_graph); e_it.first != e_it.second; ++e_it.first) {
            int i = vtx_idx[boost::source(*e_it.first, the_graph)];
            int j = vtx_idx[boost::target(*e_it.first, the_graph)];
            oss << "(" << i << ", " << j<< ") = " << weights[*e_it.first] << std::endl;
        }
        oss << std::endl;
        
        return oss.str();
    }

    std::string tsp::get_name() const 
    {
        return "Traveling Salesman Problem";
    }

}} //namespaces

//BOOST_CLASS_EXPORT_IMPLEMENT(pagmo::problem::tsp);