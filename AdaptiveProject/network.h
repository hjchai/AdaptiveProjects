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
    vector<Vehicle> existed_Vehicles;       //Designed to store those vehicles existing network already
public:
    Network();                              //Default constructor
    Network(string net_File, string tl_File);
    void add_Vehicle(int vehicleID, int startNode, int endNode, int startTime);
    vector<vector<connect>> getCurrentTravelTime();
    void checkState();// In every time step, check out all vehicles to see if they should change their decision or not.
    //Doing
    void update_TLS();
    void update_TLS(int tl_ID);
    int get_Number_Of_Vehicles(int upstreamNode, int currentNode, int downstreamNode);
    //Utility function
    /*
     To Do
     */
    int getTotalTravelTime();
    //Friend classes
    friend class Vehicle;
};

#endif