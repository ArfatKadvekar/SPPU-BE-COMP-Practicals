#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string imageFilename;
    cout << "Enter the path to the raw disk image/partition file: ";
    cin >> imageFilename;

    // Open the raw disk image or partition in binary mode based on user input
    ifstream diskImage(imageFilename, ios::binary);
    if (!diskImage.is_open()) {
        cout << "Error: Unable to open raw disk image: " << imageFilename << endl;
        return 1;
    }

    // JPEG Magic Number (Header: FF D8 FF, Footer: FF D9)
    const unsigned char jpegHeader[] = {0xFF, 0xD8, 0xFF};
    
    char byte;
    bool carving = false;
    ofstream recoveredFile;
    int fileCounter = 1;
    vector<char> buffer;

    cout << "\n--- Forensic File Recovery Started ---" << endl;
    cout << "[*] Scanning for JPEG signatures in unallocated space of " << imageFilename << "..." << endl;

    while (diskImage.get(byte)) {
        buffer.push_back(byte);
        
        // Maintain a sliding window of 3 bytes for signature comparison
        if (buffer.size() > 3) {
            buffer.erase(buffer.begin());
        }

        // Header Detection
        if (!carving && buffer.size() == 3 && 
            (unsigned char)buffer[0] == jpegHeader[0] && 
            (unsigned char)buffer[1] == jpegHeader[1] && 
            (unsigned char)buffer[2] == jpegHeader[2]) {
            
            carving = true;
            string filename = "recovered_image_" + to_string(fileCounter++) + ".jpg";
            recoveredFile.open(filename, ios::binary);
            
            if (recoveredFile.is_open()) {
                cout << "[!] Signature match found! Carving: " << filename << endl;
                // Write the initial header bytes to the new file
                recoveredFile.put(buffer[0]);
                recoveredFile.put(buffer[1]);
                recoveredFile.put(buffer[2]);
            }
        } 
        // Data Extraction and Footer Detection
        else if (carving) {
            if (recoveredFile.is_open()) {
                recoveredFile.put(byte);
            }
            
            // Check for JPEG EOF footer (FF D9)
            if ((unsigned char)buffer[1] == 0xFF && (unsigned char)buffer[2] == 0xD9) {
                cout << "[*] End of file footer found. Recovery complete." << endl;
                recoveredFile.close();
                carving = false;
            }
        }
    }

    diskImage.close();
    cout << "--- Forensic Scan Complete ---" << endl;

    return 0;
}