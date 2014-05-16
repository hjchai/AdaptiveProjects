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
    return 0;
}

