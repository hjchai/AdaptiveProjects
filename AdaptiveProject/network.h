#ifndef NETWORK_H
#define NETWORK_H

#include "supportfile.h"
#include <string>
#include <utility>

class Network {
private:
    int node_Number;
    vector<Vehicle> vehicles;               //Define vehicles within the network
    vector<Traffic_Light> TLS;              //Define traffic lights in the network
    vector<vector<connect>> graph;          //Define a graph, which contains link info, and node info
public:
    Network();                              //Default constructor
    Network(string net_File, string tl_File);
    void add_Vehicle(int vehicleID, int startNode, int endNode, int currentLink, int startTime);
};

#endif