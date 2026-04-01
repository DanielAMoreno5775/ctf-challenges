#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ptrace.h>
#include <unistd.h>
#endif

using namespace std;

bool isProcessBeingDebugged() {
#ifdef _WIN32
    return IsDebuggerPresent() != 0;
#else
    // On Linux, check TracerPid in /proc/self/status
    ifstream status_file("/proc/self/status");
    string line;
    while (getline(status_file, line)) {
        // Check if line starts with "TracerPid:"
        if (line.rfind("TracerPid:", 0) == 0) {
            // Extract value
            string pid_str = line.substr(line.find('\t') + 1);
            int tracer_pid = stoi(pid_str);
            return tracer_pid != 0;
        }
    }
    return false;
#endif
}

/* Helper functions */
// Trivial prevention of overflows
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Generate the key for encryption
vector<uint8_t> generatePermutationKey() {
    vector<uint8_t> key(256);
    // Fill {0,...,255}
    iota(key.begin(), key.end(), 0);
    // Use permutation table to generate random key
    for (int i = 255; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(key[i], key[j]);
    }
    return key;
}

// Build inverse permutation key for E_K^{-1}
vector<uint8_t> generateInverseKey(const vector<uint8_t>& key) {
    vector<uint8_t> inv(256);
    for (int i = 0; i < 256; ++i) inv[key[i]] = static_cast<uint8_t>(i);
    return inv;
}

// Look up x in the permutation table key
uint8_t blockEncrypt(const vector<uint8_t>& key, uint8_t x) {
    return key[x];
}

uint8_t blockDecrypt(const vector<uint8_t>& invKey, uint8_t x) {
    return invKey[x];
}

// Convert text to bytes and vice versa
string bytesToBitString(const vector<uint8_t>& bytes) {
    string result;
    for (uint8_t byte : bytes) {
        for (int i = 7; i >= 0; --i) {
            result += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    return result;
}
vector<uint8_t> bitStringToBytes(const string& bits) {
    vector<uint8_t> bytes;
    // Pad on the left to a multiple of 8
    int pad = (8 - (bits.size() % 8)) % 8;
    string padded(pad, '0');
    padded += bits;

    // Convert to big-endian byte vector
    for (unsigned int i = 0; i < padded.size(); i += 8) {
        uint8_t byte = 0;
        for (int b = 0; b < 8; ++b) {
            byte = (byte << 1) | (padded[i + b] - '0');
        }
        bytes.push_back(byte);
    }
    return bytes;
}

// Confirm the input is a bitstring
bool isBitString(const string& str) {
    bool result = false;
    
    // Confirm value exists and only contains 0/1
    if (str.empty()) { 
        return result;
    }
    for (char c : str) {
        if (c != '0' && c != '1') {
            return result;
        }
    }
    
    result = true;
    return result;
}

// Get the bitstring from the input
string readBitString(const string& prompt) {
    string value;
    
    cout << prompt;
    // If value retrieved and is bit string,
    if (cin >> value && isBitString(value)) {
        clearInputBuffer();
        return value;
    }
    // Clean out input buffer to prevent overflows
    clearInputBuffer();
    return "";
}



/* Encryption schemes */
string encryptGame1_3(const vector<uint8_t>& key, const string& bitString) {
    // Break into 8-bit bytes
    vector<uint8_t> blocks = bitStringToBytes(bitString);

    // Randomly generate the IV
    uint8_t iv = static_cast<uint8_t>(rand() % 256);

    // Prepend the IV
    vector<uint8_t> ciphertext;
    ciphertext.push_back(iv);
    // XOR each block with the IV
    for (uint8_t block : blocks) {
        ciphertext.push_back(blockEncrypt(key, iv ^ block));
    }

    return bytesToBitString(ciphertext);
}

string encryptGame4_6(const vector<uint8_t>& key, const string& bitString) {
    // Input validation guarantees bitString.size() % 8 == 0 so no padding occurs
    vector<uint8_t> blocks = bitStringToBytes(bitString);

    uint8_t iv = static_cast<uint8_t>(rand() % 256);
    // IV encryption is same for all blocks
    uint8_t keystream = blockEncrypt(key, iv);

    // Prepend the IV
    vector<uint8_t> ciphertext;
    ciphertext.push_back(iv);
    // XOR the encrypted IV with each message block
    for (uint8_t block : blocks) {
        ciphertext.push_back(keystream ^ block);
    }

    return bytesToBitString(ciphertext);
}

string encryptGame7_9(const vector<uint8_t>& key, const string& bitString) {
    vector<uint8_t> blocks = bitStringToBytes(bitString);
    uint8_t iv = static_cast<uint8_t>(rand() % 256);

    // Prepend the IV
    vector<uint8_t> ciphertext;
    ciphertext.push_back(iv);
    // Encrypt the IV
    uint8_t prev = blockEncrypt(key, iv);
    ciphertext.push_back(prev);

    // Encrypt each block based on the previous one
    for (uint8_t block : blocks) {
        uint8_t ci = blockEncrypt(key, prev ^ block);
        ciphertext.push_back(ci);
        prev = ci;
    }

    return bytesToBitString(ciphertext);
}

string decryptGame7_9(const vector<uint8_t>& key, const vector<uint8_t>& invKey,
                      const string& bitString) {
    vector<uint8_t> blocks = bitStringToBytes(bitString);

    // Need C_0 (IV), C_1, and at least one message-bearing block C_2
    if (blocks.size() < 3) { 
        return "---";
    }

    uint8_t c0 = blocks[0];

    // Validate the IV
    uint8_t iv_prime = blockDecrypt(invKey, blocks[1]);
    if (iv_prime != c0) return "---";

    // If all other checks passed, decrypt the message
    vector<uint8_t> message;
    for (size_t i = 1; i < blocks.size() - 1; ++i) {
        message.push_back(blockDecrypt(invKey, blocks[i + 1]) ^ blocks[i]);
    }

    return bytesToBitString(message);
}



/* Oracle headers & printed encryption algorithms */
void printOracleHeaderGame1_3() {
    cout << "\n";
    cout << "===========================================\n";
    cout << "              GUIDING LIGHT                \n";
    cout << "===========================================\n";
    cout << "The lighthouse uses the following encryption scheme:\n";
    cout << "  Break M into 8-bit blocks: M_1 ||...|| M_n.\n";
    cout << "  IV ⬅^$ {0,1}^8\n";
    cout << "  For i=1,...,n do\n";
    cout << "    C_i ⬅ E_K(IV ⨁ M_i)\n";
    cout << "  EndFor\n";
    cout << "  Return IV || C_1 ||...|| C_n\n";
    cout << "Messages must be non-empty strings of bits (e.g. \"0\", \"0101\").\n\n";
}

void printOracleHeaderGame4_6() {
    cout << "\n";
    cout << "===========================================\n";
    cout << "              GUIDING LIGHT                \n";
    cout << "===========================================\n";
    cout << "The lighthouse uses the following encryption scheme where message length is multiple of m (8):\n";
    cout << "  Break M into 8-bit blocks: M_1 ||...|| M_n.\n";
    cout << "  IV ⬅^$ {0,1,...,2^m-1}\n";
    cout << "  For i=1,...,n do\n";
    cout << "    C_i ⬅ E_K(IV) ⨁ M_i\n";
    cout << "  EndFor\n";
    cout << "  Return IV || C_1 ||...|| C_n\n";
    cout << "Messages must be non-empty strings of bits (e.g. \"0\", \"0101\").\n\n";
}

void printEncryptionOracleHeaderGame7_9() {
    cout << "\n";
    cout << "===========================================\n";
    cout << "              GUIDING LIGHT                \n";
    cout << "===========================================\n";
    cout << "The lighthouse uses the following encryption scheme:\n";
    cout << "  Break M into 8-bit blocks: M_1 ||...|| M_n.\n";
    cout << "  IV ⬅^$ {0,1}^8\n";
    cout << "  C_1 ⬅ E_K(IV)\n";
    cout << "  For i=1,...,n do\n";
    cout << "    C_{i+1} ⬅ E_K(C_i ⨁ M_i)\n";
    cout << "  EndFor\n";
    cout << "  Return IV || C_1 ||...|| C_{n+1}\n";
    cout << "Messages must be non-empty strings of bits (e.g. \"0\", \"0101\").\n\n";
}
void printDecryptionOracleHeaderGame7_9() {
    cout << "\n";
    cout << "===========================================\n";
    cout << "              GUIDING LIGHT                \n";
    cout << "===========================================\n";
    cout << "The lighthouse uses the following decryption scheme:\n";
    cout << "  Break C into 8-bit blocks: C_0 || C_1 ||...|| C_{n+1}.\n";
    cout << "  For i=1,...,n do\n";
    cout << "    M_i ⬅ E_K^{-1}(C_{i+1}) ⨁ C_i\n";
    cout << "  EndFor\n";
    cout << "  M ⬅ M_1 ||...|| M_n\n";
    cout << "  IV' ⬅ E_K^{-1}(C_{1})\n";
    cout << "  If IV' == C_0, return M\n";
    cout << "  Else, return an error\n";
    cout << "Message must be non-empty strings of bits (e.g. \"0\", \"0101\").\n";
    cout << "Message cannot have been returned by the previous lighthouse.\n\n";
}



/* UI */
void runDecryptionOracleInteraction(const vector<uint8_t>& key, const vector<uint8_t>& invKey, const string& forbiddenCt) {
    while (true) {
        printDecryptionOracleHeaderGame7_9();

        string ct = readBitString("Enter ciphertext (bit string): ");
        if (ct.empty()) {
            cout << "  [!] Invalid input. Must be a non-empty string of 0s and 1s.\n";
            cout << "  [!] The lighthouse will allow you to communicate again.\n";
            continue;
        }

        if (ct.size() % 8 != 0) {
            cout << "  [!] Ciphertext length must be a multiple of 8 bits.\n";
            cout << "  [!] The lighthouse will allow you to communicate again.\n";
            continue;
        }

        // Reject anything the encryption oracle returned this game
        if (ct == forbiddenCt) {
            cout << "  [!] That ciphertext was returned by the lighthouse.\n";
            cout << "  [!] The lighthouse will allow you to communicate again.\n";
            continue;
        }

        string result = decryptGame7_9(key, invKey, ct);
        cout << "\nThe lighthouse responds: " << result << "\n\n";
        return;
    }
}

void runOracleInteraction(int game, int world, const vector<uint8_t>& key, const vector<uint8_t>& invKey) {
    while (true) {
        if (game <= 3) {
            printOracleHeaderGame1_3();
        }
        if (game > 3 && game <= 6) {
            printOracleHeaderGame4_6();
        }
        if (game > 6 && game <= 9) {
            printEncryptionOracleHeaderGame7_9();
        }

        string m0 = readBitString("Enter message 0 (bit string): ");
        // Will be empty if string was invalid
        if (m0.empty()) {
            cout << "  [!] Invalid input. Must be a non-empty string of 0s and 1s.\n";
            cout << "  [!] The lighthouse will allow you to communicate again.\n";
            continue;
        }

        string m1 = readBitString("Enter message 1 (bit string): ");
        if (m1.empty()) {
            cout << "  [!] Invalid input. Must be a non-empty string of 0s and 1s.\n";
            cout << "  [!] The lighthouse will allow you to communicate again.\n";
            continue;
        }
        
        // These games need messages to be multiples of 8 bits in length
        if (game > 3 && game <= 6) {
            if (m0.size() % 8 != 0 || m1.size() % 8 != 0) {
                cout << "  [!] For this lighthouse communique, message length for both must be a multiple of 8 bits.\n";
                cout << "  [!] The lighthouse will allow you to communicate again.\n";
                continue;
            }
        }

        // Use world to select a message
        const string& chosen = (world == 0) ? m0 : m1;
        if (game <= 3) {
            cout << "\nThe lighthouse keeper responds: " << encryptGame1_3(key, chosen) << "\n\n";
        }
        if (game > 3 && game <= 6) {
            cout << "\nA new lighthouse keeper responds: " << encryptGame4_6(key, chosen) << "\n\n";
        }
        if (game > 6 && game <= 9) {
            string ct = encryptGame7_9(key, chosen);
            cout << "\nThe latest lighthouse keeper responds: " << ct << "\n\n";
            runDecryptionOracleInteraction(key, invKey, ct);
        }
        return;
    }
}

int main() {
    // Seed the randomness & generate the key
    srand(static_cast<unsigned int>(time(nullptr)));
    const vector<uint8_t> KEY = generatePermutationKey();
    const vector<uint8_t> INV_KEY = generateInverseKey(KEY);

    const int NUM_GAMES = 9;
    
    if (isProcessBeingDebugged()) {
        cout << "No debugging";
        exit(1);
    }

    cout << "==============================================\n";
    cout << "           The Lighthouse Challenge           \n";
    cout << "==============================================\n";
    cout << "You will play " << NUM_GAMES << " games.\n";
    cout << "In each game, a secret world (0 or 1) is chosen at random.\n";
    cout << "Each game, the lighthouse will guide you, providing the algorithm.\n";
    cout << "Each time, send the lighthouse keeper two bit-string messages.\n";
    cout << "Messages are treated as binary numbers (\"0101\" = 5).\n";
    cout << "The lighthouse encrypts the message whose number matches the secret world.\n";
    cout << "In Games 7-9, the lighthouse will also decrypt a message, so long as the ciphertext was not returned by the previous lighthouse communication.\n";
    cout << "Finally, guess which world was chosen so that you may safely arrive at port.\n";
    cout << "A single wrong guess ends the game immediately.\n";
    cout << "After all, navigation in such waters is no laughing matter.\n";
    cout << "Win all " << NUM_GAMES << " games to reveal the hidden string.\n";
    cout << "Disclaimer: It is possible to bypass this logic.\n";
    cout << "However, it is far harder than solving the challenge as intended.\n";

    for (int game = 1; game <= NUM_GAMES; ++game) {
        int world = rand() % 2;

        cout << "\n===========================================\n";
        cout << "  GAME " << game << " of " << NUM_GAMES << "\n";
        cout << "===========================================\n";

        runOracleInteraction(game, world, KEY, INV_KEY);

        int guess = -1;
        while (guess == -1) {
            cout << "Which world was chosen (0 or 1)? ";
            int raw;
            if (cin >> raw && (raw == 0 || raw == 1)) {
                guess = raw;
            } else {
                cout << "Invalid input. Please enter 0 or 1.\n";
            }
            clearInputBuffer();
        }

        if (guess == world) {
            cout << "Correct! The world was " << world << ".\n";
            cout << "Progress: " << game << " / " << NUM_GAMES << "\n";
        } else {
            cout << "Wrong. The world was " << world << ".\n";
            cout << "\nGame over. Better luck next time!\n\n";
            return 0;
        }
    }

    const string SECRET = "PCA{cryp70_d3f1n1710n5_0f_53cur17y}";
    cout << "\n===========================================\n";
    cout << "  PERFECT SCORE: " << NUM_GAMES << " / " << NUM_GAMES << "\n";
    cout << "===========================================\n";
    cout << "\nThe hidden string is revealed:\n";
    cout << "  >> " << SECRET << " <<\n\n";

    return 0;
}