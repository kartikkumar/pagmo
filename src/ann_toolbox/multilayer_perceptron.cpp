#include <cstdlib>
#include <cmath>
#include <vector>        
#include <exception>
#include "../exceptions.h"

#include "multilayer_perceptron.h"

using namespace ann_toolbox;

// Constructor
multilayer_perceptron::multilayer_perceptron(unsigned int input_nodes_, unsigned int hidden_nodes_,
	unsigned int output_nodes_, const std::vector<double> &w) : 
    	neural_network(input_nodes_, output_nodes_),
		m_hidden(hidden_nodes_)
{
	// the number of weights is equal to all the inputs (and a bias)
	// for every hidden node, plus the connections from every hidden
	// node to every output, i.e. it is fully connected
	unsigned int wghts  = (m_inputs + 1) * m_hidden + (m_hidden + 1) * m_outputs;
	m_weights = std::vector<double>(wghts, 0);
	
	if(! w.empty()) set_weights(w);
}

// Destructor
multilayer_perceptron::~multilayer_perceptron() {}

// Computing the outputs
const std::vector<double> multilayer_perceptron::compute_outputs(std::vector<double> &inputs) 
{
	// check for correct input size
	if(inputs.size() != m_inputs) {
		pagmo_throw(value_error, "incorrect size of input vector");
	}
	
	// generate values for the hidden nodes
	std::vector<double> hidden(m_hidden, 0);
	unsigned int i = 0, j, ji;
    for(  ; i < m_hidden; i++ ) {
        // Set the bias (the first weight to the i'th hidden node)
        hidden[i] = m_weights[i * (m_inputs + 1)];			    
        
        for( j = 0; j < m_inputs; j++ ) {
            // Compute the weight number
            ji = i * (m_inputs + 1) + (j + 1);
            // Add the weighted input
            hidden[i] += m_weights[ji] * inputs[j];			 
        }
        
        // Apply the transfer function (a sigmoid with output in [0,1])
        hidden[i] = 1.0/( 1 + exp( -hidden[i] ));	  
    }

    // Offset for the weights to the output nodes
    unsigned int offset = m_hidden * (m_inputs + 1);

	// generate values for the output nodes	
	std::vector<double> outputs(m_outputs, 0);
    for( i = 0; i < m_outputs; i++ ) {
        // add the bias (wheigted by the first wheigt to the i^th output node
        outputs[i] = m_weights[offset + i * (m_hidden + 1)];		 
        
        for( j = 0; j < m_hidden; j++) 
        {
            // compute the weight number
            ji = offset + i * (m_hidden + 1) + (j + 1);
            // add the weighted input
            outputs[i] += m_weights[ji] * hidden[j];			
        }        

        outputs[i] = 1.0/( 1 + exp( - outputs[i]) );	  

    }

    return outputs;
}