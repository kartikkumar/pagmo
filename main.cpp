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
 *   the Free Software Foundation; either version 3 of the License, or       *
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

#include <iostream>
#include <fstream>

#include "src/GOclasses/basic/archipelago.h"
#include "src/GOclasses/basic/island.h"
#include "src/GOclasses/problems/docking.h"

#include "src/GOclasses/algorithms/de.h"
#include "src/GOclasses/algorithms/sga.h"

#include "src/ann_toolbox/perceptron.h"
#include "src/ann_toolbox/multilayer_perceptron.h"
#include "src/ann_toolbox/elman_network.h"
#include "src/ann_toolbox/ctrnn.h"

using namespace std;
using namespace pagmo;

extern std::string max_log_string = "";
extern double max_log_fitness = 0.0;

int main(){
	double best_fitness = 0.0;
	
// input = full state (6), output = thrusters (2)

// Perceptron
//	ann_toolbox::neural_network *ann = new ann_toolbox::perceptron(6, 2);
// MultiLayerPerceptron
	//ann_toolbox::multilayer_perceptron ann = new 
	ann_toolbox::multilayer_perceptron ann(6, 5, 2);
// ElmanNetwork
//	ann_toolbox::neural_network *ann = new ann_toolbox::elman_network(6, 2, 2);
// CTRNN
//	ann_toolbox::neural_network *ann = new ann_toolbox::ctrnn(6, 2, 2);
	
	
/*		testing
	std::vector<double> chrom;
	for(int i=0;i < ann.get_number_of_weights();i++)
		chrom.push_back(1.0);
	ann.set_weights(chrom);
	cout << ann << endl;
	std::vector<double> in; in.push_back(1.0);
	std::vector<double> out = ann.compute_outputs(in);
	cout << "Output: " << out[0] << endl;
	
	return 0;*/
	
	
	/////////////////////////////////////////////////

	// Starting Conditions:  x,  vx,  y,   vy, theta, omega
	double start_cnd[] = { -2.0, 0.0, 0.0, 0.0, M_PI_2, 0.0 };

	//                                      needed_count, max_time, max_thrust
	problem::docking prob = problem::docking(&ann, 5, 25, 0.1);
	prob.set_start_condition(start_cnd, 6);
	
	//prob.set_log_genome(true);

	algorithm::sga algo( 20, 	// Generations
						0.95,	// CR
						0.15,	// Mutation	
						1);		// Elitism

					// 3 islands, 25 individuals!
	archipelago arch = archipelago(prob, algo, 1, 25);
//	island isl = island(prob, algo, 25);

	int i = 0;
	while(i++ < 700) { 	
		arch.evolve();
		arch.join();

		cout << "\rrun: " << i;
		cout.flush();
//	    cout << "--- Best in evolution #" << i << ": " << isl.best().get_fitness() << endl;
		if(arch.best().get_fitness() < best_fitness) {
			best_fitness = arch.best().get_fitness();		
    		cout << "\r=== Best increased @ #" << i << ": " << arch.best().get_fitness() << endl;
			ofstream myfile;
			myfile.open ("bestrun.dat");
			myfile << max_log_string << endl;
			myfile.close();		
		}
		
		// change the starting condition?
		prob.set_start_condition(start_cnd, 6);
		
	}	
	cout << "==================== Best Overall: " << best_fitness << "\t(i=" << i << ")" << endl;
			
	return 0;
}
