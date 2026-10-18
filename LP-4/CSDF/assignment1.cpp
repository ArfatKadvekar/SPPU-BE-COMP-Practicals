
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");

    if (start == string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string lower(string s) {
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });
    return s;
}

int main() {
    string filename;

    cout << "Enter email file name: ";
    getline(cin, filename);

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return 1;
    }

    map<string, string> headers;
    vector<string> received;
    string line, currentField;
    bool headerStarted = false;

    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty()) {
            if (!headerStarted)
                continue;
            break;
        }

        size_t pos = line.find(':');

        if ((line[0] == ' ' || line[0] == '\t')
            && !currentField.empty()) {
            string continuation = trim(line);

            if (currentField == "received" &&
                !received.empty()) {
                received.back() += " " + continuation;
            } else if (headers.count(currentField)) {
                headers[currentField] += " " + continuation;
            }
            continue;
        }

        if (pos == string::npos) {
            currentField = "";
            continue;
        }

        string key = lower(trim(line.substr(0, pos)));
        string value = trim(line.substr(pos + 1));

        if (key.empty()) {
            currentField = "";
            continue;
        }

        headerStarted = true;
        currentField = key;

        if (key == "received") {
            received.push_back(value);
        } else if (key == "from" || key == "return-path" ||
                   key == "to" || key == "subject" ||
                   key == "date" || key == "message-id" ||
                   key == "authentication-results" ||
                   key == "reply-to") {
            if (!headers.count(key))
                headers[key] = value;
        }
    }

    file.close();

    cout << "\n--- EMAIL HEADER ANALYSIS ---\n";

    string fields[] = {
        "from", "to", "subject", "date",
        "reply-to", "return-path", "message-id"
    };

    for (const string& field : fields) {
        cout << field << ": ";

        if (headers.count(field))
            cout << headers[field];
        else
            cout << "Not Found";

        cout << '\n';
    }

    cout << "\n--- EMAIL ROUTE ---\n";

    if (received.empty()) {
        cout << "No Received headers found.\n";
    } else {
        for (int i = static_cast<int>(received.size()) - 1;
             i >= 0; i--) {
            cout << received.size() - i
                 << ". " << received[i] << '\n';
        }
    }

    cout << "\n--- AUTHENTICATION RESULTS ---\n";

    if (headers.count("authentication-results"))
        cout << headers["authentication-results"] << '\n';
    else
        cout << "Not Found\n";

    cout << "\n--- SECURITY CHECK ---\n";

    if (headers.count("from") &&
        headers.count("return-path")) {
        if (lower(headers["from"]) !=
            lower(headers["return-path"])) {
            cout << "Warning: From and Return-Path differ.\n";
            cout << "Further investigation is required.\n";
        } else {
            cout << "From and Return-Path match.\n";
        }
    } else {
        cout << "Insufficient fields for comparison.\n";
    }

    cout << "\nAnalysis completed.\n";

    return 0;
}
