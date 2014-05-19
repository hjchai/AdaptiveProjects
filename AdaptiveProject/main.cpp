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
    Network net(net_file, tl_file);
    int timer = 0;
    while (timer < 1000) {
        net.add_Vehicle(1, 1, 5, {1,1,1,-1}, 0);//VehicleID, startNode, endNode, state, startTime
        vector<vector<connect>> currentLinkCost = net.getCurrentTravelTime();
        net.checkState();
        timer ++;
    }
    return 0;
}

