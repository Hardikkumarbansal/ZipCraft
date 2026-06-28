#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <unordered_map>
#include <memory>
#include <queue>

// A node inside the Huffman frequency tree
struct HuffmanNode {
    char ch;
    int frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    // Constructor for leaf nodes (actual characters)
    HuffmanNode(char character, int freq) 
        : ch(character), frequency(freq), left(nullptr), right(nullptr) {}

    // Constructor for internal tree nodes
    HuffmanNode(int freq, std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r) 
        : ch('\0'), frequency(freq), left(l), right(r) {}
};

// Custom comparator to sort nodes in a min-heap priority queue
struct CompareNodes {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency; // Smallest frequency bubbles to top
    }
};

class HuffmanEncoder {
private:
    std::unordered_map<char, std::string> huffmanCodes;
    std::shared_ptr<HuffmanNode> root;

    // Helper functions
    void generateCodes(std::shared_ptr<HuffmanNode> node, const std::string& currentCode);
    void writeTreeToBinary(std::shared_ptr<HuffmanNode> node, std::string& serializedTree);
    std::shared_ptr<HuffmanNode> readTreeFromBinary(const std::string& serializedTree, int& index);

public:
    HuffmanEncoder() : root(nullptr) {}
    
    void compress(const std::string& inputFile, const std::string& outputFile);
    void decompress(const std::string& inputFile, const std::string& outputFile);
};

#endif // HUFFMAN_H