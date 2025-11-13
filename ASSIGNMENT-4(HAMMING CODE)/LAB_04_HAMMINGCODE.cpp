#include <iostream>     
#include <cmath>        
#include <vector>      
using namespace std;

int calcRedundantBits(int m) {
    int r = 0;                       
    while (pow(2, r) < (m + r + 1)) {
        r++;                        
    }
    return r;                        // Return final redundant bits count
}

vector<int> insertParityBits(const vector<int>& data, int r) {
    int m = data.size();             
    int total = m + r;               
    vector<int> hamming(total + 1);  
    int j = 0;                       // Pointer for data bits
    
    for (int i = 1; i <= total; i++) {
       
        if ((i & (i - 1)) == 0) {
            hamming[i] = 0;          // Set parity bit placeholder
        } else {
            hamming[i] = data[j++];  // Fill data bit
        }
    }
    return hamming;                  // Return packet with parity spots
}
// Function to calculate parity bits (EVEN parity)
void setParityBits(vector<int>& hamming, int r) {
    int total = hamming.size() - 1;  // Total bits (1-indexed)
    // For each parity bit
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);         // Position of parity bit (1,2,4,8...)
        int count = 0;               // Count of 1s for parity check
        // Check bits that influence this parity bit
        for (int j = 1; j <= total; j++) {
            if (j & pos) {           // If this bit participates in parity
                count += hamming[j]; // Add the bit value
            }
        }
        hamming[pos] = count % 2;    // Set even parity ? parity = count mod 2
    }
}
// Function to detect error at receiver side
int detectError(const vector<int>& hamming, int r) {
    int total = hamming.size() - 1;  // Total bits
    int errorPos = 0;                // Store error position
    // Recalculate parity bits
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);         // Parity position
        int count = 0;               // Count of 1s
        // Check all bits related to this parity bit
        for (int j = 1; j <= total; j++) {
            if (j & pos) {           // If bit influences this parity bit
                count += hamming[j];
            }
        }
        // If parity doesn't match (count should be even)
        if (count % 2 != 0) {
            errorPos += pos;         // Add position to error index
        }
    }
    return errorPos;                 // Return final error location
}
int main() {
    string input;
    cout << "Enter binary data: ";   // Ask user for binary string
    cin >> input;                    // Read the input
    vector<int> data;                // Vector to store bits
    // Convert string characters to integers (0 or 1)
    for (char c : input) {
        data.push_back(c - '0');
    }
    int m = data.size();             // Number of data bits
    int r = calcRedundantBits(m);    // Calculate redundant bits
    // Insert parity bits into correct positions
    vector<int> hamming = insertParityBits(data, r);
    // Set actual parity values
    setParityBits(hamming, r);
    // Display final packet to be sent
    cout << "\nThe data packet to be sent is: ";
    for (int i = 1; i < hamming.size(); i++) {
        cout << hamming[i] << " ";
    }
    cout << "\n";
    // Receiver-side simulation
    cout << "\nSimulating receiver...\n";
    int errorPos = detectError(hamming, r); // Recheck parity bits
    // If errorPos is 0 ? no error found
    if (errorPos == 0) {
        cout << "Correct data packet received\n";
    } else {
        cout << "Error detected at position: " << errorPos << "\n";
        cout << "Correcting error...\n";
        // Flip the erroneous bit (0?1 or 1?0)
        hamming[errorPos] ^= 1;
        // Display corrected packet
        cout << "Corrected data packet: ";
        for (int i = 1; i < hamming.size(); i++) {
            cout << hamming[i] << " ";
        }
        cout << "\n";
    }
    return 0;                         // End of program
}

