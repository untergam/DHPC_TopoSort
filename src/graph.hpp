#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "node.hpp"
#include "analysis.hpp"


class Graph {

	public:

		enum GRAPH_TYPE {PAPER, RANDOM_EDGES};

		typedef std::vector<std::shared_ptr<Node> > nodearray_type;
        typedef std::list<std::shared_ptr<Node> > solution_type;
        
        explicit Graph(unsigned N)
			:	N_(N)
			,	nodes_(nodearray_type(N_))
			,	A_()
		{
			std::cout << "Initialized graph of size " << N_ << "\n";
			for(unsigned i=0; i<N_; ++i) {
				nodes_[i] = std::make_shared<Node>(i);
			}
		}
        void topSort();
        
		void connect(unsigned, double edgeFillDegree = .3);
        bool checkCorrect(bool verbose);
        solution_type getSolution();
        
        // Print and doc methods (graphdoc.cpp)
		void printNodeInfo();
        void printSolution();
		void viz(std::string) const;
        
	protected:

		unsigned N_;
		nodearray_type nodes_;
        solution_type solution_;
        analysis A_;

};


#endif // GRAPH_HPP
