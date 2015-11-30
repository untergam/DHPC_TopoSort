#include <iostream>
#include <omp.h>
#include <string>

#include "graph.hpp"
#include "analysis.hpp"

int main() {
    // Standard values
    unsigned N = 5000;
    double edgeFillDegree = 0.005;
    double p = 0.5;
    double q = 0.7;
	std::string visualbarrier(70,'=');
	visualbarrier = "\n\n\n" + visualbarrier + "\n\n";

	// PAPER GRAPH - Testing
	std::cout << visualbarrier;
	Graph testgraph_paper(9);
	testgraph_paper.connect(Graph::PAPER); // Constructing graph from paper
	testgraph_paper.time_topSort();
	testgraph_paper.checkCorrect(true);
	testgraph_paper.viz("paper");


	// RANDOM GRAPH - SMALL
	std::cout << visualbarrier;
	Graph testgraph_random_small(40);
	testgraph_random_small.connect(Graph::RANDOM_EDGES, edgeFillDegree);
	testgraph_random_small.time_topSort();
	testgraph_random_small.checkCorrect(false);
	
	// RANDOM GRAPH - MEDIUM
	std::cout << visualbarrier;
	unsigned const N = 5000;
	Graph testgraph_random(N);
	testgraph_random.connect(Graph::RANDOM_EDGES, edgeFillDegree);
	testgraph_random.time_topSort();
	if(testgraph_random.checkCorrect(false)){
        testgraph_random.dumpXmlAnalysis("../measurements/data/");
    }
    
    // SOFTWARE GRAPH - MEDIUM
	std::cout << visualbarrier;
	Graph softwaregraph(N);
	softwaregraph.connect(Graph::SOFTWARE, 0., p, q);
	softwaregraph.time_topSort();
	if(softwaregraph.checkCorrect(false)){
        softwaregraph.dumpXmlAnalysis("../measurements/data/");
    }

	std::cout << visualbarrier;
    
	return 0;

}
