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


vector<vector<int>> read_sensor_data(string filename){
//    read sensor data: beijing
    std::string eachrow;

    std::ifstream myfile(filename);
    std::vector<std::vector<int> > MyVector;

    while (std::getline(myfile, eachrow)){
        std::vector<int> row;
        std::istringstream is(eachrow);
        int x;
        while( is >> x ) {
            row.push_back(x);
        }
        MyVector.push_back(row);
    }

    return MyVector;
}

vector<vector<double>> read_Bayesian_data(string filename){
//    read sensor data: beijing
    std::string eachrow;

    std::ifstream myfile(filename);
    std::vector<std::vector<double> > MyVector;

    while (std::getline(myfile, eachrow)){
        std::vector<double> row;
        std::istringstream is(eachrow);
        double x;
        while( is >> x ) {
            row.push_back(x);
        }
        MyVector.push_back(row);
    }

    return MyVector;
}

#endif /* main_utils_h */
