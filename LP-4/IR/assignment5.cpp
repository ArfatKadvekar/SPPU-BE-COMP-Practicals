#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <iomanip>

using namespace std;

int main() {
    string filename, targetTopic;
    cout << "Enter the path to the XML dataset: ";
    cin >> filename;
    cout << "Enter the target topic for PageRank computation (e.g., 'sports'): ";
    cin >> targetTopic;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open XML file." << endl;
        return 1;
    }

    int numNodes = 0;
    vector<vector<int>> webGraph;
    set<int> topicSet; // Stores nodes that belong to the target topic

    string line;
    int currentNode = -1;

    cout << "\n--- Parsing XML & Generating Web Graph ---" << endl;
    
    // Extremely simplified XML parsing simulation
    // Expects format: <page id="0" topic="sports"> ... <link to="1" /> ... </page>
    while (getline(file, line)) {
        if (line.find("<page") != string::npos) {
            numNodes++;
            webGraph.push_back(vector<int>());
            currentNode = numNodes - 1;

            if (line.find("topic=\"" + targetTopic + "\"") != string::npos) {
                topicSet.insert(currentNode);
            }
        } else if (line.find("<link to=\"") != string::npos) {
            size_t start = line.find("<link to=\"") + 10;
            size_t end = line.find("\"", start);
            if (end != string::npos && currentNode != -1) {
                int targetNode = stoi(line.substr(start, end - start));
                webGraph[currentNode].push_back(targetNode);
            }
        }
    }
    file.close();

    if (numNodes == 0) {
        cout << "Error: No pages found in the XML dataset." << endl;
        return 1;
    }

    cout << "[*] Graph Constructed: " << numNodes << " nodes." << endl;
    cout << "[*] Nodes matching target topic '" << targetTopic << "': " << topicSet.size() << endl;

    if (topicSet.empty()) {
        cout << "Warning: Topic not found. Defaulting to standard PageRank." << endl;
    }

    // Compute Topic-Specific PageRank (TSPR)
    vector<double> pageRank(numNodes, 1.0 / numNodes);
    vector<double> newPageRank(numNodes, 0.0);
    double dampingFactor = 0.85;
    int iterations = 10;

    for (int iter = 1; iter <= iterations; ++iter) {
        // Apply Biased Teleportation
        for (int i = 0; i < numNodes; ++i) {
            if (!topicSet.empty() && topicSet.count(i)) {
                newPageRank[i] = (1.0 - dampingFactor) / topicSet.size();
            } else if (topicSet.empty()) {
                newPageRank[i] = (1.0 - dampingFactor) / numNodes;
            } else {
                newPageRank[i] = 0.0;
            }
        }

        // Rank Distribution
        for (int u = 0; u < numNodes; ++u) {
            int outDegree = webGraph[u].size();
            if (outDegree > 0) {
                double share = pageRank[u] / outDegree;
                for (int v : webGraph[u]) {
                    newPageRank[v] += dampingFactor * share;
                }
            } else {
                // Resolving dangling nodes using biased random jumps
                double share = pageRank[u] / (topicSet.empty() ? numNodes : topicSet.size());
                if (topicSet.empty()) {
                    for (int v = 0; v < numNodes; ++v) newPageRank[v] += dampingFactor * share;
                } else {
                    for (int v : topicSet) newPageRank[v] += dampingFactor * share;
                }
            }
        }
        pageRank = newPageRank;
    }

    cout << fixed << setprecision(4);
    cout << "\n--- Final Topic-Specific PageRank Scores ---" << endl;
    for (int i = 0; i < numNodes; ++i) {
        cout << "Page " << i << " : " << pageRank[i];
        if (topicSet.count(i)) cout << " (Topic Match)";
        cout << endl;
    }

    return 0;
}