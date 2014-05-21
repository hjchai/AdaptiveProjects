#include "supportfile.h"

Vehicle::Vehicle(int vehicleID, int startNode, int endNode, vehicle_state onlineState, int startTime, int size) {
    vehicle_ID = vehicleID;
    start_Node = startNode;
    end_Node = endNode;
    online_State = onlineState;
    start_Time = startTime;
    //route_History.resize(1,startNode);
    network_Size = size; // This info is number of node in network
}

void Vehicle::get_SP(int dest_Node, vector<vector<connect>> currentTravelTime, vector<Traffic_Light> currentTLS) {
    int timeStep = start_Time + time_Spent;
    
    SP_Result.resize(network_Size);
    for(int i = 0; i < network_Size; i++)
    {
        int k = (int)currentTravelTime[i].size();
        int cost;
        if (i == dest_Node)
            cost = 0;
        else
            cost = max_weight;
        SP_Result[i].resize(k+1, vector<pair<int, int>>(SIM_LENGTH, make_pair(cost, -1)));
    }
    
    vector<int> vertex_queue;
    vertex_queue.push_back(dest_Node);
    
	while(!vertex_queue.empty())
	{
		int currentNode = *vertex_queue.begin();
		vertex_queue.erase(vertex_queue.begin());
        
        // Adjacent nodes of current node.
		vector<connect> adjs = currentTravelTime[currentNode];
        int i_index = 0;
		for(vector<connect>::const_iterator iter = adjs.begin(); iter < adjs.end(); iter++)//iteration over all adjacnet nodes//For each adjacent tail node of current node
		{
			int h_index = 0;
            int tailNode = iter->tail_Node ;//The selected adjacent node
            int phaseNum = currentTLS[tailNode].phase_Number;
            int linkcost = iter->possible_Cost[0].first;
            
            for(vector<connect>::const_iterator iter1 = currentTravelTime[tailNode].begin(); iter1 < currentTravelTime[tailNode].end()+1; iter1++)//consider all the approaches//For each adjacent tail node of the selected node
            {
                for (int index = 0; index < SIM_LENGTH; index++) { //loop over all simulation time steps
                    /******
                     IT MAY BE IMPROVED INTERMS OF CALCULATION TIME, SINCE FOR SOME NODES, THERE ARE CYCLES
                     ******/
                    int h = 0;//Some indicator to denote if the demand originates from this node or not
                    if(iter1 == currentTravelTime[tailNode].end())
                        h = -1;//indicates that demand originates from this node
                    else
                        h = iter1->tail_Node ;
                    int j = currentNode;
                    int i = tailNode;
                    
                    //calculate penalty
                    int penalty = 0;
                    if(phaseNum > 0 && h != -1)
                        penalty = currentTLS[i].getPenalty(index+timeStep, h, i, j);// it is important to include timeStep here. Because this SP calculation is performed at very time "timeStep"!
                    double tmp_label = 0;
                    int m = index + penalty + linkcost;
                    
                    tmp_label = penalty + linkcost + SP_Result[j][i_index][m].first;
                    
                    if(tmp_label < SP_Result[i][h_index][index].first)
                    {
                        SP_Result[i][h_index][index] = make_pair(tmp_label, j);
                        if(find(vertex_queue.begin(), vertex_queue.end(), i) == vertex_queue.end())
                            vertex_queue.push_back(i);
                    }
                }
                h_index++;
            }
            i_index++;
		}
	}
}
/************************************
 ***
 For Traffic Lights
 ***
 ***********************************/

Traffic_Light::Traffic_Light(int tlID, int tlCycle, int phaseNumber, vector<phase> phase, vector<movement> alwaysAllowedMovements) {
    tl_ID = tlID-1;
    tl_Cycle = tlCycle;
    phase_Number = phaseNumber;
    phases = phase;
    always_Allowed_Movements = alwaysAllowedMovements;
}

int Traffic_Light::getPenalty(int timeStep, int upstreamNode, int trafficLightNode, int downstreamNode)
{
    if (this->phase_Number == -1) {
        return 0;
    }
    int b = timeStep % tl_Cycle;
    vector<pair<int, int>> index;
    int time_start = 0;
    int time_end = 0;
    for(vector<phase>::const_iterator iter = phases.begin(); iter < phases.end(); iter++)
    {
        time_end = time_start + iter->split * tl_Cycle;
        for (vector<movement>::const_iterator iter1 = iter->movements.begin(); iter1 < iter->movements.end(); iter1++) {
            if(iter1->upstream_Node == upstreamNode && iter1->downstream_Node == downstreamNode)
            {
                index.push_back(make_pair(time_start, time_end));
            }
        }
        time_start += time_end;
    }
    for(vector<pair<int, int>>::const_iterator itr = index.begin(); itr < index.end(); itr++)
    {
        pair<int, int> found = *itr;
        if(found.second > b)
        {
            return (found.first>b ? found.first-b : 0);
        }
    }
    return (tl_Cycle -b);
}


/************************************
 ***
 Some general functions
 ***
 ***********************************/
int generateRandCost(vector<pair<int, double>> possibleValue) {
    double rnd = rand()/(double)(RAND_MAX);
    double a = 0;
    for (vector<pair<int, double>>::const_iterator iter = possibleValue.begin(); iter < possibleValue.end(); iter++) {
        a += iter->second;
        if (rnd <= a) {
            return iter->first;
        }
    }
    return possibleValue.begin()->first;
}