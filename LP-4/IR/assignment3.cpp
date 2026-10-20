#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>

using namespace std;

// Map function: Emits a list of intermediate key-value pairs (char, 1)
vector<pair<char, int>> mapper(const string& text) {
    vector<pair<char, int>> intermediate;
    for (char c : text) {
        if (isalpha(c)) {
            // Case-insensitive counting: convert to lowercase
            intermediate.push_back({tolower(c), 1});
        }
    }
    return intermediate;
}

// Reduce function: Sums the counts for a specific character key
int reducer(char key, const vector<int>& values) {
    int sum = 0;
    for (int v : values) {
        sum += v;
    }
    return sum;
}

int main() {
    string filename;
    cout << "Enter the path to the text dataset: ";
    cin >> filename;

    // File Input handling
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open dataset file: " << filename << endl;
        return 1;
    }

    string word;
    vector<pair<char, int>> allIntermediateData;

    cout << "\n--- Initiating MapReduce Pipeline ---" << endl;
    cout << "[*] Executing Map Phase..." << endl;
    
    // 1. Map Phase
    while (file >> word) {
        vector<pair<char, int>> mappedData = mapper(word);
        // Append emitted pairs to the master intermediate list
        allIntermediateData.insert(allIntermediateData.end(), mappedData.begin(), mappedData.end());
    }
    file.close();

    cout << "[*] Executing Shuffle & Sort Phase..." << endl;
    
    // 2. Shuffle and Sort Phase
    // Grouping all values (1s) associated with the same character key
    map<char, vector<int>> groupedData;
    for (const auto& pair : allIntermediateData) {
        groupedData[pair.first].push_back(pair.second);
    }

    cout << "[*] Executing Reduce Phase..." << endl;
    
    // 3. Reduce Phase
    map<char, int> finalOutput;
    for (const auto& pair : groupedData) {
        finalOutput[pair.first] = reducer(pair.first, pair.second);
    }

    // Report Generation
    cout << "\n--- Final Character Frequencies ---" << endl;
    if (finalOutput.empty()) {
        cout << "No alphabetic characters found in the dataset." << endl;
    } else {
        for (const auto& pair : finalOutput) {
            cout << pair.first << " : " << pair.second << endl;
        }
    }

    return 0;
}