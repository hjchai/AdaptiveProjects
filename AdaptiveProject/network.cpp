#include "network.h"

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
            vector<connect>::iterator iter = findNode(tail-1, graph[head-1]);
            if (iter != graph[head-1].end()) {
                iter->possible_Cost.push_back(make_pair(time, prob));
            }
            else {
                vector<pair<int, double>> tmp;
                tmp.push_back(make_pair(time, prob));
                connect adj_Link = {tail-1, tmp, 0, capacity};
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
    if(tlFile.is_open())
    {
        getline(tlFile, skip);
        while(tlFile.good())
        {
            vector<phase> tmp_phases;
            tlFile >> currentNode >> indicator >> numOfPhase >> cycleLength;
            if(indicator != "TL")
                continue;
            else
            {
                for(int i = 0; i < numOfPhase; i++)
                {
                    tlFile >> fromNode >> currentNode >> toNode >> split;
                    tmp_phase = {fromNode-1, toNode-1, split};
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

// Add a vehicle into the network
void Network::add_Vehicle(int vehicleID, int startNode, int endNode, int currentNode, int nextNode, int if_at_next_node, int current_link_travel_time, int time_traveled, int startTime) {
    struct vehicle_state onlineState = {currentNode-1, nextNode-1, if_at_next_node, current_link_travel_time, time_traveled};
    Vehicle newVehicle = *new Vehicle(vehicleID, startNode-1, endNode-1, onlineState, startTime, node_Number);
    vehicles.push_back(newVehicle);
}

// Retrive the current link travel time
vector<vector<connect>> Network::getCurrentTravelTime() {
    vector<vector<connect>> linkInfo;
    linkInfo.resize(node_Number);
    int counter = 0;
    for (vector<vector<connect>>::const_iterator iter = graph.begin(); iter < graph.end(); iter++) {
        for (vector<connect>::const_iterator iter1 = iter->begin(); iter1 < iter->end(); iter1++) {
            vector<pair<int, double>> possibleValue;
            possibleValue.clear();
            possibleValue = iter1->possible_Cost;
            int randCost = generateRandCost(possibleValue);
            int tailNode = iter1->tail_Node;
            vector<pair<int, double>> tmp;
            tmp.push_back(make_pair(randCost,1));
            connect con = {tailNode, tmp, -1, -1};
            linkInfo[counter].push_back(con);
        }
        counter++;
    }
    return linkInfo;
}

void Network::checkState() {
    for (vector<Vehicle>::iterator iter = vehicles.begin(); iter < vehicles.end(); iter++) {
        if (iter->online_State.if_At_Next_Node == 2) {
            
            vehicles.erase(iter);
            //remove_this_vehicle, and do some update work;
            
        }
        else if (iter->online_State.if_At_Next_Node == 0) {//In the middle of the link
            int tmp1 = iter->time_Spent;
            tmp1++;
            iter->time_Spent = tmp1;
            int tmp = iter->online_State.time_traveled;
            tmp++;
            iter->online_State.time_traveled = tmp;
            if (tmp == iter->online_State.current_Link_Travel_Time) {
                if (iter->online_State.next_Node != iter->end_Node)
                    iter->online_State.if_At_Next_Node = 1;//change vehicle state to 1, means arrived at next node
                else
                    iter->online_State.if_At_Next_Node = 2;//change vehicle state to 2, emans arrived at dest node.
            }
        }
        else {//At next node, but not at dest node. Mode 1&3. 3 means origins from this node, 1 means reachs next node.
            vector<vector<connect>> currentTravelTime = getCurrentTravelTime();//calculate a new route at the current node.
            iter->get_SP(iter->end_Node, currentTravelTime, TLS);
            /*
             Update vehicle state after a shortest path finding
             */
            struct vehicle_state state_tmp = iter->online_State;
            int from = state_tmp.current_Node, current = state_tmp.next_Node;
            int dis = 0;
            if (iter->online_State.if_At_Next_Node == 3) {
    
                dis = (int)graph[current].size();
            }
            else {
                vector<connect>::iterator iter1 = findNode(from, graph[current]);
                dis = (int)distance(graph[current].begin(), iter1);//this is the index of from node in the current node's adjacent list
            }
            int to = iter->SP_Result[current][dis][0].second;
            
            int dis1 = (int)distance(graph[to].begin(), findNode(current, graph[to]));//this is the index of current node in the to node's adjacent list
            //UPDATE link flow state
            if (iter->online_State.if_At_Next_Node == 1) {
                int dis2 = (int)distance(graph[current].begin(), findNode(from, graph[current]));
                graph[current][dis2].flow = graph[current][dis2].flow - 1;
            }
            int dis3 = (int)distance(graph[to].begin(), findNode(current, graph[to]));
            graph[to][dis3].flow = graph[to][dis3].flow + 1;
            
            // UPDATE vehicle state
            iter->online_State.current_Node = current;//change current node
            iter->online_State.next_Node = to;//change next node
            iter->online_State.if_At_Next_Node = 0;//change link state from 1 to 0
            iter->online_State.time_traveled = 1;//start traveling in the new link
            iter->online_State.current_Link_Travel_Time = currentTravelTime[to][dis1].possible_Cost[0].first;//change current travel time
            iter->time_Spent = iter->time_Spent + 1;
        }
    }
}
