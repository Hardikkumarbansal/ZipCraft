#include "../include/Huffman.h"
#include <iostream>
#include <fstream>

// Recursive helper to traverse the tree and map characters to bit strings
void HuffmanEncoder::generateCodes(std::shared_ptr<HuffmanNode> node, const std::string& currentCode) {
    if (!node) return;

    // If it's a leaf node, record the compiled bit code mapping
    if (!node->left && !node->right) {
        huffmanCodes[node->ch] = currentCode;
    }

    generateCodes(node->left, currentCode + "0");
    generateCodes(node->right, currentCode + "1");
}

void HuffmanEncoder::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::in);
    if (!in.is_open()) {
        std::cerr << "Error opening input file: " << inputFile << "\n";
        return;
    }

    // 1. Calculate the frequency of every character in the file
    std::unordered_map<char, int> frequencies;
    char ch;
    std::string textContent = "";
    while (in.get(ch)) {
        frequencies[ch]++;
        textContent += ch;
    }
    in.close();

    if (frequencies.empty()) return;

    // 2. Build a Min-Heap Priority Queue out of the frequencies
    std::priority_queue<std::shared_ptr<HuffmanNode>, 
                        std::vector<std::shared_ptr<HuffmanNode>>, 
                        CompareNodes> minHeap;

    for (auto const& [character, count] : frequencies) {
        minHeap.push(std::make_shared<HuffmanNode>(character, count));
    }

    // 3. Combine lowest nodes repeatedly until a single root tree remains
    while (minHeap.size() > 1) {
        auto left = minHeap.top(); minHeap.pop();
        auto right = minHeap.top(); minHeap.pop();

        auto parent = std::make_shared<HuffmanNode>(left->frequency + right->frequency, left, right);
        minHeap.push(parent);
    }
    root = minHeap.top();

    // 4. Generate the bit-mapping translation table
    huffmanCodes.clear();
    generateCodes(root, "");

    // 5. Pack the bit sequences directly into a raw binary stream output file
    std::ofstream out(outputFile, std::ios::out | std::ios::binary);
    
    // Write out a mini-header tracking the character frequency array sizes so decoder knows the map
    size_t uniqueChars = huffmanCodes.size();
    out.write(reinterpret_cast<const char*>(&uniqueChars), sizeof(uniqueChars));
    for (auto const& [character, code] : huffmanCodes) {
        out.put(character);
        size_t codeLen = code.length();
        out.write(reinterpret_cast<const char*>(&codeLen), sizeof(codeLen));
        out.write(code.c_str(), codeLen);
    }

    // Pack bits into full 8-bit bytes
    char bitBuffer = 0;
    int bitCount = 0;

    for (char c : textContent) {
        std::string code = huffmanCodes[c];
        for (char bit : code) {
            bitBuffer <<= 1;
            if (bit == '1') {
                bitBuffer |= 1;
            }
            bitCount++;

            // Once we fill up 8 bits, write the raw byte block out to disk
            if (bitCount == 8) {
                out.put(bitBuffer);
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }

    // Clear out any remaining trailing bits left over in buffer padding
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);
        out.put(bitBuffer);
    }
    
    out.close();
    std::cout << "[ZipCraft] File successfully compressed into " << outputFile << "\n";
}

void HuffmanEncoder::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Error opening compressed file: " << inputFile << "\n";
        return;
    }

    // Read the embedded header mapping table back into RAM memory
    size_t uniqueChars;
    in.read(reinterpret_cast<char*>(&uniqueChars), sizeof(uniqueChars));
    
    std::unordered_map<std::string, char> reverseCodes;
    for (size_t i = 0; i < uniqueChars; i++) {
        char character = in.get();
        size_t codeLen;
        in.read(reinterpret_cast<char*>(&codeLen), sizeof(codeLen));
        std::string code(codeLen, ' ');
        in.read(&code[0], codeLen);
        reverseCodes[code] = character;
    }

    // Unpack bits back to original text character format
    std::ofstream out(outputFile, std::ios::out);
    char byte;
    std::string currentBitString = "";

    while (in.get(byte)) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            currentBitString += (bit == 1) ? "1" : "0";

            if (reverseCodes.find(currentBitString) != reverseCodes.end()) {
                out.put(reverseCodes[currentBitString]);
                currentBitString = "";
            }
        }
    }
    in.close();
    out.close();
    std::cout << "[ZipCraft] File successfully decompressed into " << outputFile << "\n";
}