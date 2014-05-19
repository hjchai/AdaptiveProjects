#ifndef SUPPORT_FILE_H
#define SUPPORT_FILE_H

#include <iostream>
#include <vector>

using namespace std;

const int max_weight = std::numeric_limits<double>::infinity();


struct connect {
    int tail_Node;
    vector<pair<int, double>> possible_Cost;
    int flow;
    int capacity;
};
struct vehicle_state {
    int current_Node;
    int next_Node;
    int if_At_Next_Node; // defaul:0, at_next_ndoe:1, at_destnation_node:2
    int current_Link_Travel_Time;
};

class Traffic_Light;

class Vehicle {
private:
    int vehicle_ID;
    int start_Node;
    int end_Node;
    vehicle_state online_State;
    int start_Time;
    vector<vector<vector<pair<int, int>>>> SP_Result;//pair<minDis, next_Node>
    int time_Spent = 0;//should be updated in update_Vehicle_State()
    int network_Size; // This variable is deigned to store the network info. Vehicle need to know this.
    
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

struct phase {
    int upstream_Node;
    int downstream_Node;
    double split;
};

class Traffic_Light {
private:
    int tl_ID;
    int tl_Cycle;
    int phase_Number;
    vector<phase> phases;
public:
    Traffic_Light();
    Traffic_Light(int tlID , int tlCycle , int phaseNumber , vector<phase> phase);
    int getPenalty(int time, int upstreamNode, int trafficLightNode, int downstreamNode);
    
    friend class Network;
    friend class Vehicle;
};

int generateRandCost(vector<pair<int, double>>);

#endif