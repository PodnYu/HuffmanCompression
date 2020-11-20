#pragma once
#include <iostream>
#include <vector>
#include <fstream>

class FileWriter {
private:
    std::vector<char> data;
    const char maxBit = 7;

    char currentByte = 0;
    char currentBit = this->maxBit;

    std::ofstream outFile;

public:
    ~FileWriter();

    // returns this->outFile.good();
    bool open(std::string &fileName);

    // packs bits to vector's char *data
    void fill(std::string &bitSeq);

    // writes vector's char *data to the file
    int write();

    void writeBytes(std::string &bytes);
};