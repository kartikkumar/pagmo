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

// 27/12/08 Created by Francesco Biscani.

#ifndef PAGMO_RNG_H
#define PAGMO_RNG_H

#include <boost/cstdint.hpp>
#include <boost/random/lagged_fibonacci.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

namespace pagmo
{
/// This rng returns an unsigned integer in the [0,2**32-1] range.
/**
 * @see http://www.boost.org/doc/libs/release/libs/random/random-generators.html
 */
typedef boost::mt19937 rng_uint32;

/// This rng returns a double in the [0,1[ range.
/**
 * @see http://www.boost.org/doc/libs/release/libs/random/random-generators.html
 */
typedef boost::lagged_fibonacci607 rng_double;

/// Generic thread-safe generator of pseudo-random number generators.
/**
 * To use, call the static member get() to get a pseudo-random number generator seeded with an initial pseudo-random value.
 *
 * Implementation internally uses a mutex, so that this generator can
 * be safely called concurrently from multiple threads. The initial seed used
 * is the number of microseconds elapsed since 01/01/1970, cast to uint32_t.
 *
 * @see http://www.boost.org/doc/libs/release/libs/random/index.html
 *
 * @author Francesco Biscani (bluescarni@gmail.com)
 */
class rng_generator {
	public:
		/// Return pseudo-random number generator.
		/**
		 * Type Rng must be a Boost-like pseudo-random number generator initialisable
		 * with a boost::uint32_t. Return value is seeded with an internal
		 * static pagmo::rng_uint32.
		 *
		 * @return pseudo-random number generator seeded with pseudo-random value.
		 */
		template <class Rng>
		static Rng get()
		{
			boost::lock_guard<boost::mutex> lock(m_mutex);
			return Rng(m_seeder());
		}
		static void set_seed(int);
	private:
		static boost::mutex	m_mutex;
		static rng_uint32	m_seeder;
};

}

#endif