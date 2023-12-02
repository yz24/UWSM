//
// Created by Yanhui Zhu on 3/26/23.
//
#ifndef SUBMODULAR_MODULAR_UNCONSTRAINTSUBMODULARMODULAR_H
#define SUBMODULAR_MODULAR_UNCONSTRAINTSUBMODULARMODULAR_H

#include <stdio.h>
#include "../Graph/Graph.hpp"
#include <set>
#include "../IM/TIMInfluenceCalculator.hpp"
#include "../IM/TIMCoverage.hpp"
#include "../IM/TIMNormalizedCoverage.hpp"
#include "ModularCostGenerator/ModularCostGenerator.hpp"
//#include "../log.h"
#include <limits.h>
#include <map>
#include "stdlib.h"
//#include "BloomFilter.h"
#include <iostream>
#include <bitset>
#include <random>
#include <set>
#include <cstdlib>
#include <queue>
using namespace std;


class UnconstraintSubmodularModular{
    vector<int> orderedSeed;
    int nNodes;
    double epsilon;
    double alpha;
    string application;
    vector<vector<int>> *rrSet;
    double objectiveFminusC;
    double f_value;
    double c_value;

    Graph *graph;
    vector<vector<int>> sensor_data;
    vector<double> modularCosts;
    shared_ptr<TIMCoverage> timCoverage;


public:
    UnconstraintSubmodularModular(string model, Graph *graph, double alpha);

    pair<int, double> findMaxNode(const set<int>&);
    pair<int, double> findMaxDensity(const set<int>&, double);

    double f_X(set<int> X);
    double calculateCost(set<int> S);
    void setModularCost(vector<double> costs);

    pair<int, double> distortedGreedy();
    pair<int, double> ROI();
    pair<int, double> UP(double epsilon);


};

#endif //SUBMODULAR_MODULAR_UNCONSTRAINTSUBMODULARMODULAR_H