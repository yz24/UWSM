//
// Created by Yanhui Zhu on 3/26/23.
//
#include "UnconstraintSubmodularModular.h"

UnconstraintSubmodularModular::UnconstraintSubmodularModular(string model, Graph *graph, double alpha) {
    this->graph = graph;
    this->application = model;
    this->alpha = alpha;
    this->nNodes = graph->getNumberOfVertices();
}


pair<int, double> UnconstraintSubmodularModular::findMaxNode(const set<int> &seedSetG) {
    int maxMarginalNode = -1;
    double maxMarginalValue = -1;
    for(int j = 0; j < nNodes; j++){
        if(seedSetG.count(j) == 0){
            set<int> candidate = seedSetG;
            candidate.insert(j);
            double inf = f_X(candidate);
            if(inf > maxMarginalValue){
                maxMarginalValue = inf;
                maxMarginalNode = j;
            }
        }
    }

    return make_pair(maxMarginalNode, maxMarginalValue);
}

pair<int, double> UnconstraintSubmodularModular::findMaxDensity(const set<int> &seedSet, double seedSet_f_value) {
    double maxDensity = 0;
    int maxNodeId = -1;
    double maxMarginalGain = 0;
    for(int j = 0; j < nNodes; j++){
        if(seedSet.count(j) == 0){
//            double obj = f_X(seedSet);
            set<int> temp = seedSet;
            temp.insert(j);
            double marginalGain = f_X(temp)-seedSet_f_value;
            double density = marginalGain/modularCosts[j];
            if(density > maxDensity){
                maxDensity = density;
                maxNodeId = j;
                maxMarginalGain = marginalGain;
            }
        }
    }
    return make_pair(maxNodeId, maxMarginalGain);
}



double UnconstraintSubmodularModular::f_X(set<int> X) {
    if(X.size()==0){
        return 0;
    }
    if(this->application=="Influence-Maximization"){
        return timCoverage->findInfluence(X);
    }
    else if(this->application=="Max-Coverage"){
        return graph->reachableNodesWithinLevel(X, 2).size();
    }
    return -1;
}

double UnconstraintSubmodularModular::calculateCost(set<int> S) {
    double c = 0.0;
    for (auto b : S){
        c += modularCosts[b];
    }
    return c;
}

void UnconstraintSubmodularModular::setModularCost(vector<double> costs) {
    this->modularCosts = costs;
}

pair<int, double> UnconstraintSubmodularModular::ROI() {

    TIMInfluenceCalculator timInfluenceCalculator(graph, 2);
    shared_ptr<TIMCoverage> timCoverages = timInfluenceCalculator.getTimCoverage();
    this->timCoverage = timCoverages;

    cout<<"\n-----------ROI-------------"<<endl;
    set<int> seedSet = set<int>();
    double seedSet_f_value = 0;
    int num_evaluations = 0;
    for(int i = 0; i < nNodes; i++){
        // get the best density *node id* and *marginal gain value* (NOT density)
        pair<int, double> bestMarginalDensity = findMaxDensity(seedSet, seedSet_f_value);
        num_evaluations += (nNodes - seedSet.size());
        int bestNodeId = bestMarginalDensity.first;
        double bestGain = bestMarginalDensity.second;
        if(bestNodeId != -1 && bestGain > modularCosts[bestNodeId]){
            seedSet.insert(bestNodeId);
            seedSet_f_value += bestGain;
            cout<< "Adding element "<<bestNodeId << "; current f-c = " << seedSet_f_value-calculateCost(seedSet)<<endl;
        }else{
            double objective = seedSet_f_value- calculateCost(seedSet);
            cout<<"-- f-c: "<<objective<<endl;
            cout<<"-- seed set size: "<<seedSet.size()<<endl;
            cout<<"-- function calls: "<<num_evaluations<<endl;
            return make_pair(num_evaluations, objective);
        }
    }

    return make_pair(num_evaluations, nNodes);

}


pair<int, double> UnconstraintSubmodularModular::distortedGreedy() {

    TIMInfluenceCalculator timInfluenceCalculator(graph, 2);
    shared_ptr<TIMCoverage> timCoverages = timInfluenceCalculator.getTimCoverage();
    this->timCoverage = timCoverages;

    cout<<"\n-----------Distorted Greedy-------------"<<endl;
    set<int> seedSet = set<int>();
    int num_evaluations = 0;
    double seedSet_f_value = 0;
    for(int j = 0; j<=nNodes; j++){
        // sample a node uniformly
        auto ran = std::rand() % this->nNodes;
        if(seedSet.count(ran) == 0){
            set<int> temp = seedSet;
            temp.insert(ran);
            double temp_f_value = f_X(temp);
            num_evaluations ++;
            if( pow(1 - alpha/nNodes, nNodes-j-1)*(temp_f_value - seedSet_f_value) > modularCosts[ran] ){
                seedSet.insert(ran);
                seedSet_f_value = temp_f_value;
               cout<< "Adding element "<<ran << "; current f-c = " << seedSet_f_value-calculateCost(seedSet)<<endl;
            }
        }
    }
//    double objective = f_X(seedSet)- calculateCost(seedSet);
    double objective = seedSet_f_value- calculateCost(seedSet);
//    cout<<"***"<<endl;
//    cout<<"-- Seed Set --"<<endl;
//    print(seedSet);
    cout<<"-- f-c: "<<objective<<endl;
//    cout<<"-- seed set size: "<<seedSet.size()<<endl;
    cout<<"-- function calls: "<<num_evaluations<<endl;
//    cout<<"------------------------"<<endl;
    return make_pair(num_evaluations, objective);
}


pair<int, double> UnconstraintSubmodularModular::UP(double epsilon) {

    TIMInfluenceCalculator timInfluenceCalculator(graph, 2);
    shared_ptr<TIMCoverage> timCoverages = timInfluenceCalculator.getTimCoverage();
    this->timCoverage = timCoverages;
    cout<<"\n----------- UP - epsilon = "<<epsilon<<"-----------"<<endl;

    set<int> seedSet = set<int>();
    double seedSet_f_value = 0;
    int num_evaluations = 0;
    priority_queue<pair<double, int> > pq;
    vector<int> counters(nNodes, 0);
    int delta = int(log(nNodes/(alpha*epsilon))/epsilon);
    // initialize the priority queue
    for(int i = 0; i<nNodes; i++){
        set<int> temp = {i};
        pq.push(make_pair(f_X(temp)/modularCosts[i], i));
        num_evaluations ++;
    }

    // check if the feasibility of the best node
    pair<double, int> bestDensity = pq.top();
    if(bestDensity.first>alpha){
        seedSet.insert(bestDensity.second);
        seedSet_f_value = bestDensity.first * modularCosts[bestDensity.second];
//        cout<< "Adding element "<<bestDensity.second << "; current f-c = " << seedSet_f_value-calculateCost(seedSet)<<endl;
        pq.pop();
    }
    else{
        cout<<"No feasible solution."<<endl;
        return {};
    }
    // check the remaining nodes
    for(int i = 1; i < nNodes; i++){
        while(!pq.empty()){
            pair<double, int> top = pq.top();
            pq.pop();
            if(top.first <= alpha){
                break;
            }
            set<int> temp_set = seedSet;
            temp_set.insert(top.second);

            // calculate the density if adding it
            double new_density = (f_X(temp_set) - seedSet_f_value)/modularCosts[top.second];
            num_evaluations ++;
            counters[top.second] ++;

            if(new_density >= (1-epsilon)*top.first){
                seedSet.insert(top.second);
                seedSet_f_value += new_density*modularCosts[top.second];
//                cout<< "Adding element "<<top.second << "; current f-c = " << seedSet_f_value-calculateCost(seedSet)<<endl;
                break;
            }
            // push the node back to the queue
            if(counters[top.second] <= delta){
                pq.push(make_pair(new_density, top.second));
            }

        }

    }

   cout<<"-- seed set size: "<<seedSet.size()<<endl;
    cout<<"-- function calls: "<<num_evaluations<<endl;
    cout<<"f-c="<<seedSet_f_value- calculateCost(seedSet)<<endl;
    return make_pair(num_evaluations, seedSet_f_value- calculateCost(seedSet));
}