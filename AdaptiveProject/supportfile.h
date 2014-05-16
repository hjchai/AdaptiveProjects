#ifndef SUPPORT_FILE_H
#define SUPPORT_FILE_H

#include <iostream>
#include <vector>

using namespace std;

class Vehicle {
private:
    int vehicle_ID;
    int start_Node;
    int end_Node;
    int current_Link;
    int start_Time;
public:
    Vehicle();
    Vehicle(int vehicleID, int stratNode, int endNode, int currentLink, int startTime);
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
    Traffic_Light(int tlID , int tlCycle , int phaseNumber , vector<phase> phase );
};

#endif