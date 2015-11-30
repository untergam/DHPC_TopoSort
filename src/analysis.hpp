#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include "rdtsc_timer.hpp"

#ifdef ENABLE_ANALYSIS
#include <vector>
#include <map>
#include <ostream>

struct analysis {

	// TYPES AND VARIABLES
	enum timecat {BARRIER,SOLUTIONPUSHBACK,REQUESTVALUEUPDATE,CURRENTGATHER,CURRENTSCATTER,N_TIMECAT};
	using type_time = double;
	using type_size = unsigned;
	using type_threadcount = short;
	using type_countmap = std::map<type_threadcount , type_size>;
	using type_timingmap = std::map<type_threadcount ,type_time>;
	using type_timingvector = std::vector<type_timingmap>;
	using type_clock = util::rdtsc_timer;
	using type_clockvector = std::vector<type_clock>;

	analysis()
		:	count_InitialNodes_()
		,	count_ProcessedNodes_()
		,	count_LastSyncVal_()
		,	time_Total_(0)
		,	nThreads_(0)
		,	clocks_(N_TIMECAT)
		,	timings_(N_TIMECAT,type_timingmap())
	{}

	type_countmap count_InitialNodes_;		// counts how many nodes each thread has initially
	type_countmap count_ProcessedNodes_;	// counts how many nodes each thread has processed in total
	type_countmap count_LastSyncVal_;		// keeps track of the last sync value of each thread
	type_time time_Total_;
	type_clockvector clocks_;
	type_clock totalclock_;
	type_timingvector timings_;
    
    type_threadcount nThreads_;
    type_threadcount nProcs_;
    std::string algorithmName_;
    type_size nNodes_;
    type_size nEdges_;
    std::string graphName_;
    
	// FUNCTIONS
	
	inline void initialnodes(type_threadcount tid, type_size nNodes) {
		count_InitialNodes_[tid] = nNodes;
	}
	
	inline void processednodes(type_threadcount tid, type_size nNodes) {
		count_ProcessedNodes_[tid] = nNodes;
	}
	
	inline void incrementProcessedNodes(type_threadcount tid) { // TODO: check if this can be used instead of processednodes (performance??)
		++count_ProcessedNodes_[tid];
	}

	inline void starttotaltiming();
	
	inline void starttiming(timecat c) {
		clocks_[c].start();
	}
	

	inline void stoptotaltiming();
	
	inline void stoptiming(type_threadcount tid, timecat c) {
		// c stands for the index of the time category we are measuring
		// tid is the thread id
		clocks_[c].stop(); // stop timing
		timings_[c][tid] += clocks_[c].sec(); // get time in seconds and add to total (for given thread)
	}

    void printAnalysis(std::ostream& out);
    bool xmlAnalysis(std::string relativeDir);
private:
    std::string suggestBaseFilename();

};

#else // declare empty inline functions - they will disappear

struct analysis {

	enum timecat {BARRIER,SOLUTIONPUSHBACK,REQUESTVALUEUPDATE,CURRENTGATHER,CURRENTSCATTER,N_TIMECAT};
	using type_time = double;
	using type_size = unsigned;
	using type_clock = util::rdtsc_timer;
	using type_threadcount = short;
	
	type_time time_Total_;
	type_clock totalclock_;

	analysis() {}

	inline void initialnodes(type_threadcount tid, type_size nNodes) {}
	inline void processednodes(type_threadcount tid, type_size nNodes) {}
	inline void incrementProcessedNodes(type_threadcount tid) {} // TODO: check if this can be used instead of processednodes (performance??)
	inline void starttotaltiming();
	inline void starttiming(timecat c) {}
	inline void stoptotaltiming();
	inline void stoptiming(type_threadcount tid, timecat c) {}
	inline void threadcount(type_threadcount n) {}
    void printAnalysis(std::ostream& out){}
    bool xmlAnalysis(std::string relativeDir){}

};

#endif // ENABLE_ANALYSIS


inline void analysis::starttotaltiming() {
	totalclock_.start();
}

inline void analysis::stoptotaltiming() {
	totalclock_.stop();
	time_Total_ = totalclock_.sec();
}


#endif // ANALYSIS_HPP
