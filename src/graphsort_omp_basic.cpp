#include "graph.hpp"

#include <omp.h>

#ifdef ENABLE_ANALYSIS
#include "rdtsc_timer.hpp"
#endif

void Graph::topSort() {

    #ifdef ENABLE_ANALYSIS
    util::rdtsc_timer rt_total;
    rt_total.start();
    #endif
	// Sorting Magic happens here
	
	unsigned syncVal = 1;
	unsigned nFinished = 0;
	std::vector<short> threadFinished;

	// Spawn OMP threads
	#pragma omp parallel shared(syncVal, nFinished)
	{
        #ifdef ENABLE_ANALYSIS
        util::rdtsc_timer rt_barrier;
        util::rdtsc_timer rt_solution_pushback;
        util::rdtsc_timer rt_requestValueUpdate;
        size_t n_processed_nodes = 0;
        bool hasJustFinished = true;
        #endif
		// Create a Vector entry specifying whether thread is done or not
		#pragma omp critical
		threadFinished.push_back(false);

		// Declare Thread Private Variables
		const int nThreads = omp_get_num_threads();
		const int threadID = omp_get_thread_num();
		std::list<std::shared_ptr<Node> > currentnodes;
		
		std::shared_ptr<Node> parent;
		std::shared_ptr<Node> child;
		unsigned childcount = 0;
		unsigned currentvalue = 0;

		// Distribute Root Nodes among Threads
		for(unsigned i=0; i<N_; ++i) {
			// TODO: find smarter way for distributing nodes
			if(nodes_[i]->getValue()==1 && i%nThreads==threadID) currentnodes.push_back(nodes_[i]);
		}

        #ifdef ENABLE_ANALYSIS
        analysis_.n_initial_currentnodes[threadID] = currentnodes.size();
        #endif
		// TODO: OPTIMIZATION handle and redistribute tasks
	
        #ifdef ENABLE_ANALYSIS
        rt_barrier.start();
        #endif
		#pragma omp barrier // make sure everything is set up alright
        #ifdef ENABLE_ANALYSIS
        rt_barrier.stop();
        analysis_.t_barrier[threadID] += rt_barrier.sec();
        #endif
		unsigned i=0;
		while(i<N_ && nFinished<nThreads) {

			while(!currentnodes.empty()) {
                #ifdef ENABLE_ANALYSIS
                n_processed_nodes++;
                #endif
				parent = currentnodes.front();
				currentvalue = parent->getValue();

				if(currentvalue>syncVal) {
					assert(currentvalue == syncVal+1);
					break;
				} else {
                    #ifdef ENABLE_ANALYSIS
                    rt_solution_pushback.start();
                    #endif
					#pragma omp critical 
					{
						solution_.push_back(parent); // IMPORTANT: this must be atomic
					}
                    #ifdef ENABLE_ANALYSIS
                    rt_solution_pushback.stop();
                    analysis_.t_solution_pushback[threadID] += rt_solution_pushback.sec();
                    #endif
					currentnodes.pop_front(); // remove current node - already visited
				}

				++currentvalue; // increase value for child nodes
				childcount = parent->getChildCount();

				bool flag;
				for(unsigned c=0; c<childcount; ++c) {
					child = parent->getChild(c);

					// Checking if last parent trying to update
                    #ifdef ENABLE_ANALYSIS
                    rt_requestValueUpdate.start();
                    #endif
                        
                    #pragma omp critical
                    {
						flag = child->requestValueUpdate(); // IMPORTANT: this must be atomic
					}
                    #ifdef ENABLE_ANALYSIS
                    rt_requestValueUpdate.stop();
                    analysis_.t_requestValueUpdate[threadID] += rt_requestValueUpdate.sec();
                    #endif
                    if(flag) { // last parent checking child
						currentnodes.push_back(child); // add child node at end of queue
						child->setValue(currentvalue); // set value of child node to parentvalue
					} 
			
				}
			}
			threadFinished[threadID] = (currentnodes.empty() ? 1 : 0);
			#ifdef ENABLE_ANALYSIS
            if(threadFinished[threadID] == 1 && hasJustFinished){
                analysis_.last_syncVal[threadID] = syncVal;
                hasJustFinished = false;
            }
            #endif
            
            
			#pragma omp single
			nFinished = std::accumulate(threadFinished.begin(),threadFinished.end(),unsigned(0));

            #ifdef ENABLE_ANALYSIS
            rt_barrier.start();
            #endif
			#pragma omp barrier
            #ifdef ENABLE_ANALYSIS
            rt_barrier.stop();
            analysis_.t_barrier[threadID] += rt_barrier.sec();
            #endif
			#pragma omp single
            ++syncVal;
			//std::cout << "\nCurrent Depth = " << ++syncVal << std::flush;
			
			++i;
		}
        #ifdef ENABLE_ANALYSIS
            analysis_.n_processed_nodes[threadID] = n_processed_nodes;
        #endif
	} // end of OMP parallel
    
#ifdef ENABLE_ANALYSIS
    rt_total.stop();
    analysis_.t_total = rt_total.sec();
#endif
}
