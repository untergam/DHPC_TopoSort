#include <iostream>
#include <omp.h>

#include "graph.hpp"
#include "rdtsc_timer.hpp"

#include "graphsort_omp_tasks.hpp"
#include "graphsort_omp_basic.hpp"

typedef GraphSort_OMP_Basic GraphSort;

int main() {
#ifndef NDEBUG
    std::cout << "THIS IS THE DEBUG MODE" << std::endl;

    #pragma omp parallel
    #pragma omp master
    std::cout << "Number of threads: " << omp_get_num_threads() << std::endl;
#endif
	util::rdtsc_timer t;

	// Initializing graph
	GraphSort testgraph_paper(9);
	testgraph_paper.connect(Graph::PAPER); // Constructing graph from paper
	testgraph_paper.topSort();
	testgraph_paper.checkCorrect(true);
	testgraph_paper.printNodeInfo();
	testgraph_paper.viz("paper");
    testgraph_paper.printSolution();
	
	// Initializing graph with random edges
	unsigned const N = 5000;
	GraphSort testgraph_random(N);
	testgraph_random.connect(Graph::RANDOM_EDGES, 0.05); // Constructing graph from paper

	t.start();
	testgraph_random.topSort();
	t.stop();
	
	testgraph_random.checkCorrect(false);
	std::cout << "Elapsed [s]: " << t.sec() << std::endl;
    std::cout << "Elapsed [cycles]: " << t.cycles() << std::endl;
	
	return 0;

}
