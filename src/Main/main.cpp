#include <iostream>
#include <sstream>

#include "data_structures/createGraphs.h"
#include "data_structures/Graph.h"
#include "Modes/driving.h"

void CommandLine(Graph<int> &g);
void ModeDriving(Graph<int> &g, int source, int destination);
void ModeDrivingRestrictions(Graph<int> &g, int source, int destination);
void ModeDrivingandWalking(Graph<int> &g, int source, int destination, int maxWalkTime);
void avoidNodesLine(Graph<int> &g);
void avoidSegmentLine(Graph<int> &g);


bool enablepreconfig = false; //enable pre-configurations for the menu

/*
PRESS "q" ON THE CONSOLE TO ENABLE PREVIOUSLY DEFINED OPTIONS
*/

std::string presetmode = "driving"; //chose wherever you want
int presetsource = 2; //chose wherever you want -----> ex: "2"
int presetdestination = 8; //chose wherever you want ----> ex "7"
std::string presetrestrictions = "N"; //chose between "Y" or "N"
std::string presetavoidNodes = ""; //chose wherever you want ----> ex "5,8"
std::string presetavoidSegment = ""; //chose wherever you want ----> ex "(1,2),(3,7)"
std::string presetIncludeNode = ""; //chose wherever you want ----> ex "4"
int presetmaxWalkTime = 10;

/*
PRESS "q" ON THE CONSOLE TO ENABLE PREVIOUSLY DEFINED OPTIONS
*/

int main() {
    string folder = "../../DA2425_PRJ1_G75/src/Main/For_Students";

    bool CML = true;
    while (CML) {
        Graph<int> g = createGraphs::graphFromFile(folder);
        string input;
        std::cout<< "If you want to Use Command Line press 'Y'" <<endl;
        std::cin >> input;
        if (input == "e") {
            enablepreconfig = true;
            CommandLine(g);
        }
        if (input == "Y" or input == "y") {
            CommandLine(g);
        }else {
            CML = false;
        }
    }

    return 0;
}

void CommandLine(Graph<int> &g) {
    if (enablepreconfig == true) {
        if (presetrestrictions == "Y") {
            std::getline(std::cin, presetavoidNodes);
            std::istringstream iss(presetavoidNodes);
            int Vertex;
            while (iss >> Vertex) {
                g.findVertex(Vertex)->setAvailable(-1);
            }

            std::istringstream iss2(presetavoidSegment);
            char discard;

            while (iss2 >> discard && discard == '(') {
                int source, destination;
                char comma;

                if (!(iss >> source >> comma >> destination)) {
                    break;
                }

                iss >> discard;
                if (discard != ')') {
                    break;
                }
                g.findVertex(source)->removeEdge(destination);
                g.findVertex(destination)->removeEdge(source);

                if (iss.peek() == ',') {
                    iss.ignore();
                }
            }

            std::istringstream iss3(presetIncludeNode);
            int Vertex2;
            while (iss >> Vertex2) {
                g.findVertex(Vertex2)->setAvailable(1);
                g.includenodevar = Vertex2;
            }

            ModeDrivingRestrictions(g, presetsource, presetdestination);
        } else if (presetmode == "Driving-walking" || presetmode == "driving-walking") {
            ModeDrivingandWalking(g, presetsource, presetdestination, presetmaxWalkTime);
        } else {
            ModeDriving(g, presetsource, presetdestination);
        }
    } else {
        std::cout << "If you want Restrictions press Y or N" << std::endl;
        std::string input;
        std::cin >> input;
        std::cout << "Choose one of the Modes (Driving, Driving-walking)" << std::endl;

        std::string mode;
        int source;
        int destination;
        std::cout << "Mode: "; std::cin >> mode;
        std::cout << "Source: "; std::cin >> source;
        std::cout << "Destination: "; std::cin >> destination;

        if (input == "Y" || input == "y") {
            if (mode == "Driving" || mode == "driving") {
                ModeDrivingRestrictions(g, source, destination);
            }
        } else if (input == "N" || input == "n") {
            if (mode == "Driving" || mode == "driving") {
                ModeDriving(g, source, destination);
            } else if (mode == "Driving-walking" || mode == "driving-walking") {
                int maxWalkTime;
                std::cout << "MaxWalkTime: "; std::cin >> maxWalkTime;
                ModeDrivingandWalking(g, source, destination, maxWalkTime);
            }
        }
    }
}


void avoidNodesLine(Graph<int> &g) {
    std::string avoidNodes;
    std::getline(std::cin, avoidNodes);
    std::istringstream iss(avoidNodes);
    int Vertex;
    while (iss >> Vertex) {
        g.findVertex(Vertex)->setAvailable(-1);
    }

}

void avoidSegmentLine(Graph<int> &g) {
    std::string avoidSegment;
    std::getline(std::cin, avoidSegment);
    std::istringstream iss(avoidSegment);
    char discard;

    while (iss >> discard && discard == '(') {
        int source, destination;
        char comma;

        if (!(iss >> source >> comma >> destination)) {
            break;
        }

        iss >> discard;
        if (discard != ')') {
            break;
        }
        g.findVertex(source)->removeEdge(destination);
        g.findVertex(destination)->removeEdge(source);

        if (iss.peek() == ',') {
            iss.ignore();
        }
    }

}

void includeNode(Graph<int> &g) {
    std::string IncludeNode;
    std::getline(std::cin, IncludeNode);
    std::istringstream iss(IncludeNode);
    int Vertex;
    while (iss >> Vertex) {
        g.findVertex(Vertex)->setAvailable(1);
        g.includenodevar = Vertex;
    }
}

void ModeDriving(Graph<int> &g, int source, int destination) {
    dijkstra(&g, source);
    std::vector<int> bestDrivingRoute = getPath(&g, source, destination);
    int cost1 = getCost(&g, destination);
    for (int i = 1; i < bestDrivingRoute.size()-1; i++) {
        g.findVertex(bestDrivingRoute[i])->setAvailable(-1);
    }
    dijkstra(&g, source);
    std::vector<int> AlternativeDrivingRoute = getPath(&g, source, destination);
    int cost2 = getCost(&g, destination);
    std::cout<<"Source: " <<source<<endl;
    std::cout<<"Destination: " <<destination<<endl;
    std::cout<<"BestDrivingRoute: ";
    if (bestDrivingRoute.empty()) {
        std::cout << "None";
    } else {
        for (size_t i = 0; i < bestDrivingRoute.size(); ++i) {
            if (i != bestDrivingRoute.size() - 1) {
                std::cout << bestDrivingRoute[i] << ",";
            } else {
                std::cout << bestDrivingRoute[i] << "(" << cost1 <<")";
            }
        }
    }
    std::cout<<endl<<"AlternativeDrivingRoute: ";
    if (AlternativeDrivingRoute.empty()) {
        std::cout << "None";
    } else {
        for (size_t i = 0; i < AlternativeDrivingRoute.size(); ++i) {
            if (i != AlternativeDrivingRoute.size() - 1) {
                std::cout << AlternativeDrivingRoute[i] << ",";
            } else {
                std::cout << AlternativeDrivingRoute[i] << "(" << cost2 <<")";
            }
        }
    }
    std::cout<<endl;
}

void ModeDrivingRestrictions(Graph<int> &g, int source, int destination) {
    if (!enablepreconfig) {
        std::cin.ignore();
        std::cout<<"AvoidNodes: "; avoidNodesLine(g);
        std::cout<<"AvoidSegments: "; avoidSegmentLine(g);
        std::cout<<"IncludeNode: "; includeNode(g);
    }

    std::vector<int> RestrictedDrivingRoute;
    int cost1;

    if (g.includenodevar != -1) {
        dijkstra(&g, g.includenodevar);
        std::vector<int> aux = getPath(&g, g.includenodevar, destination);
        cost1 =getCost(&g, destination);
        dijkstra(&g, source);
        RestrictedDrivingRoute = getPath(&g, source, g.includenodevar);
        cost1 += getCost(&g, g.includenodevar);
        RestrictedDrivingRoute.insert(RestrictedDrivingRoute.end(), aux.begin()+1, aux.end());
    } else {
        dijkstra(&g, source);
        RestrictedDrivingRoute = getPath(&g, source, destination);
        cost1 =getCost(&g, destination);
    }

    std::cout<<"Source: " <<source<<endl;
    std::cout<<"Destination: " <<destination<<endl;
    std::cout<<"RestrictedDrivingRoute: ";
    if (RestrictedDrivingRoute.empty()) {
        std::cout << "None";
    } else {
        for (size_t i = 0; i < RestrictedDrivingRoute.size(); ++i) {
            if (i != RestrictedDrivingRoute.size() - 1) {
                std::cout << RestrictedDrivingRoute[i] << ",";
            } else {
                std::cout << RestrictedDrivingRoute[i] << "(" << cost1 <<")";
            }
        }
    }
    std::cout<<endl;
}

void ModeDrivingandWalking(Graph<int> &g, int source, int destination, int maxWalkTime) {

    //ensuring source and destination are not parking nodes
    if (g.findVertex(source)->getParking() || g.findVertex(destination)->getParking()) {
        std::cout << "Source or destination cannot be parking nodes." << std::endl;
        return;
    }

    //ensuring source and destination are not adjacent
    bool adjacent = false;
    for (auto edge : g.findVertex(source)->getAdj()) {
        if (edge->getDest()->getInfo() == destination) {
            adjacent = true;
            break;
        }
    }
    if (adjacent) {
        std::cout <<  "Source and destination cannot be adjacent nodes." << std::endl;
        return;
    }

    //finding all parking nodes
    std::vector<int> parkingNodes;
    for (auto vertex : g.getVertexSet()) {
        if (vertex->getParking()) {
            parkingNodes.push_back(vertex->getInfo());
        }
    }

    // to store the best route
    std::vector<int> bestDrivingRoute, bestWalkingRoute;
    int bestParkingNode = -1;
    int bestTotalTime = INF;
    int bestWalkingTime = 0;
    int bestDrivingTime = 0;

    //iterating over all the parking nodes
    for (int parkingNode : parkingNodes) {
        //compute driving route from source to parking node
        dijkstra(&g, source);
        std::vector<int> drivingRoute = getPath(&g, source, parkingNode);
        int drivingTime = getCost(&g, parkingNode);

        //compute walking route from parking node to destination
        dijkstra(&g, parkingNode);
        std::vector<int> walkingRoute = getPath(&g, parkingNode, destination);
        int walkingTime = getCost(&g, destination);

        //checking if walking time is within the limit
        if (walkingTime <= maxWalkTime) {
            int totalTime = drivingTime + walkingTime;

            //update best route if this one is better
            if (totalTime < bestTotalTime || (totalTime == bestTotalTime && walkingTime > bestWalkingTime)) {
                bestDrivingRoute = drivingRoute;
                bestWalkingRoute = walkingRoute;
                bestParkingNode = parkingNode;
                bestTotalTime = totalTime;
                bestDrivingTime = drivingTime;
                bestWalkingTime = walkingTime;
            }
        }
    }

    //output the best route
    if (bestParkingNode == -1) {
        std::cout << "Source: " << source << std::endl;
        std::cout << "Destination: " << destination << std::endl;
        std::cout << "DrivingRoute: " << std::endl;
        std::cout << "ParkingNode: " << std::endl;
        std::cout << "WalkingRoute: " << std::endl;
        std::cout << "Total Time: "  << std::endl;
        std::cout << "Message: No possible route with max. walking time of " << maxWalkTime << " minutes." << std::endl;
    } else {
        std::cout << "Source: " << source << std::endl;
        std::cout << "Destination: " << destination << std::endl;
        std::cout << "DrivingRoute: ";
        for (size_t i = 0; i < bestDrivingRoute.size(); ++i) {
            if ( i != bestDrivingRoute.size() - 1) {
                std::cout << bestDrivingRoute[i] << ",";
            } else {
                std::cout << bestDrivingRoute[i] << "(" << bestDrivingTime << ")" << std::endl;
            }
        }
        std::cout << "ParkingNode: " << bestParkingNode << std::endl;
        std::cout << "WalkingRoute: ";
        for (size_t i = 0; i < bestWalkingRoute.size(); ++i) {
            if (i != bestWalkingRoute.size() - 1) {
                std::cout << bestWalkingRoute[i] << ",";
            } else {
                std::cout << bestWalkingRoute[i] << "(" << bestWalkingTime << ")" << std::endl;
            }
        }
        std::cout << "TotalTime: " << bestTotalTime << std::endl;
    }
}