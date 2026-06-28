#include "../include/Huffman.h"
#include <iostream>
#include <fstream>

int main() {
    HuffmanEncoder encoder;

    std::string sampleFile = "sample.txt";
    std::string compressedFile = "compressed.bin";
    std::string decompressedFile = "restored.txt";

    // 1. Generate a repetitive dummy file to showcase high optimization scaling
    std::ofstream out(sampleFile);
    out << "Sand dunes are natural mounds or ridges of sand formed by the action of wind, commonly found in deserts and coastal regions. They develop when wind carries loose sand particles and deposits them in a particular area, where they gradually accumulate. Over time, these dunes can take on various shapes—such as crescent, linear, or star-shaped—depending on wind direction and strength. Besides being striking landforms, sand dunes play an important ecological role by protecting inland areas from wind erosion and providing habitats for specialized plants and animals.";
    out.close();

    std::cout << "[System] Starting compression routine...\n";
    encoder.compress(sampleFile, compressedFile);

    std::cout << "[System] Starting decompression routine...\n";
    encoder.decompress(compressedFile, decompressedFile);

    return 0;
}