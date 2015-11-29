#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <memory>

#include "node.hpp"
#include "analysis.hpp"


class Graph {

	public:

		enum GRAPH_TYPE {PAPER, RANDOM_EDGES};

		using type_nodeptr = std::shared_ptr<Node>;
		using type_nodearray = std::vector<type_nodeptr>;
        using type_nodelist = std::list<type_nodeptr>;
        using type_solution = type_nodelist; // NB: I would prefer type_nodelist over type_solution - more generic (not all nodelists are solutions, for example currentnodes)
        using type_size = analysis::type_size;
        
        explicit Graph(unsigned N)
			:	N_(N)
			,	maxDiam_(0)
			,	nEdges_(0)
			,	nodes_(type_nodearray(N_))
			,	A_()
		{
			std::cout << "DEBUG = " << DEBUG << "\tVERBOSE = " << VERBOSE << "\tOPTIMISTIC = " << OPTIMISTIC << "\tENABLE_ANALYSIS = " << ENABLE_ANALYSIS << "\n\n";
			std::cout << "Initializing graph of size " << N_ << "...\n";
			for(unsigned i=0; i<N_; ++i) {
				nodes_[i] = std::make_shared<Node>(i);
			}
		}
		analysis::type_time time_topSort() {
			A_.starttotaltiming();
			this->topSort();
			A_.stoptotaltiming();
			std::cout << "\n\nMaximum Diameter: " << maxDiam_;
			std::cout << "\n\n\tSorting completed in:\t" << std::setprecision(8) << std::fixed << A_.time_Total_ << " sec\n\n";
			return A_.time_Total_;
		}
        void topSort();
        
		void connect(unsigned, double edgeFillDegree = .3);
		void countEdges();
        bool checkCorrect(bool verbose);
        type_solution getSolution();
        
        // Print and doc methods (graphdoc.cpp)
		void printNodeInfo();
        void printSolution();
		void viz(std::string) const;
        
	protected:

		type_size N_; // size of graph
		type_size nEdges_; // number of edges
		type_size maxDiam_; // maximal diameter
		type_nodearray nodes_;
        type_solution solution_;
        analysis A_;

};


#if DEBUG>0 || VERBOSE>0
// Overloading output operator of nodelist
// Can be useful for debugging
std::ostream& operator<<(std::ostream& os, Graph::type_nodelist& ls);
#endif // DEBUG>0 || VERBOSE>0


#endif // GRAPH_HPP
