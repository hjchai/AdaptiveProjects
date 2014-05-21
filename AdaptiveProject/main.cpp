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
            int vehicleID, startNode, endNode, startTime;
            getline(vehicle_Add, skip);
            vehicle_Add >> vehicleID >> startNode >> endNode >> startTime;
            vector<int> tmp({vehicleID, startNode, endNode, startTime});
            vehicle_data.push_back(tmp);
        }
    }
    
    Network net(net_file, tl_file);
    
    int timer = 0;
    
    while (timer < 1000) {
        // read in vehicle data dynamically
        if (!vehicle_data.empty()) {
            vector<vector<int>>::iterator iter = vehicle_data.begin();
            while (*(iter->end()-1) <= timer) {
                net.add_Vehicle(*(iter->begin()), *(iter->begin()+1), *(iter->begin()+2), *(iter->begin()+3));
                vehicle_data.erase(iter);
                if (vehicle_data.empty()) {
                    break;
                }
                iter = vehicle_data.begin();
            }
        }
        
        vector<vector<connect>> currentLinkCost = net.getCurrentTravelTime();
        net.checkState();
        timer ++;
    }
    return 0;
}

