#include <iostream>
#include <sstream>

#include "data_structures/createGraphs.h"
#include "data_structures/Graph.h"
#include "Modes/driving.h"

void CommandLine(Graph<int> &g);
void ModeDriving(Graph<int> &g, int source, int destination);
void ModeDrivingRestrictions(Graph<int> &g, int source, int destination);
void avoidNodesLine(Graph<int> &g);
void avoidSegmentLine(Graph<int> &g);

int main() {
    string folder = "../../DA2425_PRJ1_G75/src/Main/For_Students";

    bool CML = true;
    while (CML) {
        Graph<int> g = createGraphs::graphFromFile(folder);
        string input;
        std::cout<< "If you want to Use Command Line press 'Y'" <<endl;
        std::cin >> input;
        if (input == "Y" or input == "y") {
            CommandLine(g);
        }else {
            CML = false;
        }
    }

    return 0;
}

void CommandLine(Graph<int> &g) {
    std::cout<<"If you want Restrictions press Y or N"<<endl;
    std::string input;
    std::cin >> input;
    std::cout << "Chose one of the Modes(Driving, Driving-walking)" <<endl;

    std::string mode;
    int source;
    int destination;
    string restrictions;
    std::cout<<"Mode: "; std::cin>>mode;
    std::cout<<"Source: "; std::cin>>source;
    std::cout<<"Destination: "; std::cin>>destination;

    if (input == "Y" or input == "y") {
        if (mode == "Driving" or mode == "driving") {
            ModeDrivingRestrictions(g, source, destination);
        }
    }
    if (input == "N" or input == "n") {
        if (mode == "Driving" or mode == "driving") {
            ModeDriving(g, source, destination);
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

void IncludeNode(Graph<int> &g) {
    std::string IncludeNode;
    std::getline(std::cin, IncludeNode);
    std::istringstream iss(IncludeNode);
    int Vertex;
    while (iss >> Vertex) {
        g.findVertex(Vertex)->setAvailable(1);
    }
}

void ModeDriving(Graph<int> &g, int source, int destination) {
    dijkstra(&g, source);
    std::vector<int> bestDrivingRoute = getPath(&g, source, destination);
    int cost1 =getCost(&g, destination);
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
    std::cin.ignore();
    std::cout<<"AvoidNodes: "; avoidNodesLine(g);
    std::cout<<"AvoidSegments: "; avoidSegmentLine(g);
    std::cout<<"IncludeNode: "; avoidNodesLine(g);

    dijkstra(&g, source);
    std::vector<int> RestrictedDrivingRoute = getPath(&g, source, destination);
    int cost1 =getCost(&g, destination);
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