#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <algorithm>

using namespace std;

// --- Helper Functions ---
string convert_decimal_to_binary(int decimal) {
    return bitset<4>(decimal).to_string();
}

int convert_binary_to_decimal(const string& binary) {
    return stoi(binary, nullptr, 2);
}

string Xor(const string& a, const string& b) {
    string result = "";
    for (size_t i = 0; i < b.size(); i++) {
        result += (a[i] != b[i]) ? '1' : '0';
    }
    return result;
}

// --- Permutations ---
string initial_permutation(const string& input) {
    const int ip[64] = {
        58,50,42,34,26,18,10,2, 60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6, 64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1, 59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5, 63,55,47,39,31,23,15,7
    };
    string permuted = "";
    for (int i = 0; i < 64; i++) permuted += input[ip[i] - 1];
    return permuted;
}

string inverse_initial_permutation(const string& input) {
    const int ip_inv[64] = {
        40,8,48,16,56,24,64,32, 39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30, 37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28, 35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26, 33,1,41,9,49,17,57,25
    };
    string permuted = "";
    for (int i = 0; i < 64; i++) permuted += input[ip_inv[i] - 1];
    return permuted;
}

// --- Key Generation ---
class KeyGenerator {
private:
    string key;
    vector<string> roundKeys;
    const int pc1[56] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
    const int pc2[48] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};

    string shift_left(string chunk, int n) {
        for(int i=0; i<n; i++) chunk = chunk.substr(1) + chunk[0];
        return chunk;
    }

public:
    KeyGenerator(const string& input_key) : key(input_key) {}
    void generateRoundKeys() {
        roundKeys.clear();
        string permKey = "";
        for (int i = 0; i < 56; ++i) permKey += key[pc1[i] - 1];
        string L = permKey.substr(0, 28), R = permKey.substr(28, 28);
        for (int i = 0; i < 16; ++i) {
            int shift = (i == 0 || i == 1 || i == 8 || i == 15) ? 1 : 2;
            L = shift_left(L, shift); R = shift_left(R, shift);
            string combined = L + R, rKey = "";
            for (int j = 0; j < 48; ++j) rKey += combined[pc2[j] - 1];
            roundKeys.push_back(rKey);
        }
    }
    vector<string> getRoundKeys() { return roundKeys; }
};

// --- DES Core ---
class DES {
private:
    const int E[48] = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
    const int P[32] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
    const int S[8][4][16] = {
        {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
        {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
        {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
        {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
        {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
        {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
        {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
        {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
    };

    string feistel(string R, string K) {
        string expanded = "";
        for(int i=0; i<48; i++) expanded += R[E[i]-1];
        string xored = Xor(expanded, K);
        string substituted = "";
        for(int i=0; i<8; i++) {
            int row = convert_binary_to_decimal(string(1, xored[i*6]) + xored[i*6+5]);
            int col = convert_binary_to_decimal(xored.substr(i*6+1, 4));
            substituted += convert_decimal_to_binary(S[i][row][col]);
        }
        string permuted = "";
        for(int i=0; i<32; i++) permuted += substituted[P[i]-1];
        return permuted;
    }

public:
    string processBlock(string block, vector<string> keys) {
        string perm = initial_permutation(block);
        string L = perm.substr(0, 32), R = perm.substr(32, 32);
        for(int i=0; i<16; i++) {
            string next_R = Xor(L, feistel(R, keys[i]));
            L = R; R = next_R;
        }
        return inverse_initial_permutation(R + L);
    }
};

// --- Triple DES Logic ---
string runTripleDES(string input, string k1, string k2, string k3, bool encryptMode) {
    KeyGenerator kg1(k1), kg2(k2), kg3(k3);
    kg1.generateRoundKeys(); kg2.generateRoundKeys(); kg3.generateRoundKeys();
    
    vector<string> keys1 = kg1.getRoundKeys();
    vector<string> keys2 = kg2.getRoundKeys();
    vector<string> keys3 = kg3.getRoundKeys();
    
    DES core;
    if (encryptMode) {
        // E(K3, D(K2, E(K1, P)))
        string s1 = core.processBlock(input, keys1);
        reverse(keys2.begin(), keys2.end());
        string s2 = core.processBlock(s1, keys2);
        return core.processBlock(s2, keys3);
    } else {
        // D(K1, E(K2, D(K3, C)))
        reverse(keys3.begin(), keys3.end());
        string s1 = core.processBlock(input, keys3);
        string s2 = core.processBlock(s1, keys2);
        reverse(keys1.begin(), keys1.end());
        return core.processBlock(s2, keys1);
    }
}

int main() {
    int mode;
    if (!(cin >> mode)) return 0;

    if (mode == 1 || mode == 2) { // DES Encrypt/Decrypt
        string input, key;
        cin >> input >> key;
        
        if (mode == 1) { // Q2: Padding for Encryption
            while(input.length() % 64 != 0) input += "0";
        }

        KeyGenerator kg(key);
        kg.generateRoundKeys();
        vector<string> keys = kg.getRoundKeys();
        if (mode == 2) reverse(keys.begin(), keys.end()); // Q3: Decryption keys

        DES des;
        string result = "";
        for (size_t i = 0; i < input.length(); i += 64) {
            result += des.processBlock(input.substr(i, 64), keys);
        }
        cout << result << endl;
    } 
    else if (mode == 3 || mode == 4) { // Q4: TripleDES
        string input, k1, k2, k3;
        cin >> input >> k1 >> k2 >> k3;
        cout << runTripleDES(input, k1, k2, k3, mode == 3) << endl;
    }

    return 0;
}
