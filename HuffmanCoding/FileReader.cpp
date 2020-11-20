#include "FileReader.hpp"

FileReader::~FileReader() {
    if (this->inFile.is_open()) this->inFile.close();
}

bool FileReader::open(std::string &fileName) {

    if (this->inFile.is_open()) this->inFile.close();

    this->inFile.open(fileName, std::ios::binary);
    return this->inFile.good();
}

std::string FileReader::readAll(bool encodeToBinary) {

    std::string result;
    std::streampos currentPosition = this->inFile.tellg();
    this->inFile.seekg(0, std::ios::end);
    std::streampos endPosition = this->inFile.tellg();
    result.reserve((endPosition - currentPosition) * 8);

    this->inFile.seekg(currentPosition, std::ios::beg);

    result = this->read(int(endPosition - currentPosition), encodeToBinary);

    return std::move(result);
}

std::string FileReader::read(int byteCount, bool encodeToBinary) {

    if (this->inFile.eof()) return "";

    std::string result;
    result.reserve(byteCount * (encodeToBinary ? 8 : 1 ));

    char ch;
    char currBit = this->maxBit;
    while (byteCount-- > 0) {
        ch = this->inFile.get();
        if (this->inFile.eof()) break;

        if (encodeToBinary) {
            for (; currBit != -1; --currBit) {
                char res = ch & (1 << currBit);
                result.push_back((res == 0) ? '0' : '1');
            }
            currBit = this->maxBit;
        } else {
            result.push_back(ch);
        }

    } 

    return std::move(result);

} 

void FileReader::rewind() {
    this->inFile.clear();
    this->inFile.seekg(0, this->inFile.beg);
}

bool FileReader::readBit() {

    if (currentBit == -1) {
        this->currentBit = this->maxBit;
        this->currentByte = this->inFile.get();
    }

    int res = this->currentByte & (1 << this->currentBit--);
    return res == 0 ? false : true;
}

int FileReader::readBits(int bitCount) {
    int res = 0;

    for (int i = bitCount - 1; i >= 0; --i) {
        res |= (this->readBit() == 0) ? 0 : (1 << i);
    }

    return res;
}