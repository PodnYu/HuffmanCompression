#include "FileWriter.hpp"

bool FileWriter::open(std::string &fileName) {
    if (this->outFile.is_open()) this->outFile.close();

    this->outFile.open(fileName, std::ios::binary);

    return this->outFile.good();
}

FileWriter::~FileWriter() {
    if (this->outFile.is_open()) this->outFile.close();
}

void FileWriter::fill(std::string &bitSeq) {
    for (int i = 0; i < bitSeq.length(); i++) {
        this->currentByte |= bitSeq[i] == '1' ? (1 << this->currentBit) : 0;

        --this->currentBit;
        if (this->currentBit == -1) {
            this->data.push_back(this->currentByte);
            this->currentByte = 0;
            this->currentBit = this->maxBit;
        }
    }
}

int FileWriter::write() {

    if (this->currentBit != this->maxBit) {
        this->data.push_back(currentByte);
    }

    int bytesWritten = this->data.size();

    this->outFile.write(this->data.data(), this->data.size());

    this->currentByte = 0;
    this->currentBit = this->maxBit;
    this->data.clear();

    return bytesWritten;
}

void FileWriter::writeBytes(std::string &bytes) {
    this->outFile.write(bytes.data(), bytes.size());
}