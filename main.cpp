#include <iostream>
using namespace std;
#include <string>
#include <queue>
#include <ctime>
#include "vector"
#include <fstream>
#include "Graph/Graph.hpp"
#include "main_utils.h"
#include "RISCalculation/RISCalcSpaceEfficient.hpp"
#include <random>
#include "ModularCostGenerator/ModularCostGenerator.hpp"
#include "SubmodularModular/UnconstraintSubmodularModular.h"
#include "SubmodularModular/CardinalitySubmodularModular.h"
#include "Eigen/Dense"
#include "BayesianAOptimal.h"
using Eigen::MatrixXd;
using Eigen::VectorXd;

vector<double> getIMModularCosts(Graph *graph, double lambda, double gamma) {
    return ModularCostGenerator::generateOutDegreeBasedIMModularCosts(graph, lambda, gamma);
}

vector<double> getVertexCoverModularCosts(Graph *graph, double penalty) {
    return ModularCostGenerator::generateVertexCoverModularCosts(graph, penalty);
}


int main(int argc, char **argv) {
    cout << "Starting program\n";

    Graph *graph = createGraphObject("graph_ic.inf");
    UnconstraintSubmodularModular USM("Influence-Maximization", graph, 1.0);
    pair<int, double> seed1 = USM.ROI();
    pair<int, double> seed2 = USM.distortedGreedy();
    pair<int, double> seed31 = USM.UP(0.1);
    pair<int, double> seed32 = USM.UP(0.2);
    pair<int, double> seed33 = USM.UP(0.5);

    delete graph;

    disp_mem_usage("");
    return 0;

}
