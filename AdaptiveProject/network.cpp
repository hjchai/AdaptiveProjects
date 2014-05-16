#include "network.h"
#include <fstream>
#include <iterator>

vector<connect>::iterator findNode(int node, vector<connect> & adjacent_Nodes) {
    vector<connect>::iterator iter;
    for (iter = adjacent_Nodes.begin(); iter < adjacent_Nodes.end(); iter++) {
        if (iter->tail_Node == node) {
            return iter;
        }
    }
    return iter;
}

//Initialize network object
Network::Network(string net_File, string tl_File) {
    ifstream myfile(net_File.c_str());
    string skip;
    if (myfile.is_open()) {
        int num_of_nodes, head, tail, capacity, ffs, time, length, speed;
        double prob;
        myfile >> skip >> num_of_nodes;
        
        node_Number = num_of_nodes;
        graph.resize(node_Number);
        vector<phase> a;
        phase p = {0,0,0};
        a.push_back(p);
        TLS.resize(node_Number, *new Traffic_Light(-1,-1,-1,a));
        
        for (int i = 0; i < 8; i++) {
            myfile >> skip;
        }
        while (myfile.good()) {
            myfile >> head >> tail >> capacity >> length >> speed >> ffs >> time >> prob;
            vector<connect>::iterator iter = findNode(tail, graph[head-1]);
            if (iter != graph[head-1].end()) {
                iter->possible_Cost.push_back(make_pair(time, prob));
            }
            else {
                vector<pair<int, double>> tmp;
                tmp.push_back(make_pair(time, prob));
                connect adj_Link = {tail, tmp, 0, capacity};
                graph[head-1].push_back(adj_Link);
            }
        }
    }
    else
        cout << "Fail to load network file!" << endl;
    myfile.close();
    
    ifstream tlFile(tl_File.c_str());
    
    int currentNode, fromNode, toNode, cycleLength, numOfPhase;
    double split;
    string indicator;
    phase tmp_phase;
    vector<phase> tmp_phases;
    if(tlFile.is_open())
    {
        getline(tlFile, skip);
        while(tlFile.good())
        {
            tlFile >> currentNode >> indicator >> numOfPhase >> cycleLength;
            if(indicator != "TL")
                continue;
            else
            {
                for(int i = 0; i < numOfPhase; i++)
                {
                    tlFile >> fromNode >> currentNode >> toNode >> split;
                    tmp_phase = {fromNode, toNode, split};
                    tmp_phases.push_back(tmp_phase);
                }
                Traffic_Light tmp_tl = *new Traffic_Light(currentNode, cycleLength, numOfPhase, tmp_phases);
                TLS[currentNode-1] = tmp_tl;
            }
        }
    }
    else
    {
        cout << "Traffic light file does not load!" << endl;
    }
}

