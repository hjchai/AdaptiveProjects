#ifndef NETWORK_H
#define NETWORK_H

#include "supportfile.h"
#include <string>
#include <utility>
#include <fstream>
#include <iterator>



class Network {
private:
    int node_Number;
    vector<Vehicle> vehicles;               //Define vehicles within the network
    vector<Traffic_Light> TLS;              //Define traffic lights in the network
    vector<vector<connect>> graph;          //Define a graph, which contains link info, and node info
public:
    Network();                              //Default constructor
    Network(string net_File, string tl_File);
    void add_Vehicle(int vehicleID, int startNode, int endNode, int currentNode, int nextNode, int if_at_next_node, int current_link_travel_time, int time_traveled, int startTime);
    vector<vector<connect>> getCurrentTravelTime();
    void checkState();// In every time step, check out all vehicles to see if they should change their decision or not.
    
    friend class Vehicle;
};

#endif