//
//  main.cpp
//  AdaptiveProject
//
//  Created by Huajun Chai on 5/15/14.
//  Copyright (c) 2014 UC Davis. All rights reserved.
//

#include <iostream>
#include "network.h"


int main(int argc, const char * argv[])
{

    // insert code here...
    cout << "Hello, World!\n";
    string net_file = "./data/network.txt";
    string tl_file = "./data/trafficlight.txt";
    /*
     This part is for read in the vehicle file
     */
    string veh_file = "./data/vehicle.txt";
    ifstream vehicle_Add(veh_file.c_str());
    vector<vector<int>> vehicle_data;
    if (!vehicle_Add.is_open()) {
        cout << "Fail to open vehicle data file" << endl;
    }
    else {
        while (vehicle_Add.good()) {
            string skip;
            int vehicleID, startNode, endNode, currentNode, nextNode, if_at_next_node, current_link_travel_time, startTime;
            getline(vehicle_Add, skip);
            vehicle_Add >> vehicleID >> startNode >> endNode >> currentNode >> nextNode >> if_at_next_node >> current_link_travel_time >> startTime;
            vector<int> tmp({vehicleID, startNode, endNode, currentNode, nextNode, if_at_next_node, current_link_travel_time, startTime});
            vehicle_data.push_back(tmp);
        }
    }
    
    Network net(net_file, tl_file);
    int timer = 0;
    net.add_Vehicle(1, 1, 5, 1, 1, 3, -1, -1, 2);//VehicleID, startNode, endNode, state, startTime
    while (timer < 1000) {
        vector<vector<connect>> currentLinkCost = net.getCurrentTravelTime();
        net.checkState();
        timer ++;
    }
    return 0;
}

