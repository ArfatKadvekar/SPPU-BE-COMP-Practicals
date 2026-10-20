
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <set>
#include <cctype>

using namespace std;

// Build word frequency map after removing punctuation and stop words
map<string, int> buildFrequencyMap(
    const string& filename,
    const set<string>& stopWords,
    set<string>& vocabulary) {

    map<string, int> freq;
    ifstream file(filename);

    if (!file.is_open()) {
        return freq;
    }

    string word;

    while (file >> word) {
        string cleanWord = "";

        // Convert to lowercase and remove punctuation
        for (unsigned char c : word) {
            if (isalpha(c)) {
                cleanWord += tolower(c);
            }
        }

        // Remove stop words and count remaining words
        if (!cleanWord.empty() &&
            stopWords.find(cleanWord) == stopWords.end()) {
            freq[cleanWord]++;
            vocabulary.insert(cleanWord);
        }
    }

    file.close();
    return freq;
}

int main() {
    string file1, file2;

    cout << "Enter the path for Document 1: ";
    cin >> file1;

    cout << "Enter the path for Document 2: ";
    cin >> file2;

    // Stop words to ignore
    set<string> stopWords = {
        "the", "is", "in", "and", "to",
        "a", "of", "for", "it", "with"
    };

    set<string> vocabulary;

    // Calculate word frequencies
    map<string, int> freq1 =
        buildFrequencyMap(file1, stopWords, vocabulary);

    map<string, int> freq2 =
        buildFrequencyMap(file2, stopWords, vocabulary);

    if (freq1.empty() || freq2.empty()) {
        cout << "Error: Unable to open a document "
             << "or no valid words were found." << endl;
        return 1;
    }

    double dotProduct = 0.0;
    double magnitude1 = 0.0;
    double magnitude2 = 0.0;

    // Calculate dot product and vector magnitudes
    for (const string& word : vocabulary) {
        int v1 = freq1.count(word) ? freq1.at(word) : 0;
        int v2 = freq2.count(word) ? freq2.at(word) : 0;

        dotProduct += v1 * v2;
        magnitude1 += v1 * v1;
        magnitude2 += v2 * v2;
    }

    magnitude1 = sqrt(magnitude1);
    magnitude2 = sqrt(magnitude2);

    double similarity = 0.0;

    if (magnitude1 > 0 && magnitude2 > 0) {
        similarity =
            dotProduct / (magnitude1 * magnitude2);
    }

    // Display cosine similarity
    cout << "\n--- Vector Space Model Analysis ---" << endl;
    cout << "Cosine Similarity Score: "
         << similarity << endl;

    // Display common words and their frequencies
    cout << "\n--- Common Words Vector ---" << endl;

    bool foundCommon = false;

    for (const string& word : vocabulary) {
        if (freq1.count(word) && freq2.count(word)) {
            cout << word
                 << " -> Document 1: " << freq1.at(word)
                 << ", Document 2: " << freq2.at(word)
                 << endl;

            foundCommon = true;
        }
    }

    if (!foundCommon) {
        cout << "No common words found." << endl;
    }

    // Interpret similarity score
    if (similarity == 0.0) {
        cout << "\n[*] No similarity between document vectors."
             << endl;
    } else if (similarity > 0.99) {
        cout << "\n[*] The document vectors are highly similar."
             << endl;
    } else {
        cout << "\n[*] The documents have partial similarity."
             << endl;
    }

    return 0;
}
