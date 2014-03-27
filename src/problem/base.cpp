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

// 30/01/10 Created by Francesco Biscani.

#include <algorithm>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/ref.hpp>
#include <cmath>
#include <climits>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#include "../exceptions.h"
#include "../population.h"
#include "../types.h"
#include "base.h"

namespace pagmo
{
namespace problem {

/// Constructor from global dimension, integer dimension, fitness dimension, global constraints dimension, inequality constraints dimension and constraints tolerance.
/**
 * n and nf must be positive, ni must be in the [0,n] range, nc and nic must be positive and nic must be in the [0,nc] range.
 * Lower and upper bounds are set to 0 and 1 respectively. Constraints tolerance must be positive.
 *
 * @param[in] n global dimension of the problem.
 * @param[in] ni dimension of the combinatorial part of the problem.
 * @param[in] nf dimension of the fitness vector of the problem.
 * @param[in] nc global number of constraints.
 * @param[in] nic number of inequality constraints.
 * @param[in] c_tol constraints tolerance. Fills the tolerance vector of size nc with c_tol.
 */
base::base(int n, int ni, int nf, int nc, int nic, const double &c_tol): //TODO should we use size_type directly?
	m_i_dimension(boost::numeric_cast<size_type>(ni)),m_f_dimension(boost::numeric_cast<f_size_type>(nf)),
	m_c_dimension(boost::numeric_cast<c_size_type>(nc)),m_ic_dimension(boost::numeric_cast<c_size_type>(nic)),
	m_c_tol(nc,c_tol),
	m_decision_vector_cache_f(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity)),
	m_fitness_vector_cache(boost::numeric_cast<fitness_vector_cache_type::size_type>(cache_capacity)),
	m_decision_vector_cache_c(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity)),
	m_constraint_vector_cache(boost::numeric_cast<constraint_vector_cache_type::size_type>(cache_capacity)),
	m_best_x(0),
	m_best_f(0),
	m_best_c(0),
	m_fevals(0),
	m_cevals(0)
{
	if (c_tol < 0) {
		pagmo_throw(value_error,"constraints tolerance must be non-negative");
	}
	if (n <= 0 || !nf || ni > n || nic > nc) {
		pagmo_throw(value_error,"invalid dimension(s)");
	}
	const size_type size = boost::numeric_cast<size_type>(n);
	m_lb.resize(size);
	m_ub.resize(size);
	std::fill(m_lb.begin(),m_lb.end(),0);
	std::fill(m_ub.begin(),m_ub.end(),1);
	// Resize properly temporary fitness and constraint storage.
	m_tmp_f1.resize(m_f_dimension);
	m_tmp_f2.resize(m_f_dimension);
	m_tmp_c1.resize(m_c_dimension);
	m_tmp_c2.resize(m_c_dimension);
	// Normalise bounds.
	normalise_bounds();
}

/// Constructor from global dimension, integer dimension, fitness dimension, global constraints dimension, inequality constraints dimension and constraints tolerance.
/**
 * n and nf must be positive, ni must be in the [0,n] range, nc and nic must be positive and nic must be in the [0,nc] range.
 * Lower and upper bounds are set to 0 and 1 respectively. Constraints tolerance for equalities constraints must be positive.
 *
 * @param[in] n global dimension of the problem.
 * @param[in] ni dimension of the combinatorial part of the problem.
 * @param[in] nf dimension of the fitness vector of the problem.
 * @param[in] nc global number of constraints.
 * @param[in] nic number of inequality constraints.
 * @param[in] c_tol constraints tolerance vector.
 */
base::base(int n, int ni, int nf, int nc, int nic, const std::vector<double> &c_tol): //TODO should we use size_type directly?
	m_i_dimension(boost::numeric_cast<size_type>(ni)),m_f_dimension(boost::numeric_cast<f_size_type>(nf)),
	m_c_dimension(boost::numeric_cast<c_size_type>(nc)),m_ic_dimension(boost::numeric_cast<c_size_type>(nic)),
	m_c_tol(c_tol),
	m_decision_vector_cache_f(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity)),
	m_fitness_vector_cache(boost::numeric_cast<fitness_vector_cache_type::size_type>(cache_capacity)),
	m_decision_vector_cache_c(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity)),
	m_constraint_vector_cache(boost::numeric_cast<constraint_vector_cache_type::size_type>(cache_capacity)),
	m_best_x(0),
	m_best_f(0),
	m_best_c(0),
	m_fevals(0),
	m_cevals(0)
{
	if (c_tol.size() != static_cast<constraint_vector::size_type>(nc) ) {
		pagmo_throw(value_error,"invalid constraints vector dimension");
	}
	for(unsigned int i=0; i<(m_c_dimension - m_ic_dimension);i++) {
		if (c_tol[i] < 0) {
			pagmo_throw(value_error,"constraints tolerance must be non-negative for equality constraints");
		}
	}
	if (n <= 0 || !nf || ni > n || nic > nc) {
		pagmo_throw(value_error,"invalid dimension(s)");
	}
	const size_type size = boost::numeric_cast<size_type>(n);
	m_lb.resize(size);
	m_ub.resize(size);
	std::fill(m_lb.begin(),m_lb.end(),0);
	std::fill(m_ub.begin(),m_ub.end(),1);
	// Resize properly temporary fitness and constraint storage.
	m_tmp_f1.resize(m_f_dimension);
	m_tmp_f2.resize(m_f_dimension);
	m_tmp_c1.resize(m_c_dimension);
	m_tmp_c2.resize(m_c_dimension);
	// Normalise bounds.
	normalise_bounds();
}

/// Constructor from values for lower and upper bounds, global dimension, integer dimension, fitness dimension, global constraints dimension, inequality constraints dimension and constraints tolerance.
/**
 * l_value must not be greater than u_value, n and nf must be positive, ni must be in the [0,n] range, nc and nic must be positive and nic must be in the [0,nc] range.
 * Lower and upper bounds are set to l_value and u_value respectively. Constraints tolerance must be positive.
 *
 * @param[in] l_value value for all lower bounds.
 * @param[in] u_value value for all upper bounds.
 * @param[in] n global dimension of the problem.
 * @param[in] ni dimension of the combinatorial part of the problem.
 * @param[in] nf dimension of the fitness vector of the problem.
 * @param[in] nc global number of constraints.
 * @param[in] nic number of inequality constraints.
 * @param[in] c_tol constraints tolerance. Fills the tolerance vector of size nc with c_tol.
 */
base::base(const double &l_value, const double &u_value, int n, int ni, int nf, int nc, int nic, const double &c_tol):
	m_i_dimension(boost::numeric_cast<size_type>(ni)),m_f_dimension(boost::numeric_cast<f_size_type>(nf)),
	m_c_dimension(boost::numeric_cast<c_size_type>(nc)),m_ic_dimension(boost::numeric_cast<c_size_type>(nic)),
	m_c_tol(nc,c_tol),
	m_decision_vector_cache_f(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity)),
	m_fitness_vector_cache(boost::numeric_cast<fitness_vector_cache_type::size_type>(cache_capacity)),
	m_decision_vector_cache_c(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity)),
	m_constraint_vector_cache(boost::numeric_cast<constraint_vector_cache_type::size_type>(cache_capacity)),
	m_best_x(0),
	m_best_f(0),
	m_best_c(0),
	m_fevals(0),
	m_cevals(0)
{
	if (c_tol < 0) {
		pagmo_throw(value_error,"constraints tolerance must be non-negative");
	}
	if (n <= 0 || !nf || ni > n || nic > nc) {
		pagmo_throw(value_error,"invalid dimension(s)");
	}
	if (l_value > u_value) {
		pagmo_throw(value_error,"value for lower bounds cannot be greater than value for upper bounds");
	}
	const size_type size = boost::numeric_cast<size_type>(n);
	m_lb.resize(size);
	m_ub.resize(size);
	std::fill(m_lb.begin(),m_lb.end(),l_value);
	std::fill(m_ub.begin(),m_ub.end(),u_value);
	// Resize properly temporary fitness and constraint storage.
	m_tmp_f1.resize(m_f_dimension);
	m_tmp_f2.resize(m_f_dimension);
	m_tmp_c1.resize(m_c_dimension);
	m_tmp_c2.resize(m_c_dimension);
	// Normalise bounds.
	normalise_bounds();
}

/// Constructor from upper/lower bounds, integer dimension, fitness dimension, global constraints dimension, inequality constraints dimension and constraints tolerance.
/**
 * Will fail if ni is negative or greater than lb.size(), if nf is not positive, if the sizes of the lower/upper bounds are zero or not identical, if
 * any lower bound is greater than the corresponding upper bound. nc and nic must be positive and nic must be in the [0,nc] range.
 * Constraints tolerance must be positive.
 *
 * @param[in] lb lower bounds for the problem.
 * @param[in] ub upper bounds for the problem.
 * @param[in] ni dimension of the combinatorial part of the problem.
 * @param[in] nf dimension of the fitness vector of the problem.
 * @param[in] nc global number of constraints.
 * @param[in] nic number of inequality constraints.
 * @param[in] c_tol constraints tolerance. Fills the tolerance vector of size nc with c_tol.
 */
base::base(const decision_vector &lb, const decision_vector &ub, int ni, int nf, int nc, int nic, const double &c_tol):
	m_i_dimension(boost::numeric_cast<size_type>(ni)),m_f_dimension(boost::numeric_cast<f_size_type>(nf)),
	m_c_dimension(boost::numeric_cast<c_size_type>(nc)),m_ic_dimension(boost::numeric_cast<c_size_type>(nic)),
	m_c_tol(nc,c_tol),
	m_decision_vector_cache_f(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity)),
	m_fitness_vector_cache(boost::numeric_cast<fitness_vector_cache_type::size_type>(cache_capacity)),
	m_decision_vector_cache_c(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity)),
	m_constraint_vector_cache(boost::numeric_cast<constraint_vector_cache_type::size_type>(cache_capacity)),
	m_best_x(0),
	m_best_f(0),
	m_best_c(0),
	m_fevals(0),
	m_cevals(0)
{
	if (c_tol < 0) {
		pagmo_throw(value_error,"constraints tolerance must be non-negative");
	}
	if (!nf || m_i_dimension > lb.size() || nic > nc) {
		pagmo_throw(value_error,"invalid dimension(s)");
	}
	construct_from_iterators(lb.begin(),lb.end(),ub.begin(),ub.end());
	// Resize properly temporary fitness and constraint storage.
	m_tmp_f1.resize(m_f_dimension);
	m_tmp_f2.resize(m_f_dimension);
	m_tmp_c1.resize(m_c_dimension);
	m_tmp_c2.resize(m_c_dimension);
	// Normalise bounds.
	normalise_bounds();
}

/// Trivial destructor.
/**
 * No side effects.
 */
base::~base() {}

/// Get problem's name.
/**
 * Default implementation will return the problem's mangled C++ name.
 *
 * @return name of the problem.
 */
std::string base::get_name() const
{
	return typeid(*this).name();
}

/// Lower bounds getter.
/**
 * @return const reference to the lower bounds vector.
 */
const decision_vector &base::get_lb() const
{
	return m_lb;
}

/// Upper bounds getter.
/**
 * @return const reference to the upper bounds vector.
 */
const decision_vector &base::get_ub() const
{
	return m_ub;
}

/// Bounds setter from pagmo::decision_vector.
/**
 * Set lower/upper bounds to lb/ub. Will fail if lb and ub sizes do not match, if their sizes are different
 * from the global size of the problem or if at least one lower bound is greater than the corresponding upper bound.
 *
 * @param[in] lb lower bounds.
 * @param[in] ub upper bounds.
 */
void base::set_bounds(const decision_vector &lb, const decision_vector &ub)
{
	if (lb.size() != ub.size() || lb.size() != m_lb.size()) {
		pagmo_throw(value_error,"invalid or inconsistent bounds dimensions in set_bounds()");
	}
	verify_bounds(lb.begin(),lb.end(),ub.begin(),ub.end());
	m_lb = lb;
	m_ub = ub;
	// Normalise bounds.
	normalise_bounds();
}

/// Set bounds to specified values.
/**
 * Set all lower bounds to l_value and all upper bounds to u_value. Will fail if l_value > u_value.
 *
 * @param[in] l_value value for all lower bounds.
 * @param[in] u_value value for all upper bounds.
 */
void base::set_bounds(const double &l_value, const double &u_value)
{
	if (l_value > u_value) {
		pagmo_throw(value_error,"lower bound cannot be greater than upper bound in set_bounds()");
	}
	std::fill(m_lb.begin(),m_lb.end(),l_value);
	std::fill(m_ub.begin(),m_ub.end(),u_value);
	normalise_bounds();
}

/// Set bounds to specified values.
/**
 * Set i-th lower bound to l_value and i-th upper bound to u_value. Will fail if l_value > u_value.
 *
 * @param[in] n index of the lower bound to be set.
 * @param[in] l_value value for lower bounds.
 * @param[in] u_value value for upper bounds.
 */
void base::set_bounds(int n, const double &l_value, const double &u_value)
{
	if (l_value > u_value) {
		pagmo_throw(value_error,"lower bound cannot be greater than upper bound in set_bounds()");
	}
	m_lb[n] = l_value;
	m_ub[n] = u_value;
	// Normalise bounds.
	normalise_bounds();
}

/// Set lower bounds from pagmo::decision_vector.
/**
 * Will fail if lb's size is different from the global size or if at least one lower bound is greater than the corresponding upper bound.
 *
 * @param[in] lb lower bounds.
 */
void base::set_lb(const decision_vector &lb)
{
	if (lb.size() != m_lb.size()) {
		pagmo_throw(value_error,"invalid bounds dimension in set_lb()");
	}
	verify_bounds(lb.begin(),lb.end(),m_ub.begin(),m_ub.end());
	m_lb = lb;
	// Normalise bounds.
	normalise_bounds();
}

/// Set specific lower bound to value.
/**
 * Will fail if n overflows global dimension or if value is greater than the corresponding upper bound.
 *
 * @param[in] n index of the lower bound to be set.
 * @param[in] value value the specified lower bound will be set to.
 */
void base::set_lb(int n, const double &value)
{
	const size_type i = boost::numeric_cast<size_type>(n);
	if (i >= m_lb.size() || m_ub[i] < value) {
		pagmo_throw(value_error,"invalid index and/or value for lower bound");
	}
	m_lb[i] = value;
	// Normalise bounds.
	normalise_bounds();
}

/// Set all lower bounds to value.
/**
 * Will fail if value is greater than at least one upper bound.
 *
 * @param[in] value value to which the lower bounds will be set.
 */
void base::set_lb(const double &value)
{
	for (size_type i = 0; i < m_lb.size(); ++i) {
		if (m_ub[i] < value) {
			pagmo_throw(value_error,"invalid value for lower bound");
		}
	}
	std::fill(m_lb.begin(),m_lb.end(),value);
	// Normalise bounds.
	normalise_bounds();
}

/// Set upper bounds from pagmo::decision_vector.
/**
 * Will fail if ub's size is different from the global size or if at least one upper bound is less than the corresponding lower bound.
 *
 * @param[in] ub upper bounds.
 */
void base::set_ub(const decision_vector &ub)
{
	if (ub.size() != m_lb.size()) {
		pagmo_throw(value_error,"invalid bounds dimension in set_ub()");
	}
	verify_bounds(m_lb.begin(),m_lb.end(),ub.begin(),ub.end());
	m_ub = ub;
	// Normalise bounds.
	normalise_bounds();
}

/// Set specific upper bound to value.
/**
 * Will fail if n overflows global dimension or if value is less than the corresponding lower bound.
 *
 * @param[in] n index of the upper bound to be set.
 * @param[in] value value the specified upper bound will be set to.
 */
void base::set_ub(int n, const double &value)
{
	const size_type i = boost::numeric_cast<size_type>(n);
	if (i >= m_lb.size() || m_lb[i] > value) {
		pagmo_throw(value_error,"invalid index and/or value for upper bound");
	}
	m_ub[i] = value;
	// Normalise bounds.
	normalise_bounds();
}

/// Set all upper bounds to value.
/**
 * Will fail if value is less than at least one lower bound.
 *
 * @param[in] value value to which the upper bounds will be set.
 */
void base::set_ub(const double &value)
{
	for (size_type i = 0; i < m_lb.size(); ++i) {
		if (m_lb[i] > value) {
			pagmo_throw(value_error,"invalid value for upper bound");
		}
	}
	std::fill(m_ub.begin(),m_ub.end(),value);
	// Normalise bounds.
	normalise_bounds();
}

/// Return number of function evaluations.
/**
 * @return number of function evaluations.
 */
unsigned int base::get_fevals() const
{
	return m_fevals;
}

/// Return number of constraints function evaluations.
/**
 * @return number of constraints function evaluations.
 */
unsigned int base::get_cevals() const
{
	return m_cevals;
}

/// Return number of function evaluations.
/**
 * @return number of function evaluations.
 */
void base::add_fevals(unsigned int fevals) const
{
	m_fevals += fevals;
}

/// Return number of constraints function evaluations.
/**
 * @return number of constraints function evaluations.
 */
void base::add_cevals(unsigned int cevals) const
{
	m_cevals += cevals;
}

/// Return global dimension.
/**
 * @return global dimension of the problem.
 */
base::size_type base::get_dimension() const
{
	return m_lb.size();
}

/// Return integer dimension.
/**
 * @return dimension of the combinatorial part of the problem.
 */
base::size_type base::get_i_dimension() const
{
	return m_i_dimension;
}

/// Return fitness dimension.
/**
 * @return fitness dimension of the problem.
 */
base::f_size_type base::get_f_dimension() const
{
	return m_f_dimension;
}

/// Return global constraints dimension.
/**
 * @return global constraints dimension of the problem.
 */
base::c_size_type base::get_c_dimension() const
{
	return m_c_dimension;
}

/// Return inequality constraints dimension.
/**
 * @return inequality constraints dimension of the problem.
 */
base::c_size_type base::get_ic_dimension() const
{
	return m_ic_dimension;
}

/// Return constraints tolerance.
/**
 * @return tolerance used in constraints analysis.
 */
const std::vector<double>& base::get_c_tol() const
{
	return m_c_tol;
}

/// Get the diameter of the problem.
/**
 * Calculate and return the space diagonal of the hyperrectangle defined by the bounds of the problem.
 *
 * @return the diameter of the problem.
 */
double base::get_diameter() const
{
	double retval = 0;
	for (size_type i = 0; i < get_dimension(); ++i) {
		retval += (m_ub[i] - m_lb[i]) * (m_ub[i] - m_lb[i]);
	}
	return std::sqrt(retval);
}

/// Return fitness of pagmo::decision_vector.
/**
 * Equivalent to:
@verbatim
fitness_vector f(get_f_dimension());
objfun(f,x);
return f;
@endverbatim
 *
 * @param[in] x decision vector whose fitness will be calculated.
 *
 * @return fitness vector of x.
 * @throws value_error if the chromosome dimension does not match with the problem dimension
 */
fitness_vector base::objfun(const decision_vector &x) const
{
	if (x.size()!=get_dimension()) {
		pagmo_throw(value_error,"invalid chromosome length");
	}
	fitness_vector f(m_f_dimension);
	objfun(f,x);
	return f;
}

/// Write fitness of pagmo::decision_vector into pagmo::fitness_vector.
/**
 * Will call objfun_impl() internally.
 * The implementation internally uses a caching mechanism, so that recently-computed quantities are remembered and re-used when appropriate.
 *
 * @param[out] f fitness vector to which x's fitness will be written.
 * @param[in] x decision vector whose fitness will be calculated.
 *
 * @throws value_error if f's and/or x's dimensions are different from the corresponding dimensions of the problem.
 */
void base::objfun(fitness_vector &f, const decision_vector &x) const
{
	// Some checks on the input values.
	if (f.size() != m_f_dimension) {
		pagmo_throw(value_error,"wrong fitness vector size when calling objective function");
	}
	if (x.size() != get_dimension()) {
		pagmo_throw(value_error,"wrong decision vector size when calling objective function");
	}
	// Look into the cache.
	typedef decision_vector_cache_type::iterator x_iterator;
	typedef fitness_vector_cache_type::iterator f_iterator;
	const x_iterator x_it = std::find(m_decision_vector_cache_f.begin(),m_decision_vector_cache_f.end(),x);
	if (x_it == m_decision_vector_cache_f.end()) {
		// Fitness is not into memory. Calculate it.
		objfun_impl(f,x);
		// Increase function evaluation counter.
		m_fevals++;
		// Make sure that the implementation of objfun_impl() in the derived class did not fuck up the dimension of the fitness vector.
		if (f.size() != m_f_dimension) {
			pagmo_throw(value_error,"fitness dimension was changed inside objfun_impl()");
		}
		// Store the decision vector and the newly-calculated fitness in the front of the buffers.
		m_decision_vector_cache_f.push_front(x);
		m_fitness_vector_cache.push_front(f);
	} else {
		// Compute the corresponding iterator in the fitness vector cache.
		f_iterator f_it = m_fitness_vector_cache.begin();
		std::advance(f_it,std::distance(m_decision_vector_cache_f.begin(),x_it));
		pagmo_assert(f_it != m_fitness_vector_cache.end());
		// Assign to the fitness vector the value in the cache.
		f = *f_it;
		// Move the content of the current positions to the front of the buffers, and shift everything else down
		// by one position.
		x_iterator tmp_x_it = m_decision_vector_cache_f.begin();
		f_iterator tmp_f_it = m_fitness_vector_cache.begin();
		while (x_it != tmp_x_it) {
			x_it->swap(*tmp_x_it);
			f_it->swap(*tmp_f_it);
			++tmp_x_it;
			++tmp_f_it;
		}
		pagmo_assert(tmp_f_it == f_it);
	}
}

/// Compare fitness vectors.
/**
 * Will perform sanity checks on v_f1 and v_f2 and then will call base::compare_fitness_impl().
 *
 * @param[in] v_f1 first fitness vector.
 * @param[in] v_f2 second fitness vector.
 *
 * @return compare_fitness_impl(v_f1,v_f2);
 */
bool base::compare_fitness(const fitness_vector &v_f1, const fitness_vector &v_f2) const
{
	if (v_f1.size() != m_f_dimension || v_f2.size() != m_f_dimension) {
		pagmo_throw(value_error,"invalid sizes for fitness vector(s) during comparison");
	}
	return compare_fitness_impl(v_f1,v_f2);
}

/// Implementation of fitness vectors comparison.
/**
 * Return true if v_f1 Pareto dominate v_f2, false otherwise. This default implementation will assume minimisation for each one of the v_f components
 * I.e., each pair of corresponding elements in v_f1 and v_f2 is compared: if all elements in v_f1 are less or equal to the corresponding
 * element in v_f2, true will be returned. Otherwise, false will be returned.
 *
 *
 * @param[in] v_f1 first fitness vector.
 * @param[in] v_f2 second fitness vector.
 *
 * @return true if v_f1 is dominating v_f2, false otherwise.
 */
bool base::compare_fitness_impl(const fitness_vector &v_f1, const fitness_vector &v_f2) const
{
	pagmo_assert(v_f1.size() == v_f2.size()); // NOTE: v_f1.size CAN be different from m_f_dimension (i.e. when its a base_meta calling it)
	f_size_type count1 = 0; f_size_type count2 = 0;
	for (f_size_type i = 0; i < v_f1.size(); ++i) {
		if (v_f1[i] < v_f2[i]) {
			++count1;
		}
		if (v_f1[i] == v_f2[i]) {
			++count2;
		}
	}
	return ( ( (count1+count2) == v_f1.size()) && (count1>0) );
}

/// Return human readable representation of the problem.
/**
 * Will return a formatted string containing:
 * - problem's name from get_name(),
 * - dimensions,
 * - lower and upper bounds.
 *
 * The output of human_readable_extra() will be appended at the end of the string.
 *
 * @return std::string containing a human-readable representation of the problem.
 */
std::string base::human_readable() const
{
	std::ostringstream s;
	s << "Problem name: " << get_name() << '\n';
	const size_type size = get_dimension();
	s << "\tGlobal dimension:\t\t\t" << size << '\n';
	s << "\tInteger dimension:\t\t\t" << m_i_dimension << '\n';
	s << "\tFitness dimension:\t\t\t" << m_f_dimension << '\n';
	s << "\tConstraints dimension:\t\t\t" << m_c_dimension << '\n';
	s << "\tInequality constraints dimension:\t" << m_ic_dimension << '\n';
	s << "\tLower bounds: ";
	s << m_lb << '\n';
	s << "\tUpper bounds: ";
	s << m_ub << '\n';
	s << "\tConstraints tolerance: ";
	s << m_c_tol << '\n';
	s << human_readable_extra();
	return s.str();
}

/// Extra information in human readable format.
/**
 * Default implementation returns an empty string.
 *
 * @return std::string containing additional problem-specific human-readable representation of the problem.
 */
std::string base::human_readable_extra() const
{
	return std::string();
}

/// Equality operator.
/**
 * The following conditions will be tested, in order:
 * - return value of is_compatible(),
 * - return value of equality_operator_extra().
 *
 * If any of the conditions above is false, then the return value will also be false. Otherwise return value will be true.
 *
 * @param[in] p problem::base to which this will be compared.
 *
 * @return true if this is equal to p, false otherwise.
 */
bool base::operator==(const base &p) const
{
	if (!is_compatible(p)) {
		return false;
	}
	return equality_operator_extra(p);
}

/// Compatibility operator.
/**
 * The concept of compatibility is used within the archipelago class: all islands must contain mutually-compatible problems. The rationale
 * behind this method is that migration between islands is allowed only if the problems are compatible. Compatibility differs from equality in the sense
 * that two problems might describe logically the same problem with different parameters. E.g., the optimisation of a neurocontroller driving a rover
 * in related but different environments (different gravity, different terrain, etc.). When migration occurs between islands whose problem are not equal
 * (i.e., identical), the incoming decision vector will be re-evaluated before being compared to the existing population.
 *
 * The following conditions will be tested, in order:
 * - problems are of the same type,
 * - problems have the same global, integer and constraints dimension.
 *
 * If any of the conditions above is false, then the return value will also be false. Otherwise return value will be true.
 *
 * @param[in] p problem::base to which this will be compared.
 *
 * @return true if this is compatible with p, false otherwise.
 */
bool base::is_compatible(const base &p) const
{
// marcusm: changed this for experiments with migration between single- and multiobjective islands.
/*	if (typeid(*this) != typeid(p) || get_dimension() != p.get_dimension() || m_i_dimension != p.m_i_dimension || m_f_dimension != p.m_f_dimension ||
		m_c_dimension != p.m_c_dimension || m_ic_dimension != p.m_ic_dimension)*/
	if (typeid(*this) != typeid(p) || get_dimension() != p.get_dimension() || m_i_dimension != p.m_i_dimension ||
		m_c_dimension != p.m_c_dimension || m_ic_dimension != p.m_ic_dimension)
	{
		return false;
	}
	return true;
}

/// Compare decision vectors.
/**
 * This functions returns true if x1 is a better decision_vector than x2, false otherwise.
 * This function will compute the fitness vectors and constraint vectors associated to x1
 * and x2 via objfun() and compute_constraints(), and will feed them to compare_fc(),
 * whose result will be returned.
 *
 * @param[in] x1 first pagmo::decision_vector.
 * @param[in] x2 second pagmo::decision_vector.
 *
 * @return true if x1 is better than x2, false otherwise.
 */
bool base::compare_x(const decision_vector &x1, const decision_vector &x2) const
{
	// Make sure the size of the tmp fitness vectors are suitable.
	pagmo_assert(m_tmp_f1.size() == m_f_dimension && m_tmp_f2.size() == m_f_dimension);
	// Store fitnesses into temporary space.
	objfun(m_tmp_f1,x1);
	objfun(m_tmp_f2,x2);
	// Make sure the size of the tmp constraint vectors are suitable.
	pagmo_assert(m_tmp_c1.size() == m_c_dimension && m_tmp_c2.size() == m_c_dimension);
	// Store constraint vector into temporary space.
	compute_constraints(m_tmp_c1,x1);
	compute_constraints(m_tmp_c2,x2);
	// Call the comparison implementation.
	return compare_fc(m_tmp_f1,m_tmp_c1,m_tmp_f2,m_tmp_c2);
}

/// Simultaneous fitness-constraint comparison.
/**
 * This function will perform sanity checks on the input arguments and will then call
 * compare_fc_impl() if the constraint dimensions is not null, compare_fitness_impl() otherwise.
 *
 * @param[in] f1 first pagmo::fitness_vector.
 * @param[in] c1 first pagmo::constraint_vector.
 * @param[in] f2 second pagmo::fitness_vector.
 * @param[in] c2 second pagmo::constraint_vector.
 *
 * @return result of compare_fc_impl() or compare_fitness_impl().
 */
bool base::compare_fc(const fitness_vector &f1, const constraint_vector &c1, const fitness_vector &f2, const constraint_vector &c2) const
{
	if (f1.size() != m_f_dimension || f2.size() != m_f_dimension) {
		pagmo_throw(value_error,"wrong size(s) for fitness vector(s)");
	}
	if (c1.size() != m_c_dimension || c2.size() != m_c_dimension) {
		pagmo_throw(value_error,"wrong size(s) for constraint vector(s)");
	}
	if (m_c_dimension) {
		return compare_fc_impl(f1,c1,f2,c2);
	} else {
		return compare_fitness_impl(f1,f2);
	}
}

/// Implementation of simultaneous fitness-constraint comparison.
/**
 * This function combines the information of two fitness/constraint vector pairs in order to establish which of the two pairs if strictly
 * better than the other. Return value will be true if the first pair is strictly better than the second pair, false otherwise.
 *
 * Default implementation will return true if one of these conditions, tested in this order, holds:
 * - c1 is feasible, c2 is not;
 * - both c1 and c2 are not feasible and compare_constraints_impl(c1,c2) returns true;
 * - both c1 and c2 are feasible and compare_fitness_impl(f1,f2) returns true.
 *
 * Otherwise, false will be returned.
 *
 * @param[in] f1 first pagmo::fitness_vector.
 * @param[in] c1 first pagmo::constraint_vector.
 * @param[in] f2 second pagmo::fitness_vector.
 * @param[in] c2 second pagmo::constraint_vector.
 *
 * @return true if first fitness/constraint vector pair is strictly better than the second one, false otherwise.
 */
bool base::compare_fc_impl(const fitness_vector &f1, const constraint_vector &c1, const fitness_vector &f2, const constraint_vector &c2) const
{
	const bool test1 = feasibility_c(c1), test2 = feasibility_c(c2);
	if (test1 && !test2) {
		return true;
	}
	if (!test1 && test2) {
		return false;
	}
	// At this point, either they are both feasible or they are not.
	if (test1) {
		pagmo_assert(test2);
		// If they are feasible, compare fitnesses and return.
		return compare_fitness_impl(f1,f2);
	} else {
		pagmo_assert(!test2);
		// If they are not feasible, compare constraints and return.
		return compare_constraints_impl(c1,c2);
	}
}

/** @name Constraints-related methods.
 * Methods used to calculate and compare constraints.
 */
//@{

/// Implementation of constraint computation.
/**
 * This functions is intended to write to c the constraint vector of input decision vector x.
 * The first get_c_dimension() - get_ic_dimension() elements of c will hold the equality constraints:
 * if a constraint is satisfied, the corresponding value in the vector will be zero. The remaining elements of the vector will hold the
 * inequality constraints: if a constraint is satisfied, the corresponding value in the vector will be non-positive.
 *
 * Default implementation will fill c with zeroes.
 *
 * @param[out] c pagmo::constraint_vector into which the constraints will be written.
 * @param[in] x pagmo::decision_vector whose constraints will be computed.
 */
void base::compute_constraints_impl(constraint_vector &c, const decision_vector &x) const
{
	(void)x;
	std::fill(c.begin(),c.end(),0);
}

/// Compute constraints and write them into contraint vector.
/**
 * This function will perform sanity checks on c and x and will then call compute_constraints_impl().
 *
 * The implementation internally uses a caching mechanism, so that recently-computed quantities are remembered and re-used when appropriate.
 *
 * @param[out] c pagmo::constraint_vector into which the constraints will be written.
 * @param[in] x pagmo::decision_vector whose constraints will be computed.
 */
void base::compute_constraints(constraint_vector &c, const decision_vector &x) const
{
	if (x.size() != get_dimension() || c.size() != get_c_dimension()) {
		pagmo_throw(value_error,"invalid constraint and/or decision vector(s) size(s) during constraint testing");
	}
	// Do not do anything if constraints size is 0.
	if (!m_c_dimension) {
		return;
	}
	// Look into the cache.
	typedef decision_vector_cache_type::iterator x_iterator;
	typedef constraint_vector_cache_type::iterator c_iterator;
	const x_iterator x_it = std::find(m_decision_vector_cache_c.begin(),m_decision_vector_cache_c.end(),x);
	if (x_it == m_decision_vector_cache_c.end()) {
		// Constraint vector is not into memory. Calculate it.
		compute_constraints_impl(c,x);
		m_cevals++;
		// Make sure c was not fucked up in the implementation of constraints calculation.
		if (c.size() != get_c_dimension()) {
			pagmo_throw(value_error,"constraints dimension was changed inside compute_constraints_impl()");
		}
		// Store the decision vector and the newly-calculated constraint vector in the front of the buffers.
		m_decision_vector_cache_c.push_front(x);
		m_constraint_vector_cache.push_front(c);
	} else {
		// Compute the corresponding iterator in the constraint vector cache.
		c_iterator c_it = m_constraint_vector_cache.begin();
		std::advance(c_it,std::distance(m_decision_vector_cache_c.begin(),x_it));
		pagmo_assert(c_it != m_constraint_vector_cache.end());
		// Assign to the fitness vector the value in the cache.
		c = *c_it;
		// Move the content of the current positions to the front of the buffers, and shift everything else down
		// by one position.
		x_iterator tmp_x_it = m_decision_vector_cache_c.begin();
		c_iterator tmp_c_it = m_constraint_vector_cache.begin();
		while (x_it != tmp_x_it) {
			x_it->swap(*tmp_x_it);
			c_it->swap(*tmp_c_it);
			++tmp_x_it;
			++tmp_c_it;
		}
		pagmo_assert(tmp_c_it == c_it);
	}
}

/// Compute constraints and return constraint vector.
/**
 * Equivalent to:
@verbatim
constraint_vector c(get_c_dimension());
compute_constraints(c,x);
return c;
@endverbatim
 *
 * @param[in] x pagmo::decision_vector whose constraints will be computed.
 *
 * @return x's constraint vector.
 */
constraint_vector base::compute_constraints(const decision_vector &x) const
{
	constraint_vector c(get_c_dimension());
	compute_constraints(c,x);
	return c;
}

/// Test feasibility of decision vector.
/**
 * This method will compute the constraint vector associated to x and test it with feasibility_c().
 *
 * @param[in] x pagmo::decision_vector whose feasibility will be tested.
 *
 * @return true if x satisfies the constraints, false otherwise.
 */
bool base::feasibility_x(const decision_vector &x) const
{
	// Compute the constraints and store internally.
	compute_constraints(m_tmp_c1,x);
	return feasibility_c(m_tmp_c1);
}

/// Test i-th constraint of c (using tolerance information).
/**
 * This method will return true if the i-th constraint is satisfied, false otherwise.
 *
 * @param[in] c pagmo::constraint_vector to be tested.
 * @param[in] i pagmo::constraint_vector index.
 *
 * @return true if the i-th constraint is satisfied (up to the numeric tolerance m_c_tol[i]), false otherwise.
 */
bool base::test_constraint(const constraint_vector &c, const c_size_type &i) const
{
	pagmo_assert(i < m_c_dimension);
	if (i < m_c_dimension - m_ic_dimension) {
		// Equality constraint testing.
		return (std::abs(c[i]) <= m_c_tol[i]);
	} else {
		return c[i] <= m_c_tol[i];
	}
}

/// Test feasibility of constraint vector.
/**
 * This method will return true if all constraints are satisfied, false otherwise.
 *
 * @param[in] c pagmo::constraint_vector to be tested.
 *
 * @return true if c satisfies the constraints, false otherwise.
 */
bool base::feasibility_c(const constraint_vector &c) const
{
	if (c.size() != m_c_dimension) {
		pagmo_throw(value_error,"invalid size for constraint vector");
	}
	pagmo_assert(m_c_dimension >= m_ic_dimension);
	for (c_size_type i = 0; i < m_c_dimension; ++i) {
		if (!test_constraint(c,i)) {
			return false;
		}
	}
	return true;
}

/// Compare constraint vectors.
/**
 * This function will perform safety checks on c1 and c2 and will then call compare_constraints_impl().
 *
 * @param[in] c1 first pagmo::constraint_vector to compare.
 * @param[in] c2 second pagmo::constraint_vector to compare.
 *
 * @return true if c1 is a better constraint vector than c2, false otherwise.
 */
bool base::compare_constraints(const constraint_vector &c1, const constraint_vector &c2) const
{
	if (c1.size() != m_c_dimension || c2.size() != m_c_dimension) {
		pagmo_throw(value_error,"invalid size(s) for constraint vector(s)");
	}
	return compare_constraints_impl(c1,c2);
}

/// Implementation of constraint vector comparison.
/**
 * Return true if c1 is a strictly better constraint vector than c2, false otherwise. Default implementation
 * will return true under the following conditions, tested in order:
 * - c1 satisfies more constraints than c2,
 * - c1 and c2 satisfy the same number of constraints and the L2 norm of the constraint mismatches for c1 is smaller than for c2.
 *
 * Otherwise, false will be returned.
 *
 * @param[in] c1 first pagmo::constraint_vector to compare.
 * @param[in] c2 second pagmo::constraint_vector to compare.
 *
 * @return true if c1 is a better constraint vector than c2, false otherwise.
 */
bool base::compare_constraints_impl(const constraint_vector &c1, const constraint_vector &c2) const
{
	pagmo_assert(c1.size() == c2.size() && c1.size() == m_c_dimension);
	// Counters of satisfied constraints.
	c_size_type count1 = 0, count2 = 0;
	// L2 norm of constraints mismatches.
	double norm1 = 0, norm2 = 0;
	// Equality constraints.
	for (c_size_type i = 0; i < m_c_dimension - m_ic_dimension; ++i) {
		if (test_constraint(c1,i)) {
			++count1;
		}
		if (test_constraint(c2,i)) {
			++count2;
		}
		norm1 += std::abs(c1[i]) * std::abs(c1[i]);
		norm2 += std::abs(c2[i]) * std::abs(c2[i]);
	}
	// Inequality constraints.
	for (c_size_type i = m_c_dimension - m_ic_dimension; i < m_c_dimension; ++i) {
		if (test_constraint(c1,i)) {
			++count1;
		} else {
			norm1 += c1[i] * c1[i];
		}
		if (test_constraint(c2,i)) {
			++count2;
		} else {
			norm2 += c2[i] * c2[i];
		}
	}
	if (count1 > count2) {
		return true;
	} else if (count1 < count2) {
		return false;
	} else {
		return (norm1 < norm2);
	}
}

//@}

/// Extra requirements for equality.
/**
 * Additional problem-specific equality testing. Default implementation returns true.
 *
 * @param[in] p problem::base to which this will be compared.
 *
 * @return true if p satisfies the additional equality testing, false otherwise.
 */
bool base::equality_operator_extra(const base &p) const
{
	(void)p;
	return true;
}

/// Inequality operator.
/**
 * Equivalent to the negation of equality operator.
 *
 * @param[in] p problem::base to which this will be compared.
 *
 * @return !(*this == p).
 */
bool base::operator!=(const base &p) const
{
	return !(*this == p);
}

/// Verify compatibility of decision vector x with problem.
/**
 * Will return false if any of these conditions, checked in this order, holds:
 * - x's dimension is different from get_dimension(),
 * - at least one value of x is outside the bounds of the problem,
 * - at least one element of the integer part of x is not an integer.
 *
 * Otherwise, true will be returned.
 *
 * @param[in] x pagmo::decision_vector to be tested.
 *
 * @return true if x is compatible with the problem, false otherwise.
 */
bool base::verify_x(const decision_vector &x) const
{
	if (x.size() != get_dimension()) {
		return false;
	}
	for (size_type i = 0; i < get_dimension(); ++i) {
		if (x[i] < m_lb[i] || x[i] > m_ub[i]) {
			return false;
		}
		// Check that, if this is an integer component, it is really an integer.
		if (i >= (get_dimension() - m_i_dimension) && double_to_int::convert(x[i]) != x[i]) {
			return false;
		}
	}
	return true;
}

// This function will round to the nearest integer the upper/lower bounds of the integer part of the problem.
// This should be called each time bounds are set.
void base::normalise_bounds()
{
	pagmo_assert(m_lb.size() >= m_i_dimension);
	// Flag to be set if we had to fix the bounds.
	bool bounds_fixed = false;
	for (size_type i = 0; i < m_lb.size() - m_i_dimension; ++i) {
		// Handle NaNs: if either lower/upper bound(s) are NaN, replace with 0 and 1 respectively.
		if (boost::math::isnan(m_lb[i]) || boost::math::isnan(m_ub[i])) {
			m_lb[i] = 0;
			m_ub[i] = 1;
			bounds_fixed = true;
		}
		// +-Infs are replaced by the highest/lowest values representable by double.
		if (boost::math::isinf(m_lb[i])) {
			m_lb[i] = (m_lb[i] > 0) ? boost::numeric::bounds<double>::highest() : boost::numeric::bounds<double>::lowest();
			bounds_fixed = true;
		}
		if (boost::math::isinf(m_ub[i])) {
			m_ub[i] = (m_ub[i] > 0) ? boost::numeric::bounds<double>::highest() : boost::numeric::bounds<double>::lowest();
			bounds_fixed = true;
		}
	}
	for (size_type i = m_lb.size() - m_i_dimension; i < m_lb.size(); ++i) {
		// First let's make sure that integer bounds are in the allowed range.
		if (m_lb[i] < INT_MIN) {
			m_lb[i] = INT_MIN;
			bounds_fixed = true;
		}
		if (m_lb[i] > INT_MAX) {
			m_lb[i] = INT_MAX;
			bounds_fixed = true;
		}
		if (m_ub[i] < INT_MIN) {
			m_ub[i] = INT_MIN;
			bounds_fixed = true;
		}
		if (m_ub[i] > INT_MAX) {
			m_ub[i] = INT_MAX;
			bounds_fixed = true;
		}
		// Then convert them to the nearest integer if necessary.
		if (m_lb[i] != double_to_int::convert(m_lb[i])) {
			m_lb[i] = double_to_int::convert(m_lb[i]);
			bounds_fixed = true;
		}
		if (m_ub[i] != double_to_int::convert(m_ub[i])) {
			m_ub[i] = double_to_int::convert(m_ub[i]);
			bounds_fixed = true;
		}
	}
	if (bounds_fixed) {
		pagmo_throw(value_error,"problem bounds were invalid and had to be fixed");
	}
}

/// Overload stream operator for problem::base.
/**
 * Equivalent to printing base::human_readable() to stream.
 *
 * @param[out] s std::ostream to which the problem will be streamed.
 * @param[in] p problem::base to be inserted into the stream.
 *
 * @return reference to s.
 */
std::ostream &operator<<(std::ostream &s, const base &p)
{
	s << p.human_readable();
	return s;
}

/// Sets the sparsity pattern of the gradient
/**
 * Some solvers can make use of the sparsity of the gradient matrix \f$ \mathbf G \f$. This is a matrix defined as
 * \f$ \mathbf G_{ij} = \frac{\partial F_i}{\partial x_j}\f$ where \f$ \mathbf F = [fit_1,\ldots,fit_{nfit}, ceq_1,\ldots,ceq_{neq}, cineq_1,\ldots, cineq_{nineq}] \f$
 * and \f$ \mathbf x \f$ is the decision vector. Such a pattern is there to define the non zero entries of
 * \f$ \mathbf G \f$ so that when evaluating numerical derivatives these are the only entries considered.
 *
 * This function might be used by specific solvers in order to speed-up
 * the computation of numerical derivatives. Please refer to the
 * solver-specific documentation in order to see if and how this method
 * is used.
 *
 * The reimplementation may call estimate_sparsity() to obtain a numerical estimate for
 * the sparsity pattern (CAUTION: this is not guaranteed to be always correct).
 *
 * The matrix \f$ \mathbf G \f$ needs to be represented as a sparse matrix so that if \f$ G_{ij} \neq 0 \f$
 * iGfun[l] = i, jGvar[l] = j.
 *
 */
void base::set_sparsity(int &lenG, std::vector<int> &iGfun, std::vector<int> &jGvar) const
{
	(void)lenG;
	(void)iGfun;
	(void)jGvar;
	pagmo_throw(not_implemented_error,"sparsity is not implemented for this problem");
}

/// Heuristics to estimate the sparsity pattern of the problem
/**
 * An alternative to reimplementing the base::set_pattern() method, one could let pagmo estimate
 * the sparsity structure of a given problem. The numerical procedure starts from a point \f$ \mathbf x_0 \f$
 * provided by the user and perturbs \f$ x_j \f$ locally as to detect a change in \f$ F_i \f$ in which case sets
 * (i,j) as a non zero element.
 * You should use this procedure with caution, it is always better to manually code the sparsity pattern
 * in set_pattern(). The procedure costs function evaluations and is not guaranteed to give
 * a correct result if not locally around the provided point. Some constraint may be independent of \f$ x_j \f$
 * near \f$ x_{0_j}\f$ but not globally, for such a discontinuous problem estimate_pattern would provide a false gradient information
 * The function intended use is in the reimplementation of set_sparsity, thuse its protected attribute
 */
void base::estimate_sparsity(const decision_vector &x0, int& lenG, std::vector<int>& iGfun, std::vector<int>& jGvar) const {
	// We check that the user is providing a decision vector that is of the required length
	if (!verify_x(x0)) {
		pagmo_throw(value_error,"cannot estimate pattern from this decision vector: not compatible with problem");
	}
	size_type Dc = m_lb.size() - m_i_dimension;
	fitness_vector f0(m_f_dimension),f_new(m_f_dimension);
	objfun(f0,x0);
	constraint_vector c0(m_c_dimension),c_new(m_c_dimension);
	compute_constraints(c0,x0);
	decision_vector x_new = x0;
	iGfun.resize(0);jGvar.resize(0); lenG=0;

	for (size_type j=0;j<Dc;++j)
	{
		//we perturb the component of x0 only if ub>lb, if ub=lb the variable is assumed
		//to be 'just' a parameter ... in some problem implementations this is rather
		//useful, but it also requires that the algorithm treat those variables accordingly (i.e.
		//it does not allow them to be outside the box bounds)
		if (m_ub[j] == m_lb[j]) continue;
		x_new[j] = x0[j] +  std::max(std::fabs(x0[j]), 1.0) * 1e-8;
		objfun(f_new,x_new);
		compute_constraints(c_new,x_new);
		for (size_type i=0;i<m_f_dimension;++i)
		{
			if (f_new[i]!=f0[i]) {iGfun.push_back(i); jGvar.push_back(j); lenG++;}
		}
		for (size_type i=0;i<m_c_dimension;++i)
		{
			if (c_new[i]!=c0[i]) {iGfun.push_back(i+m_f_dimension); jGvar.push_back(j); lenG++;}
		}
		x_new[j] = x0[j];
	}

}

/// Heuristics to estimate the sparsity pattern of the problem
/**
 * An alternative to reimplementing the base::set_pattern() method, one can let pagmo estimate
 * the sparsity structure of a given problem. This numerical procedure starts from a random point \f$ \mathbf x_0 \f$
 * provided by the user and perturbs \f$ x_j \f$ globally within the bounds as to detect a change in \f$ F_i \f$
 * in which case sets(i,j) as a non zero element.
 * You should use this procedure with caution, it is always better to manually code the sparsity pattern
 * in set_pattern(). The procedure costs function evaluations and is not guaranteed to give
 * a correct result. The function intended use is in the reimplementation of set_sparsity, thuse its protected attribute
 */
void base::estimate_sparsity(int& lenG, std::vector<int>& iGfun, std::vector<int>& jGvar) const {

	size_type Dc = m_lb.size() - m_i_dimension;
	fitness_vector f0(m_f_dimension),f_new(m_f_dimension);
	decision_vector x0(Dc);
	// Double precision random number generator.
	rng_double drng(rng_generator::get<rng_double>());

	for (decision_vector::size_type i = 0; i<Dc;++i) {
		x0[i] = boost::uniform_real<double>(m_lb[i],m_ub[i])(drng);
	}

	objfun(f0,x0);
	constraint_vector c0(m_c_dimension),c_new(m_c_dimension);
	compute_constraints(c0,x0);
	decision_vector x_new = x0;
	iGfun.resize(0);jGvar.resize(0); lenG=0;

	for (decision_vector::size_type j=0;j<Dc;++j)
	{
		//we perturb the component of x0 only if ub>lb, if ub=lb the variable is assumed
		//to be 'just' a parameter ... in some problem implementations this is rather
		//useful, but it also requires that the algorithm treat those variables accordingly (i.e.
		//it does not allow them to go outside the box bounds)
		if (m_ub[j] == m_lb[j]) continue;
		x_new[j] = boost::uniform_real<double>(m_lb[j],m_ub[j])(drng);
		objfun(f_new,x_new);
		compute_constraints(c_new,x_new);
		for (fitness_vector::size_type i=0;i<m_f_dimension;++i)
		{
			if (f_new[i]!=f0[i]) {iGfun.push_back(i); jGvar.push_back(j); lenG++;}
		}
		for (constraint_vector::size_type i=0;i<m_c_dimension;++i)
		{
			if (c_new[i]!=c0[i]) {iGfun.push_back(i+m_f_dimension); jGvar.push_back(j); lenG++;}
		}
		x_new[j] = x0[j];
	}
}

/// Sets the best known decision vectors
/**
 * This method is used to set the best know decision vectors members.
 *
 * @param[in] best_x best known pagmo::decision_vector for the problem
 */
void base::set_best_x(const std::vector<decision_vector>& best_x)
{
	if(best_x.size() != 0){
		size_type n_opts = best_x.size();
		m_best_x.resize(n_opts);
		m_best_f.resize(n_opts);
		m_best_c.resize(n_opts);
		for (size_type i=0; i<n_opts; i++){
			if(best_x.at(i).size() != get_dimension())
				pagmo_throw(value_error,"invalid size(s) for best known decision vector(s)");
			else{
				//save in the class data member the value of the decision variable of solution i
				m_best_x.at(i) = best_x.at(i);
				//save in the class data member the corresponding value of objective(s)
				m_best_f.at(i) = objfun(best_x.at(i));
				//save in the class data member the corresponding value of constraint(s)
				if(m_c_dimension>0)
					m_best_c.at(i) = compute_constraints(best_x.at(i));
			}
		}
	}
}

/// Get the best known constraint vector.
/**
 * @return the best known constraint vector for the problem.
 *
 */
const std::vector<constraint_vector>& base::get_best_c(void) const
{
	return m_best_c;
}

/// Get the best known decision vector.
/**
 * @return the best known decision vector for the problem.
 *
 */
const std::vector<decision_vector>& base::get_best_x(void) const
{
	return m_best_x;
}

/// Get the best known fitness vector.
/**
 * @return the best known fitness vector for the problem.
 *
 */
const std::vector<fitness_vector>& base::get_best_f(void) const
{
	return m_best_f;
}

/// Pre-evolution hook.
/**
 * This method will be called by the island objects before every optimisation run, and can be used to alter the island's population before evolution takes place.
 * Default implementation will do nothing.
 *
 * @param[in,out] pop pagmo::population belonging to the island calling this method.
 */
void base::pre_evolution(population &pop) const
{
	(void)pop;
}

/// Post-evolution hook.
/**
 * This method will be called by the island objects after every optimisation run, and can be used to alter the island's population after evolution took place.
 * Default implementation will do nothing.
 *
 * @param[in,out] pop pagmo::population belonging to the island calling this method.
 */
void base::post_evolution(population &pop) const
{
	(void)pop;
}

/// Reset internal caches.
/**
 * This method will reset the internal caches used when (re)evaluating decision vectors for fitnesses and/or constraints.
 * It should be called whenever a modification to the internal state of the problem makes the cached values invalid (e.g.,
 * changing the seed in a stochastic optimization problem might change the way decision vectors are evaluated, thus
 * rendering invalid previously-calculated values).
 */
void base::reset_caches() const
{
	m_decision_vector_cache_f = decision_vector_cache_type(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity));
	m_fitness_vector_cache = fitness_vector_cache_type(boost::numeric_cast<fitness_vector_cache_type::size_type>(cache_capacity));
	m_decision_vector_cache_c = decision_vector_cache_type(boost::numeric_cast<decision_vector_cache_type::size_type>(cache_capacity));
	m_constraint_vector_cache = constraint_vector_cache_type(boost::numeric_cast<constraint_vector_cache_type::size_type>(cache_capacity));
}

}} //namespaces
