# ZipCraft 🗜️

A lightweight file compression and decompression tool built in C++ using the **Huffman Encoding** algorithm. ZipCraft reduces file sizes by assigning shorter bit sequences to more frequent characters.

## How It Works

1. Scans the input file and calculates character frequencies
2. Builds a Min-Heap priority queue from the frequencies
3. Constructs a Huffman Tree by repeatedly merging the two lowest-frequency nodes
4. Generates a binary code mapping table for each character
5. Packs the encoded bits into raw binary output with an embedded header for decoding

## Features

- Lossless compression and decompression
- Binary output with embedded Huffman table header
- Efficient bit packing into 8-bit bytes

## Build

```bash
g++ -std=c++17 src/Huffman.cpp -o zipcraft
```

## Usage

```cpp
HuffmanEncoder encoder;
encoder.compress("input.txt", "output.bin");
encoder.decompress("output.bin", "restored.txt");
```

## Project Structure

```
ZipCraft/
├── include/
│   └── Huffman.h
├── src/
│   └── Huffman.cpp
└── README.md
```