#include "analysis.hpp"


#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>

std::string analysis::suggestBaseFilename(){
    std::string sep = "_";
    #ifdef OPTIMISTIC
    std::string opt = std::to_string(OPTIMISTIC);
    #else
    std::string opt = "0";
    #endif
    
    #ifdef ENABLE_ANALYSIS
    std::string an = std::to_string(ENABLE_ANALYSIS);
    #else
    std::string an = "0";
    #endif    
    std::stringstream ss;
           ss << algorithmName_
           << sep << "opt" << opt
           << sep << "an" << an
           << sep << "t" << nThreads_
           << sep << "p" << nProcs_
           << sep << graphName_
           << sep << "n" << nNodes_
           << sep << "e" << nEdges_
           ;
    return ss.str();
}

bool analysis::xmlAnalysis(std::string relativeDir){
    std::stringstream output;
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    output << "<measurements>\n";
    output << "\t<measurement>\n";
    output << "\t\t<date>" << std::time(nullptr) << "</date>\n";
    output << "\t\t<numberOfThreads>" << nThreads_ << "</numberOfThreads>\n";
    output << "\t\t<processors>" << nProcs_ << "</processors>\n";
    output << "\t\t<totalTime>" << time_Total_ << "</totalTime>\n";
    output << "\t\t<algorithm>" << algorithmName_ << "</algorithm>\n";
    
    #if ENABLE_ANALYSIS == 1
    // in-depth analysis
    output << "\t\t<threads>\n";
    for(size_t i = 0; i < nThreads_; ++i){
        output << "\t\t\t<thread>\n";
        output << "\t\t\t<id>" << i << "</id>\n";
        output << "\t\t\t<timings>\n";
        output << "\t\t\t\t<timing name=\"barrier\">" << timings_[BARRIER][i] << "</timing>\n";
        output << "\t\t\t\t<timing name=\"criticalPushBack\">" << timings_[SOLUTIONPUSHBACK][i] << "</timing>\n";
        output << "\t\t\t\t<timing name=\"criticalRequestValueUpdate\">" << timings_[REQUESTVALUEUPDATE][i] << "</timing>\n";
        output << "\t\t\t</timings>\n";
        output << "\t\t\t<processedNodes>" << count_ProcessedNodes_[i] << "</processedNodes>\n";
        output << "\t\t\t</thread>\n";
    }
    output << "\t\t</threads>\n";
    #endif
    
    output << "\t\t<graph>\n";
    output << "\t\t\t<type>" << graphName_ << "</type>\n";
    output << "\t\t\t<numberOfNodes>" << nNodes_ << "</numberOfNodes>\n";
    output << "\t\t\t<numberOfEdges>" << nEdges_ << "</numberOfEdges>\n";
    output << "\t\t</graph>\n";
    #if OPTIMISTIC==1
    output << "\t\t<optimistic>true</optimistic>\n";
    #else
    output << "\t\t<optimistic>false</optimistic>\n";
    #endif
    
    #if ENABLE_ANALYSIS==1
    output << "\t\t<enableAnalysis>true</enableAnalysis>\n";
    #else
    output << "\t\t<enableAnalysis>false</enableAnalysis>\n";
    #endif
    
    output << "\t</measurement>\n";
    output << "</measurements>\n";

    //Check if relativeDir has delimiter
    if(relativeDir == ""){
        relativeDir = "./";
    }
    else if(relativeDir.back() != '/'){
        relativeDir += "/";
    }
    
    std::string baseFilename = relativeDir + suggestBaseFilename();
    std::string filename = baseFilename + ".xml";  
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 999);

    //Check if file exists
    while(true){
        std::ifstream infile(filename);
        bool exists = infile.is_open();
        if(!exists)
            break;
        infile.close();
        std::string rn = std::to_string(dis(gen));
        filename = baseFilename + "." + rn + ".xml";
    }
    std::ofstream f(filename);
    if(f){
        f << output.str();
        f.close();
    }
    else{
        std::cerr << "Could not open file " << filename << std::endl;
        return false;
    }
    
    std::cout << "Analysis written to: " << filename << std::endl;
    return true;
}