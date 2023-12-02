//
//  main_utils.h
//  InfluenceMaximization
//
//  Created by Madhavan R.P on 5/6/19.
//  Copyright © 2019 Madhavan R.P. All rights reserved.
//

#ifndef main_utils_h
#define main_utils_h

#include <iostream>
#include <fstream>
#include <ctime>
#include "Graph/Graph.hpp"
#include "memoryusage.h"
#include <sstream>



//Graph *createGraphObject(cxxopts::ParseResult result) {
//    string graphFile = result["graph"].as<string>();
//    Graph *graph = new Graph;
//    graph->readGraph(graphFile);
//    return graph;
//}

Graph *createGraphObject(string graphFile) {
    Graph *graph = new Graph;
    graph->readGraph(graphFile);
    return graph;
}


#endif /* main_utils_h */
