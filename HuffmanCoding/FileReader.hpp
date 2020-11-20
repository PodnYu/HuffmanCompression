#pragma once
#include <fstream>

class FileReader {
private:
    const char maxBit = 7;
    char currentByte = 0;
    char currentBit = -1;
    std::ifstream inFile;

public:
    ~FileReader();

    // returns this->outFile.good();
    bool open(std::string &fileName);

    // reads the rest of the file using this->read
    std::string readAll(bool encodeToBinary = true);

    //reads byteCount bytes (or less if there is eof on the way) from file and returns them as a string.
    //if there is nothing to read (eof) returns empty string
    //encodeToBinary is true by default and means that returned string will consist of 0s and 1s, representing data in 'binary'
    std::string read(int byteCount, bool encodeToBinary = true);

    //reads single bit from this->currentByte on this->currentBit position
    //if this->currentBit == -1 - reads the next byte from file.
    bool readBit();

    //packs bitCount bits (up to 32) into int
    int readBits(int bitCount);

    //set file cursor to beginning
    //this->inFile.seekg(0, this->inFile.beg);
    void rewind();
};