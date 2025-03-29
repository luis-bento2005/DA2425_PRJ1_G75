#include <fstream>
/**
 * @file main.cpp
 * @brief Main implementation file for the route planning system
 *
 * @details This file implements the command-line interface and core functionality
 * for the route planning system, including driving, restricted driving, and
 * combined driving-walking modes.
 */

#include <climits>
#include <iostream>
#include <sstream>
#include <stdint.h>

#include "data_structures/createGraphs.h"
#include "data_structures/Graph.h"
#include "Modes/driving.h"

void CommandLine(Graph<int> &g);
void BatchModeLine();
void processModeBlock(const vector<string>& blockLines, const string& folder, std::ofstream& outputFile);
void processDrivingBlock(Graph<int>& g, const vector<string>& blockLines, std::ofstream& outputFile);
void processDrivingWalkingBlock(Graph<int>& g, const vector<string>& blockLines, std::ofstream& outputFile);
void ModeDriving(Graph<int> &g, int source, int destination, std::ofstream& outputFile);
void ModeDrivingRestrictions(Graph<int> &g, int source, int destination, std::ofstream& outputFile);
void ModeDrivingandWalking(Graph<int> &g, int source, int destination, int maxWalkTime, std::ofstream& outputFile);
void avoidNodesLine(Graph<int> &g);
void avoidSegmentLine(Graph<int> &g);
void printOutput();


/**
 * @brief Global flag to enable Restrictions scenarios
 */
bool enableRestrictions = false;

/**
 * @brief Structure to store approximate solutions when no perfect route is found
 */
typedef struct {
    std::vector<int> DrivingRoute;
    int ParkingNode = -1;
    std::vector<int> WalkingRoute;
    int drivingtime = INT_MAX;
    int walkingtime = INT_MAX;
    int totaltime = INT_MAX;
} ApproximateSolution;
ApproximateSolution approximatesolution1, approximatesolution2;


/**
 * @brief Main program entry point
 * @return Exit status (0 for success)
 *
 * @details Initializes the graph and handles the main command loop.
 * The program loads graph data from files and presents a command-line interface
 * for route planning operations.
 */
int main() {

    bool CML = true;
    while (CML) {
        string input;
        std::cout<< "If you want to Use Command Line press 'Y', if you want to Use Batch Mode Press 'T'" <<endl;
        std::cin >> input;
        if (input == "Y" or input == "y") {
            string folder = "../../DA2425_PRJ1_G75/src/Main/CreateGraph";
            Graph<int> g = createGraphs::graphFromFile(folder);
            CommandLine(g);
        } else if (input == "T" or input == "t") {
            BatchModeLine();
        }
        else {
            CML = false;
        }
        enableRestrictions = false;
    }

    return 0;
}

/**
 * @brief Handles the command-line interface for route planning
 * @param g Reference to the graph object
 *
 * @details Provides interactive menu for:
 * - Mode selection (driving/driving-walking)
 * - Source/destination input
 * - Route restrictions
 * - Pre-configured test scenarios
 */
void CommandLine(Graph<int> &g) {
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

    std::ofstream outputFile("../../DA2425_PRJ1_G75/src/Main/BatchMode/output.txt");
    if (!outputFile) {  // Check if the file opened successfully
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    if (mode == "Driving" || mode == "driving") {
        if (restrictions) {
            enableRestrictions = true;
            ModeDrivingRestrictions(g, source, destination, outputFile);
        } else {
            ModeDriving(g, source, destination, outputFile);
        }
    } else if (mode == "Driving-walking" || mode == "driving-walking") {
        int maxWalkTime;
        enableRestrictions = true;
        std::cout << "MaxWalkTime: "; std::cin >> maxWalkTime;
        ModeDrivingandWalking(g, source, destination, maxWalkTime, outputFile);
    }
    printOutput();

}

/**
 * @brief Prints the output from output.txt.
 */
void printOutput() {
    string filename = "../../DA2425_PRJ1_G75/src/Main/BatchMode/output.txt";
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return ;
    }
    string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}

/**
 * @brief Reads input file, processes the file in blocks, and writes an output file.
 */
void BatchModeLine() {
    string folder = "../../DA2425_PRJ1_G75/src/Main/CreateGraph";
    string filename = "../../DA2425_PRJ1_G75/src/Main/BatchMode/input.txt";

    std::ofstream outputFile("../../DA2425_PRJ1_G75/src/Main/BatchMode/output.txt");
    if (!outputFile) {  // Check if the file opened successfully
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    vector<string> currentBlock;
    string line;
    while (getline(file, line)) {
        if (line.find("Mode:") == 0) {
            if (!currentBlock.empty()) {
                processModeBlock(currentBlock, folder, outputFile);
                currentBlock.clear();
            }
        }
        currentBlock.push_back(line);
    }
    if (!currentBlock.empty()) {
        processModeBlock(currentBlock, folder, outputFile);
    }
    file.close();
    outputFile.close();
}

/**
 * @brief Processes a block of input lines corresponding to a specific mode.
 * @param blockLines Vector containing lines of mode-related commands.
 * @param folder Path to graph data folder.
 * @param outputFile Output stream to write results.
 */
void processModeBlock(const vector<string>& blockLines, const string& folder, std::ofstream& outputFile) {
    if (blockLines.empty()) return;

    string modeLine = blockLines[0];
    std::istringstream iss(modeLine);
    string text;
    string mode;
    std::getline(iss, text,':');
    getline(iss, mode);

    Graph<int> g = createGraphs::graphFromFile(folder);

    if (mode == "driving" || mode == "Driving") {
        processDrivingBlock(g, blockLines, outputFile);
    } else if (mode == "driving-walking" || mode == "Driving-walking") {
        processDrivingWalkingBlock(g, blockLines, outputFile);
    }
}

/**
 * @brief Processes driving mode input.
 * @param g Graph object representing the map.
 * @param blockLines Vector containing driving-related commands.
 * @param outputFile Output stream to write results.
 */
void processDrivingBlock(Graph<int>& g, const vector<string>& blockLines, std::ofstream& outputFile) {
    int source = -1, destination = -1;
    int IncludeNode = -1, AvoidNode = -1, AvoidSegment = -1;

    for (size_t i = 1; i < blockLines.size(); ++i) {
        string line = blockLines[i];
        if (line.find("Source:") == 0) {
            source = stoi(line.substr(7));
        } else if (line.find("Destination:") == 0) {
            destination = stoi(line.substr(12));
        } else if (line.find("AvoidNodes:") == 0) {
            std::string avoidNodes;
            std::istringstream iss(line);
            getline(iss, avoidNodes, ':');
            int Vertex;
            while (iss >> Vertex) {
                g.findVertex(Vertex)->setAvailable(-1);
            }
            AvoidNode = 0;
        } else if (line.find("AvoidSegments:") == 0) {
            std::istringstream iss(line);
            std::string avoidSegment;
            getline(iss,avoidSegment,':');

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
            AvoidSegment = 0;
        } else if (line.find("IncludeNode:") == 0) {
            std::string includeNode;
            std::istringstream iss(line);
            getline(iss, includeNode, ':');
            int Vertex;
            while (iss >> Vertex) {
                g.findVertex(Vertex)->setAvailable(1);
                g.includenodevar = Vertex;
            }
            IncludeNode = 0;
        }
    }

    if (source == -1 || destination == -1) {
        cerr << "Missing Source/Destination in Driving block." << endl;
        return;
    }

    if (AvoidNode == 0 || AvoidSegment == 0 || IncludeNode == 0) {
        ModeDrivingRestrictions(g, source, destination, outputFile);
    } else {
        ModeDriving(g, source, destination, outputFile);
    }
}

/**
 * @brief Processes driving-walking mode input.
 * @param g Graph object representing the map.
 * @param blockLines Vector containing driving-walking-related commands.
 * @param outputFile Output stream to write results.
 */
void processDrivingWalkingBlock(Graph<int>& g, const vector<string>& blockLines, std::ofstream& outputFile) {
    int source = -1, destination = -1, maxWalkTime = -1;
    vector<int> avoidNodes;
    vector<pair<int, int>> avoidSegments;

    for (size_t i = 1; i < blockLines.size(); ++i) {
        string line = blockLines[i];
        if (line.find("Source:") == 0) {
            source = stoi(line.substr(7));
        } else if (line.find("Destination:") == 0) {
            destination = stoi(line.substr(12));
        } else if (line.find("MaxWalkTime:") == 0) {
            maxWalkTime = stoi(line.substr(12));
        } else if (line.find("AvoidNodes:") == 0) {
            string nodes = line.substr(11);
            istringstream iss(nodes);
            int node;
            while (iss >> node) {
                avoidNodes.push_back(node);
                if (auto v = g.findVertex(node)) v->setAvailable(-1);
            }
        } else if (line.find("AvoidSegments:") == 0) {
            string segments = line.substr(13);
            istringstream iss(segments);
            char c;
            int src, dest;
            while (iss >> c && c == '(') {
                if (iss >> src >> c >> dest) {
                    avoidSegments.emplace_back(src, dest);
                    if (auto v = g.findVertex(src)) v->removeEdge(dest);
                    if (auto v = g.findVertex(dest)) v->removeEdge(src);
                }
                while (iss >> c && c != ')');
            }
        }
    }

    if (source == -1 || destination == -1 || maxWalkTime == -1) {
        cerr << "Missing parameters in Driving-Walking block." << endl;
        return;
    }

    ModeDrivingandWalking(g, source, destination, maxWalkTime, outputFile);
}

/**
 * @brief Processes nodes to avoid from user input
 * @param g Reference to the graph object
 */
void avoidNodesLine(Graph<int> &g) {
    std::string avoidNodes;
    std::getline(std::cin, avoidNodes);
    std::istringstream iss(avoidNodes);
    int Vertex;
    while (iss >> Vertex) {
        g.findVertex(Vertex)->setAvailable(-1);
    }

}

/**
 * @brief Processes segments to avoid from user input
 * @param g Reference to the graph object
 */
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

/**
 * @brief Processes nodes to include in route from user input
 * @param g Reference to the graph object
 */

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

/**
 * @brief Finds the fastest driving route between two nodes
 * @param g Reference to the graph object
 * @param source Starting node ID
 * @param destination Target node ID
 *
 * @details Calculates both the optimal route and an alternative independent route.
 * Outputs the routes with their respective travel times.
 */
void ModeDriving(Graph<int> &g, int source, int destination, std::ofstream& outputFile) {
    dijkstra(&g, source);
    std::vector<int> bestDrivingRoute = getPath(&g, source, destination);
    int cost1 = getCost(&g, destination);
    for (int i = 1; i < bestDrivingRoute.size()-1; i++) {
        g.findVertex(bestDrivingRoute[i])->setAvailable(-1);
    }
    dijkstra(&g, source);
    std::vector<int> AlternativeDrivingRoute = getPath(&g, source, destination);
    int cost2 = getCost(&g, destination);

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
    outputFile<<endl<<endl;
}

/**
 * @brief Finds driving route with restrictions
 * @param g Reference to the graph object
 * @param source Starting node ID
 * @param destination Target node ID
 *
 * @details Handles:
 * - Avoided nodes
 * - Avoided segments
 * - Required nodes to include
 */
void ModeDrivingRestrictions(Graph<int> &g, int source, int destination, std::ofstream& outputFile) {
    if (enableRestrictions) {
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
    outputFile<<endl<<endl;
}

/**
 * @brief Finds optimal combined driving-walking route
 * @param g Reference to the graph object
 * @param source Starting node ID
 * @param destination Target node ID
 * @param maxWalkTime Maximum allowed walking time in minutes
 *
 * @details Finds route that:
 * 1. Starts with driving
 * 2. Includes parking at a node
 * 3. Finishes with walking
 * 4. Respects max walking time
 *
 * If no perfect route is found, provides up to two approximate solutions.
 */

void ModeDrivingandWalking(Graph<int> &g, int source, int destination, int maxWalkTime, std::ofstream& outputFile) {
    if (enableRestrictions) {
        std::cin.ignore();
        std::cout<<"AvoidNodes: "; avoidNodesLine(g);
        std::cout<<"AvoidSegments: "; avoidSegmentLine(g);
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
        int totalTime = drivingTime + walkingTime;

        if (walkingTime <= maxWalkTime) {
            //update best route if this one is better
            if (totalTime < bestTotalTime || (totalTime == bestTotalTime && walkingTime > bestWalkingTime)) {
                bestDrivingRoute = drivingRoute;
                bestWalkingRoute = walkingRoute;
                bestParkingNode = parkingNode;
                bestTotalTime = totalTime;
                bestDrivingTime = drivingTime;
                bestWalkingTime = walkingTime;
            }
        } else {
            if (walkingTime < approximatesolution1.walkingtime) {
                if (approximatesolution2.totaltime == INT_MAX || approximatesolution1.totaltime < approximatesolution2.totaltime) {
                    approximatesolution2.DrivingRoute = approximatesolution1.DrivingRoute;
                    approximatesolution2.ParkingNode = approximatesolution1.ParkingNode;
                    approximatesolution2.WalkingRoute = approximatesolution1.WalkingRoute;
                    approximatesolution2.walkingtime = approximatesolution1.walkingtime;
                    approximatesolution2.drivingtime = approximatesolution1.drivingtime;
                }
                approximatesolution1.DrivingRoute = drivingRoute;
                approximatesolution1.ParkingNode = parkingNode;
                approximatesolution1.WalkingRoute = walkingRoute;
                approximatesolution1.walkingtime = walkingTime;
                approximatesolution1.drivingtime = drivingTime;
            } else if (walkingTime < approximatesolution2.walkingtime) {
                approximatesolution2.DrivingRoute = drivingRoute;
                approximatesolution2.ParkingNode = parkingNode;
                approximatesolution2.WalkingRoute = walkingRoute;
                approximatesolution2.walkingtime = walkingTime;
                approximatesolution2.drivingtime = drivingTime;
            }
        }
    }

    //output the best route
    outputFile << "Source: " << source << std::endl;
    outputFile << "Destination: " << destination << std::endl;
    if (bestParkingNode == -1) {
        outputFile << "Message: No possible route with max. walking time of " << maxWalkTime << " minutes." << std::endl;
        if (approximatesolution1.walkingtime != -1) {
                outputFile << "DrivingRoute1: ";
                for (size_t i = 0; i < approximatesolution1.DrivingRoute.size(); ++i) {
                    if (i != approximatesolution1.DrivingRoute.size() - 1) {
                        outputFile << approximatesolution1.DrivingRoute[i] << ",";
                    } else {
                        outputFile << approximatesolution1.DrivingRoute[i] << "(" << approximatesolution1.drivingtime << ")" << std::endl;
                    }
                }
                outputFile << "ParkingNode1: " << approximatesolution1.ParkingNode << std::endl;
                outputFile << "WalkingRoute1: ";
                for (size_t i = 0; i < approximatesolution1.WalkingRoute.size(); ++i) {
                    if (i != approximatesolution1.WalkingRoute.size() - 1) {
                        outputFile << approximatesolution1.WalkingRoute[i] << ",";
                    } else {
                        outputFile << approximatesolution1.WalkingRoute[i] << "(" << approximatesolution1.walkingtime << ")" << std::endl;
                    }
                }
                outputFile << "TotalTime1: " << (approximatesolution1.walkingtime + approximatesolution1.drivingtime) << std::endl;

                if (approximatesolution2.walkingtime != -1) {
                    outputFile << "DrivingRoute2: ";
                    for (size_t i = 0; i < approximatesolution2.DrivingRoute.size(); ++i) {
                        if (i != approximatesolution2.DrivingRoute.size() - 1) {
                            outputFile << approximatesolution2.DrivingRoute[i] << ",";
                        } else {
                            outputFile << approximatesolution2.DrivingRoute[i] << "(" << approximatesolution2.drivingtime << ")" << std::endl;
                        }
                    }
                    outputFile << "ParkingNode2: " << approximatesolution2.ParkingNode << std::endl;
                    outputFile << "WalkingRoute2: ";
                    for (size_t i = 0; i < approximatesolution2.WalkingRoute.size(); ++i) {
                        if (i != approximatesolution2.WalkingRoute.size() - 1) {
                            outputFile << approximatesolution2.WalkingRoute[i] << ",";
                        } else {
                            outputFile << approximatesolution2.WalkingRoute[i] << "(" << approximatesolution2.walkingtime << ")" << std::endl;
                        }
                    }
                    outputFile << "TotalTime2: " << (approximatesolution2.walkingtime + approximatesolution2.drivingtime) << std::endl;
                }
            } else {
                outputFile << "DrivingRoute:none" << std::endl;
                outputFile << "ParkingNode:none" << std::endl;
                outputFile << "WalkingRoute:none" << std::endl;
            }
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
    outputFile<<endl;
}