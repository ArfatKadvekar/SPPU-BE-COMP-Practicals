#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include <vector>

using namespace std;

// Simulated network fetch: parses a local text/html file for href links
vector<string> extractLinks(const string& url) {
    vector<string> links;
    ifstream file(url);
    if (!file.is_open()) {
        return links; // Simulate a 404 Dead Link by returning no new links
    }

    string word;
    while (file >> word) {
        size_t pos = word.find("href=\"");
        if (pos != string::npos) {
            size_t start = pos + 6;
            size_t end = word.find("\"", start);
            if (end != string::npos) {
                links.push_back(word.substr(start, end - start));
            }
        }
    }
    return links;
}

int main() {
    string seedUrl;
    int maxPages;

    cout << "Enter the seed URL (local filename): ";
    cin >> seedUrl;
    cout << "Enter maximum number of pages to crawl: ";
    cin >> maxPages;

    queue<string> frontier;
    set<string> visited;

    // Initialize crawler state
    frontier.push(seedUrl);
    visited.insert(seedUrl);

    int pagesCrawled = 0;

    cout << "\n--- Web Crawler Started ---" << endl;

    // Breadth-First Search Traversal
    while (!frontier.empty() && pagesCrawled < maxPages) {
        string currentUrl = frontier.front();
        frontier.pop();

        cout << "[*] Crawling: " << currentUrl << endl;
        pagesCrawled++;

        // Extract all hyperlinks from the current page
        vector<string> newLinks = extractLinks(currentUrl);
        
        for (const string& link : newLinks) {
            // Check for cycles: only process unvisited URLs
            if (visited.find(link) == visited.end()) {
                cout << "    -> Found new link: " << link << endl;
                visited.insert(link);
                frontier.push(link);
            }
        }
    }

    cout << "--- Crawl Complete ---" << endl;
    cout << "Total unique pages crawled: " << pagesCrawled << endl;

    return 0;
}
