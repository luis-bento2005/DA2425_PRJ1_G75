#include <iostream>
#include "data_structures/createGraphs.h"
#include "data_structures/Graph.h"
#include "Modes/driving.h"

void CommandLine(Graph<int> &g);
void ModeDriving(Graph<int> &g, int source, int destination);

int main() {
    string folder = "../../DA2425_PRJ1_G75/src/Main/For_Students";
    Graph<int> g = createGraphs::graphFromFile(folder);

    bool CML = true;
    while (CML) {
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
    std::string mode;
    int source;
    int destination;
    std::cout<<"Mode: ";
    std::cin>>mode;
    std::cout<<"Source: ";
    std::cin>>source;
    std::cout<<"Destination: ";
    std::cin>>destination;
    if (mode == "Driving") {
        ModeDriving(g, source, destination);
    }
}

void ModeDriving(Graph<int> &g, int source, int destination) {
    dijkstra(&g, source);
    std::vector<int> bestDrivingRoute = getPath(&g, source, destination);

    std::vector<int> AlternativeDrivingRoute = getPath(&g, source, destination);
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
                std::cout << bestDrivingRoute[i] << "(" << getCost(&g, destination) <<")";
            }
        }
    }
    std::cout<<endl;
}