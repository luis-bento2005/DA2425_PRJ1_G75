#include <fstream>
#include <iostream>
#include <sstream>

#include "data_structures/createGraphs.h"
#include "data_structures/Graph.h"
#include "Modes/driving.h"

void CommandLine(Graph<int> &g);
void BatchModeLine(Graph<int> &g);
void BatchModeLineDriving(Graph<int> &g,std::ifstream &file);
void BatchModeLineDriving_Walking(Graph<int> &g,std::ifstream &file);
void ModeDriving(Graph<int> &g, int source, int destination);
void ModeDrivingOutput(Graph<int> &g, int source, int destination);
void ModeDrivingRestrictions(Graph<int> &g, int source, int destination);
void ModeDrivingRestrictionsOutput(Graph<int> &g, int source, int destination);
void ModeDrivingandWalking(Graph<int> &g, int source, int destination, int maxWalkTime);
void ModeDrivingandWalkingOutput(Graph<int> &g, int source, int destination, int maxWalkTime);
void avoidNodesLine(Graph<int> &g);
void avoidSegmentLine(Graph<int> &g);


bool enablepreconfig = false; //enable pre-configurations for the menu

/*
PRESS "q" ON THE CONSOLE TO ENABLE PREVIOUSLY DEFINED OPTIONS
*/

std::string presetmode = "driving-walking"; //chose wherever you want
int presetsource = 1; //chose wherever you want -----> ex: "2"
int presetdestination = 8; //chose wherever you want ----> ex "7"
std::string presetrestrictions = "N"; //chose between "Y" or "N"
std::string presetavoidNodes = ""; //chose wherever you want ----> ex "5,8"
std::string presetavoidSegment = ""; //chose wherever you want ----> ex "(1,2),(3,7)"
std::string presetIncludeNode = ""; //chose wherever you want ----> ex "4"
int presetmaxWalkTime = 30;

/*
PRESS "q" ON THE CONSOLE TO ENABLE PREVIOUSLY DEFINED OPTIONS
*/

int main() {
    string folder = "../../DA2425_PRJ1_G75/src/Main/CreateGraph";

    bool CML = true;
    while (CML) {
        Graph<int> g = createGraphs::graphFromFile(folder);
        string input;
        std::cout<< "If you want to Use Command Line press 'Y', if you want to Use Batch Mode Press 'T'" <<endl;
        std::cin >> input;
        if (input == "e") {
            enablepreconfig = true;
            CommandLine(g);
        } else if (input == "Y" or input == "y") {
            CommandLine(g);
        } else if (input == "T" or input == "t") {
            BatchModeLine(g);
        }

        else {
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
        enablepreconfig = false;
    } else {
        bool restrictions = false;
        std::cout << "Choose one of the Modes (Driving, Driving-walking)" << std::endl;
        std::string mode;
        std::cin >> mode;

        if (mode == "Driving" || mode == "driving") {
            std::cout << "If you want Restrictions press Y or N" << std::endl;
            std::string input;
            std::cin >> input;
            if (input == "Y" || input == "y") {restrictions = true;}
        }

        int source;
        int destination;
        std::cout << "Source: "; std::cin >> source;
        std::cout << "Destination: "; std::cin >> destination;


        if (mode == "Driving" || mode == "driving") {
            if (restrictions) {
                ModeDrivingRestrictions(g, source, destination);
            } else {
                ModeDriving(g, source, destination);
            }
        } else if (mode == "Driving-walking" || mode == "driving-walking") {
            int maxWalkTime;
            std::cout << "MaxWalkTime: "; std::cin >> maxWalkTime;
            ModeDrivingandWalking(g, source, destination, maxWalkTime);
        }
    }
}
void BatchModeLine(Graph<int> &g) {
    string filename = "../../DA2425_PRJ1_G75/src/Main/BatchMode/input.txt";
    ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    string Mode;
    getline(file, Mode);
    if (Mode == "Mode:Driving" || Mode == "Mode:driving") {
        BatchModeLineDriving(g,file);
    }
    else if (Mode == "Mode:driving-walking" || Mode == "Mode:Driving-walking") {
        BatchModeLineDriving_Walking(g,file);
    }

    file.close();
}

void BatchModeLineDriving(Graph<int> &g,std::ifstream &file) {
    int count = 0;
    string line;
    int source, destination;
    while (getline(file, line)) {
        istringstream iss(line);
        string paramenter;
        string value;
        switch (count) {
            case 0: {
                getline(iss,paramenter,':');
                getline(iss,value);
                source = stoi(value);
                break;
            }
            case 1: {
                getline(iss,paramenter,':');
                getline(iss,value);
                destination = stoi(value);
                break;
            }
            case 2: {
                getline(iss,paramenter,':');
                getline(iss,value);
                std::istringstream iss2(value);
                int Vertex;
                while (iss2 >> Vertex) {
                    g.findVertex(Vertex)->setAvailable(-1);
                }
                break;
            }
            case 3: {
                getline(iss,paramenter,':');
                getline(iss,value);
                std::istringstream iss3(value);
                char discard;

                while (iss3 >> discard && discard == '(') {
                    int source1, destination1;
                    char comma;

                    if (!(iss3 >> source1 >> comma >> destination1)) {
                        break;
                    }

                    iss3 >> discard;
                    if (discard != ')') {
                        break;
                    }
                    g.findVertex(source1)->removeEdge(destination1);
                    g.findVertex(destination1)->removeEdge(source1);

                    if (iss3.peek() == ',') {
                        iss3.ignore();
                    }
                }
                break;
            }
            case 4: {
                getline(iss,paramenter,':');
                getline(iss,value);
                std::istringstream iss4(value);
                int Vertex_Node;
                while (iss4 >> Vertex_Node) {
                    g.findVertex(Vertex_Node)->setAvailable(1);
                    g.includenodevar = Vertex_Node;
                }
                break;
            }
            default: {
                break;
            }
        }
        count++;
    }

    if (count == 2) {
        ModeDrivingOutput(g, source, destination);
    }
    else {
        ModeDrivingRestrictionsOutput(g, source, destination);
    }
}
void BatchModeLineDriving_Walking(Graph<int> &g,std::ifstream &file) {
    string line;
    int count = 0;
    int source, destination, max_walk_time;
    while (getline(file, line)) {
        istringstream iss(line);
        string paramenter;
        string value;
        switch (count) {
            case 0: {
                getline(iss,paramenter,':');
                getline(iss,value);
                source = stoi(value);
                break;
            }
            case 1: {
                getline(iss,paramenter,':');
                getline(iss,value);
                destination = stoi(value);
                break;
            }
            case 2: {
                getline(iss,paramenter,':');
                getline(iss,value);
                max_walk_time = stoi(value);
                break;
            }
            case 3: {
                getline(iss,paramenter,':');
                getline(iss,value);
                std::istringstream iss1(value);
                char discard;

                while (iss1 >> discard && discard == '(') {
                    int source1, destination1;
                    char comma;

                    if (!(iss1 >> source1 >> comma >> destination1)) {
                        break;
                    }

                    iss1 >> discard;
                    if (discard != ')') {
                        break;
                    }
                    g.findVertex(source1)->removeEdge(destination1);
                    g.findVertex(destination1)->removeEdge(source1);

                    if (iss1.peek() == ',') {
                        iss1.ignore();
                    }
                }
                break;
            }
            case 4: {
                getline(iss,paramenter,':');
                getline(iss,value);
                std::istringstream iss2(value);
                int Vertex_Node;
                while (iss2 >> Vertex_Node) {
                    g.findVertex(Vertex_Node)->setAvailable(1);
                    g.includenodevar = Vertex_Node;
                }
                break;
            }
            default: {
                break;
            }
        }
        count++;
    }

    ModeDrivingandWalkingOutput(g,source,destination,max_walk_time);
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

void ModeDrivingOutput(Graph<int> &g, int source, int destination) {
    dijkstra(&g, source);
    std::vector<int> bestDrivingRoute = getPath(&g, source, destination);
    int cost1 = getCost(&g, destination);
    for (int i = 1; i < bestDrivingRoute.size()-1; i++) {
        g.findVertex(bestDrivingRoute[i])->setAvailable(-1);
    }
    dijkstra(&g, source);
    std::vector<int> AlternativeDrivingRoute = getPath(&g, source, destination);
    int cost2 = getCost(&g, destination);

    std::ofstream outputFile("../../DA2425_PRJ1_G75/src/Main/BatchMode/Output.txt");
    if (!outputFile) {  // Check if the file opened successfully
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    outputFile<<"Source: "<<source<<endl;
    outputFile<<"Destination: "<<destination<<endl;
    outputFile<<"BestDrivingRoute: ";
    if (bestDrivingRoute.empty()) {
        outputFile << "None";
    } else {
        for (size_t i = 0; i < bestDrivingRoute.size(); ++i) {
            if (i != bestDrivingRoute.size() - 1) {
                outputFile<< bestDrivingRoute[i] << ",";
            } else {
                outputFile << bestDrivingRoute[i] << "(" << cost1 <<")";
            }
        }
    }
    outputFile<<endl<<"AlternativeDrivingRoute: ";
    if (AlternativeDrivingRoute.empty()) {
        outputFile << "None";
    } else {
        for (size_t i = 0; i < AlternativeDrivingRoute.size(); ++i) {
            if (i != AlternativeDrivingRoute.size() - 1) {
                outputFile << AlternativeDrivingRoute[i] << ",";
            } else {
                outputFile << AlternativeDrivingRoute[i] << "(" << cost2 <<")";
            }
        }
    }
    outputFile.close();
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

void ModeDrivingRestrictionsOutput(Graph<int> &g, int source, int destination) {

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
    std::ofstream outputFile("../../DA2425_PRJ1_G75/src/Main/BatchMode/Output.txt");
    if (!outputFile) {  // Check if the file opened successfully
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }
    outputFile<<"Source: " <<source<<endl;
    outputFile<<"Destination: " <<destination<<endl;
    outputFile<<"RestrictedDrivingRoute: ";
    if (RestrictedDrivingRoute.empty()) {
        outputFile << "None";
    } else {
        for (size_t i = 0; i < RestrictedDrivingRoute.size(); ++i) {
            if (i != RestrictedDrivingRoute.size() - 1) {
                outputFile << RestrictedDrivingRoute[i] << ",";
            } else {
                outputFile << RestrictedDrivingRoute[i] << "(" << cost1 <<")";
            }
        }
    }
    outputFile<<endl;
}

void ModeDrivingandWalking(Graph<int> &g, int source, int destination, int maxWalkTime) {
    if (!enablepreconfig) {
        std::cin.ignore();
        std::cout<<"AvoidNodes: "; avoidNodesLine(g);
        std::cout<<"AvoidSegments: "; avoidSegmentLine(g);
    }

    //ensuring source and destination are not parking nodes
    if (g.findVertex(source)->getParking() || g.findVertex(destination)->getParking()) {
        std::cout << "Source or destination cannot be parking nodes." << std::endl;
        return;
    }

    //ensuring source and destination are not adjacent
    for (auto edge : g.findVertex(source)->getAdj()) {
        if (edge->getDest()->getInfo() == destination) {
            std::cout <<  "Source and destination cannot be adjacent nodes." << std::endl;
            return;
        }
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
        g.switchwalking = true;
        dijkstra(&g, parkingNode);
        g.switchwalking = false;
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
    std::cout << "Source: " << source << std::endl;
    std::cout << "Destination: " << destination << std::endl;
    if (bestParkingNode == -1) {
        std::cout << "DrivingRoute: " << std::endl;
        std::cout << "ParkingNode: " << std::endl;
        std::cout << "WalkingRoute: " << std::endl;
        std::cout << "Total Time: "  << std::endl;
        std::cout << "Message: No possible route with max. walking time of " << maxWalkTime << " minutes." << std::endl;
    } else {
        if (bestDrivingRoute.empty() || bestWalkingRoute.empty()) {
            std::cout << "DrivingRoute:none" << std::endl;
            std::cout << "ParkingNode:none" << std::endl;
            std::cout << "WalkingRoute:none" << std::endl;
        } else {
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
}

void ModeDrivingandWalkingOutput(Graph<int> &g, int source, int destination, int maxWalkTime) {
    std::ofstream outputFile("../../DA2425_PRJ1_G75/src/Main/BatchMode/Output.txt");
    if (!outputFile) {  // Check if the file opened successfully
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }
    //ensuring source and destination are not parking nodes
    if (g.findVertex(source)->getParking() || g.findVertex(destination)->getParking()) {
        outputFile << "Source or destination cannot be parking nodes." << std::endl;
        return;
    }

    //ensuring source and destination are not adjacent
    for (auto edge : g.findVertex(source)->getAdj()) {
        if (edge->getDest()->getInfo() == destination) {
            outputFile <<  "Source and destination cannot be adjacent nodes." << std::endl;
            return;
        }
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
        g.switchwalking = true;
        dijkstra(&g, parkingNode);
        g.switchwalking = false;
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
    outputFile << "Source: " << source << std::endl;
    outputFile << "Destination: " << destination << std::endl;
    if (bestParkingNode == -1) {
        outputFile << "DrivingRoute: " << std::endl;
        outputFile << "ParkingNode: " << std::endl;
        outputFile << "WalkingRoute: " << std::endl;
        outputFile << "Total Time: "  << std::endl;
        outputFile << "Message: No possible route with max. walking time of " << maxWalkTime << " minutes." << std::endl;
    } else {
        if (bestDrivingRoute.empty() || bestWalkingRoute.empty()) {
            outputFile << "DrivingRoute:none" << std::endl;
            outputFile << "ParkingNode:none" << std::endl;
            outputFile << "WalkingRoute:none" << std::endl;
        } else {
            outputFile << "DrivingRoute: ";
            for (size_t i = 0; i < bestDrivingRoute.size(); ++i) {
                if ( i != bestDrivingRoute.size() - 1) {
                    outputFile << bestDrivingRoute[i] << ",";
                } else {
                    outputFile << bestDrivingRoute[i] << "(" << bestDrivingTime << ")" << std::endl;
                }
            }
            outputFile << "ParkingNode: " << bestParkingNode << std::endl;
            outputFile << "WalkingRoute: ";
            for (size_t i = 0; i < bestWalkingRoute.size(); ++i) {
                if (i != bestWalkingRoute.size() - 1) {
                    outputFile << bestWalkingRoute[i] << ",";
                } else {
                    outputFile << bestWalkingRoute[i] << "(" << bestWalkingTime << ")" << std::endl;
                }
            }
            outputFile << "TotalTime: " << bestTotalTime << std::endl;
        }
    }
    outputFile.close();
}