#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    int numNodes, numEdges;
    cout << "Enter the number of web pages (nodes): ";
    cin >> numNodes;
    
    if (numNodes <= 0) {
        cout << "Error: Number of nodes must be greater than 0." << endl;
        return 1;
    }

    cout << "Enter the number of links (edges): ";
    cin >> numEdges;

    // Adjacency list representation of the web graph
    // webGraph[i] contains a list of nodes that page i links TO
    vector<vector<int>> webGraph(numNodes);
    vector<int> outDegree(numNodes, 0);

    cout << "Enter the directed links (source destination) separated by space (0-indexed):" << endl;
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        cin >> u >> v;
        if (u >= 0 && u < numNodes && v >= 0 && v < numNodes) {
            webGraph[u].push_back(v);
            outDegree[u]++;
        } else {
            cout << "Warning: Invalid edge ignored." << endl;
        }
    }

    // Initialize PageRank values to 1/N
    vector<double> pageRank(numNodes, 1.0 / numNodes);
    vector<double> newPageRank(numNodes, 0.0);
    double dampingFactor = 0.85;
    int iterations = 10; // Fixed number of iterations for stability

    cout << "\n--- Calculating PageRank ---" << endl;
    for (int iter = 1; iter <= iterations; ++iter) {
        // Reset newPageRank to the base random-jump value: (1 - d) / N
        double baseRank = (1.0 - dampingFactor) / numNodes;
        for (int i = 0; i < numNodes; ++i) {
            newPageRank[i] = baseRank;
        }

        // Distribute PageRank to outbound links
        for (int u = 0; u < numNodes; ++u) {
            if (outDegree[u] > 0) {
                double share = pageRank[u] / outDegree[u];
                for (int v : webGraph[u]) {
                    newPageRank[v] += dampingFactor * share;
                }
            } else {
                // Handling dangling nodes (pages with no outbound links)
                // Distribute their rank evenly across all nodes to prevent rank sinks
                double share = pageRank[u] / numNodes;
                for (int v = 0; v < numNodes; ++v) {
                    newPageRank[v] += dampingFactor * share;
                }
            }
        }

        // Update the ranks for the next iteration
        pageRank = newPageRank;
    }

    // Output the final PageRank scores
    cout << fixed << setprecision(4);
    cout << "\n--- Final PageRank Scores (after " << iterations << " iterations) ---" << endl;
    for (int i = 0; i < numNodes; ++i) {
        cout << "Page " << i << ": " << pageRank[i] << endl;
    }

    return 0;
}