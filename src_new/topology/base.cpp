/*****************************************************************************
 *   Copyright (C) 2004-2009 The PaGMO development team,                     *
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

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>

#include "../exceptions.h"
#include "base.h"

namespace pagmo { namespace topology {

/// Default constructor.
/**
 * Will build an empty topology.
 */
base::base():m_graph() {};

/// Copy constructor.
/**
 * Will deep-copy all members.
 *
 * @param[in] t topology::base to be copied.
 */
base::base(const base &t):m_graph(t.m_graph) {}

/// Assignment operator.
/**
 * Will perform a deep copy of all members.
 *
 * @param[in] t topology::base which will be copied into this.
 *
 * @return reference to this.
 */
base &base::operator=(const base &t)
{
	if (this != &t) {
		m_graph = t.m_graph;
	}
	return *this;
}

/// Trivial destructor.
/**
 * No side effects.
 */
base::~base() {}

/** @name Input/output. */
//@{

/// Return terse human readable representation.
/**
 * Will return a formatted string containing:
 * - the name of the topology, in mangled C++ form,
 * - the number of vertices.
 *
 * @return string containing terse human readable representation of the topology.
 */
std::string base::human_readable_terse() const
{
	std::ostringstream s;
	s << "Topology type:\t" << typeid(*this).name() << '\n';
	s << "Number of vertices:\t" << boost::num_vertices(m_graph) << '\n';
	s << "Number of edges:\t" << boost::num_edges(m_graph) << '\n';
	return s.str();
}

/// Return complete human readable representation.
/**
 * Will return a formatted string containing:
 * - the output of human_readable_terse(),
 * - the output of human_readable_extra(),
 * - the list of vertices and edges in a DOT-like syntax.
 *
 * @return string containing complete human readable representation of the topology.
 */
std::string base::human_readable() const
{
	std::ostringstream s;
	s << human_readable_terse();
	s << human_readable_extra() << '\n';
	s << "Connections:\n\n";
	std::pair<v_iterator,v_iterator> vertices = boost::vertices(m_graph);
	std::pair<a_iterator,a_iterator> adj_vertices;
	for (; vertices.first != vertices.second; ++vertices.first) {
		adj_vertices = boost::adjacent_vertices(*vertices.first,m_graph);
		s << m_graph[*vertices.first].m_idx;
		if (adj_vertices.first != adj_vertices.second) {
			s << " -> {";
			while (adj_vertices.first != adj_vertices.second) {
				s << m_graph[*adj_vertices.first].m_idx;
				++adj_vertices.first;
				if (adj_vertices.first != adj_vertices.second) {
					s << ',';
				}
			}
			s << "}\n";
		}
	}
	return s.str();
}

/// Return extra information for human readable representation.
/**
 * Return extra topology-dependent information that will be displayed when calling human_readable(). Default
 * implementation will return an empty string.
 *
 * @return string containing extra information for human readable representation.
 */
std::string base::human_readable_extra() const
{
	return std::string();
}

//@}

/// Get vertex iterator from island index.
/**
 * Will throw a value_error exception if island index is not in the graph or if n is negative.
 *
 * @param[in] n island index.
 *
 * @return iterator to the graph vertex.
 */
base::v_iterator base::get_it(int n) const
{
	const std::pair<v_iterator,v_iterator> vs = boost::vertices(m_graph);
	const v_iterator retval = std::find_if(vs.first,vs.second,idx_finder(m_graph,boost::numeric_cast<idx_type>(n)));
	if (retval == vs.second) {
		pagmo_throw(value_error,"vertex is not in the graph");
	}
	return retval;
}

/// Check if island index n is already present in the topology.
/**
 * Will fail if n is negative.
 *
 * @return true if island with index n is present in the topology, false otherwise.
 */
bool base::contains_vertex(int n) const
{
	try {
		(void)get_it(n);
	} catch (const value_error &) {
		return false;
	}
	return true;
}

/// Add a vertex.
/**
 * Add vertex containing island positional index n. Will fail if n is negative or
 * if n is already present inside the topology.
 *
 * @param[in] n island positional index to be inserted into the topology.
 */
void base::add_vertex(int n)
{
	if (contains_vertex(n)) {
		pagmo_throw(value_error,"cannot add vertex, already present in topology");
	}
	boost::add_vertex(island_property(boost::numeric_cast<idx_type>(n)),m_graph);
}

/// Return true if two vertices are adjacent.
/**
 * The direction must be from *it1 to *it2.
 *
 * @param[in] it1 iterator to first vertex.
 * @param[in] it2 iterator to second vertex.
 *
 * @return true if it exists an edge *it1 -> *it2, false otherwise.
 */
bool base::are_adjacent(const v_iterator &it1, const v_iterator &it2) const
{
	// Find it1's adjacent vertices.
	const std::pair<a_iterator,a_iterator> a_vertices = boost::adjacent_vertices(*it1,m_graph);
	if (std::find(a_vertices.first,a_vertices.second,*it2) == a_vertices.second) {
		return false;
	} else {
		return true;
	}
}

/// Add an edge.
/**
 * Add an edge connecting *it1 to *it2. Will fail if are_adjacent() returns true.
 *
 * @param[in] it1 iterator to first vertex.
 * @param[in] it2 iterator to second vertex.
 */
void base::add_edge(const v_iterator &it1, const v_iterator &it2)
{
	if (are_adjacent(it1,it2)) {
		pagmo_throw(value_error,"cannot add edge, vertices are already connected");
	}
	const bool result = boost::add_edge(*it1,*it2,m_graph).second;
	(void)result;
	pagmo_assert(result);
}

/// Remove an edge.
/**
 * Remove the edge connecting *it1 to *it2. Will fail if are_adjacent() returns false.
 *
 * @param[in] it1 iterator to first vertex.
 * @param[in] it2 iterator to second vertex.
 */
void base::remove_edge(const v_iterator &it1, const v_iterator &it2)
{
	if (!are_adjacent(it1,it2)) {
		pagmo_throw(value_error,"cannot remove edge, vertices are not connected");
	}
	boost::remove_edge(*it1,*it2,m_graph);
}

/// Get number of vertices.
/**
 * @return total number of vertices in the graph.
 */
base::vertices_size_type base::get_number_of_vertices() const
{
	return boost::num_vertices(m_graph);
}

/// Push back island index.
/**
 * This method will add index n into the graph and will then call connect() to establish the connections between the newly-added node
 * and the existing nodes in the graph.
 *
 * @param[in] n index of the island to be inserted into the graph.
 */
void base::push_back(int n)
{
	add_vertex(n);
	connect(n);
}

/// Overload stream insertion operator for topology::base.
/**
 * Will print to stream the output of topology::base::human_readable().
 *
 * @param[in] s stream into which topology will be inserted.
 * @param[in] t topology to be inserted into stream.
 *
 * @return reference to s.
 */
std::ostream &operator<<(std::ostream &s, const base &t)
{
	s << t.human_readable();
	return s;
}

}}