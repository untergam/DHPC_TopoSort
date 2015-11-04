#ifndef DIRGRAPH_HPP
#define DIRGRAPH_HPP

#include <iostream>
#include <vector>
#include <algorithm>

#include "node.hpp"

class DirGraph {

	public:

		enum GRAPH_TYPE {PAPER, RANDOM_EDGES};

		typedef std::vector<Node*> nodearray_type;
		// TODO: memory leak here - using smart pointers? 

		DirGraph(unsigned N)
			: N_(N)
			, nodes_(nodearray_type(N_))
		{
			std::cout << "Initialized graph of size " << N_ << "\n";
			for(unsigned i=0; i<N_; ++i) {
				// TODO: memory leak here - using smart pointers? 
				nodes_[i] = new Node(i);
			}
		}

		void printNodeInfo();

		void topSort();
		void connect(unsigned);
		bool checkCorrect();

		void viz(std::string) const;


	private:

		unsigned N_;
		nodearray_type nodes_;

};


#endif // DIRGRAPH_HPP
