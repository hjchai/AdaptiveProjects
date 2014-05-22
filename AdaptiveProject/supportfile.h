#ifndef SUPPORT_FILE_H
#define SUPPORT_FILE_H

#include <iostream>
#include <vector>

using namespace std;

const int max_weight = std::numeric_limits<double>::infinity();
const int SIM_LENGTH = 30; // This defines the number of time steps that a node should remeber in shortest path calculation.


struct connect {
    int tail_Node;
    vector<pair<int, double>> possible_Cost;
    int flow;
    int capacity;
    //////
    vector<pair<int, int>> flow_By_Movement; //pair<nextNode, number of vehicles>
};
struct vehicle_state {
    int last_Node;
    int current_Node;
    int next_Node;
    int if_At_Next_Node; // defaul:0, at_next_ndoe:1, at_destnation_node:2
    int current_Link_Travel_Time;
    int time_traveled;//time spent in current link
};

class Traffic_Light;

class Vehicle {
private:
    int vehicle_ID;
    int start_Node;
    int end_Node;
    vehicle_state online_State;
    int start_Time;
    vector<vector<vector<pair<int, int>>>> SP_Result;       //pair<minDis, next_Node>, online shortest path
    int time_Spent = 0;                                     //should be updated in update_Vehicle_State()
    vector<int> route_History;                              //Designed to store this vehicles route history
    int network_Size;                                       // This variable is deigned to store the network info. Vehicle need to know this.
    
public:
    Vehicle();
    Vehicle(int vehicleID, int stratNode, int endNode, vehicle_state onlineState, int startTime, int size);
    void get_SP(int dest_Node, vector<vector<connect>> currentTravelTime, vector<Traffic_Light> currentTLS);
    
    // TO DO's
    void update_Vehicle_State();// To update the state of vehicle during the simulation.
    
    // friend class to make private members available to to Network class.
    friend class Network;
    //friend class Traffic_Light;
};

/************************************
 ***
 For Traffic Lights
 ***
 ***********************************/

/* Phase contains multiple movements. Each movement has a upstream node, and a downstream node.*/
/* Each intersection has a bunch of phases, each phase has a conresponding split. The first phase in phases vector is designed for the always allows movements, like all the right turns if there are any.*/

struct movement {
    int upstream_Node;
    int current_node;
    int downstream_Node;
};

struct phase {
    vector<movement> movements;
    double split;
};

class Traffic_Light {
private:
    int tl_ID;
    int node_ID;
    int tl_Cycle;
    int phase_Number;
    vector<phase> phases;
    vector<movement> always_Allowed_Movements;
    
    int cycle_Time_Eclaps;//This variable is designed to store the time of entire cycles eclapsed, up to now
public:
    Traffic_Light();
    Traffic_Light(int tlID, int nodeID, int tlCycle , int phaseNumber , vector<phase> phase, vector<movement> alwaysAllowedMovement);
    int getPenalty(int time, int upstreamNode, int trafficLightNode, int downstreamNode);
    //friend classes
    friend class Network;
    friend class Vehicle;
};

int generateRandCost(vector<pair<int, double>>);
vector<pair<int, int>>::iterator find(int nextNode, vector<pair<int, int>>&);

#endif