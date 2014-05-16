#include "supportfile.h"

Vehicle::Vehicle(int vehicleID, int startNode, int endNode, int currentLink, int startTime) {
    vehicle_ID = vehicleID;
    start_Node = startNode;
    end_Node = endNode;
    current_Link = currentLink;
    start_Time = startTime;
}

/************************************
 ***
 For Traffic Lights
 ***
 ***********************************/

Traffic_Light::Traffic_Light(int tlID, int tlCycle, int phaseNumber, vector<phase> phase) {
    tl_ID = tlID;
    tl_Cycle = tlCycle;
    phase_Number = phaseNumber;
    phases = phase;
}