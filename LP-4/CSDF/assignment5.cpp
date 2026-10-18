#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main() {
    string logFilename;
    cout << "Enter the path to the system log file: ";
    cin >> logFilename;

    // File Input handling using ifstream dynamically based on user input
    ifstream logFile(logFilename);
    if (!logFile.is_open()) {
        cout << "Error: Unable to open log file: " << logFilename << endl;
        return 1;
    }

    string line;
    map<string, int> failedLogins; // To track failed attempts per IP
    vector<string> securityAlerts; // To store correlated high-severity events

    cout << "\n--- Log Capturing Started on " << logFilename << " ---" << endl;

    // Log Parsing and Event Correlation
    while (getline(logFile, line) && !line.empty()) {
        // Simulating simple keyword-based log parsing
        if (line.find("Failed login") != string::npos) {
            // Extracting IP address (assuming format: "... Failed login from [IP]")
            size_t ipPos = line.find("from ");
            if (ipPos != string::npos) {
                string ipAddress = line.substr(ipPos + 5);
                failedLogins[ipAddress]++;

                // Correlation Engine: Trigger alert if 3 or more failed logins occur from the same IP
                if (failedLogins[ipAddress] == 3) {
                    securityAlerts.push_back("Brute-force attack suspected from IP: " + ipAddress);
                }
            }
        } else if (line.find("CRITICAL") != string::npos || line.find("FATAL") != string::npos) {
            securityAlerts.push_back("High Severity System Event: " + line);
        }
    }
    logFile.close();

    // Report Generation
    cout << "\n--- Event Correlation Report ---" << endl;
    if (securityAlerts.empty()) {
        cout << "[*] No security anomalies detected." << endl;
    } else {
        for (size_t i = 0; i < securityAlerts.size(); ++i) {
            cout << "[!] ALERT " << (i + 1) << ": " << securityAlerts[i] << endl;
        }
    }

    return 0;
}
