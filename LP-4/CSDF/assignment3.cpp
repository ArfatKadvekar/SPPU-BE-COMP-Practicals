
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <sstream>
using namespace std;

int main() {
    string filename, line;
    int threshold = 3;

    cout << "Enter authentication log file: ";
    getline(cin, filename);

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return 1;
    }

    map<string, int> failures;
    set<string> blocked;

    cout << "\n--- Wi-Fi Intrusion Detection ---\n";
    cout << "Failure threshold: " << threshold << "\n\n";

    while (getline(file, line)) {
        if (line.empty())
            continue;

        string timestamp, mac, status;
        istringstream input(line);

        if (!(input >> timestamp >> mac >> status)) {
            cout << "Invalid log entry: " << line << '\n';
            continue;
        }

        if (blocked.count(mac)) {
            cout << timestamp << " | " << mac
                 << " | Client is quarantined.\n";
            continue;
        }

        if (status == "FAIL") {
            failures[mac]++;

            cout << timestamp << " | " << mac
                 << " | Failed attempts: "
                 << failures[mac] << '\n';

            if (failures[mac] >= threshold) {
                cout << "ALERT: Suspicious authentication attempts!\n";
                cout << "Client " << mac
                     << " has been quarantined.\n";
                cout << "Access to IP printer denied.\n\n";

                blocked.insert(mac);
            }
        }
        else if (status == "SUCCESS") {
            failures[mac] = 0;

            cout << timestamp << " | " << mac
                 << " | Authentication successful.\n";
        }
        else {
            cout << "Invalid status: " << status << '\n';
        }
    }

    file.close();

    cout << "\n--- Final Security Report ---\n";

    if (blocked.empty()) {
        cout << "No clients were quarantined.\n";
    } else {
        cout << "Quarantined clients:\n";

        for (const string& mac : blocked)
            cout << mac << '\n';
    }

    cout << "\nAnalysis completed.\n";

    return 0;
}
