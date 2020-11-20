#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include "HuffNode.hpp"
#include "FileReader.hpp"
#include "FileWriter.hpp"

/*
    Maybe I should have divided it into compressor and decompressor.
*/
class HuffmanCompressor {

private:
    HuffNode *root = nullptr;
    const int HuffEOF = 256;

    //size of chunk when reading from file (not to load entire file to RAM at once)
    const int chunkSize = 50;

    //map of codes. For example: 65 ('A') -> "0111" 
    std::map<int, std::string> charCodes;

    std::string inputFileName;
    std::string outputFileName;

    HuffNode &getTree(FileReader &fileReader);
    void getCharCodes(HuffNode &node, std::string &path);

    //creates header string of 0s and 1s where stores Huffman tree information.
    void formHeader(HuffNode &node, std::string &content);

    //reads header and recreates Huffman tree based on it.
    void formTree(HuffNode &node, FileReader &fileReader);

    void encode(FileReader &fileReader, FileWriter &fileWriter);
    void decode(FileReader &fileReader, FileWriter &fileWriter);
    void writeHeader(FileWriter &fileWriter);
    void readHeader(FileReader &fileReader);

    // info variables
    // compressing
    long inputFileSize = 0;
    int encodedHeaderSize = 0;
    long encodedBodySize = 0;

    // decompressing
    long outputFileSize = 0;

public:
    HuffmanCompressor();
    ~HuffmanCompressor();
    int compress(std::string &sourceFileName, std::string &destFileName);
    int decompress(std::string &sourceFileName, std::string &destFileName);
    void printCompressionInfo();
    void printCharCodes();

    enum ErrorCodes { SourceFileNotFound = 1, CannotCreateDestFile = 2 };

};