#include "HuffmanCompressor.hpp"

HuffmanCompressor::HuffmanCompressor() {

}

HuffmanCompressor::~HuffmanCompressor() {

    if (this->root != nullptr) {
        delete this->root;
        this->root = nullptr;
    }
}

HuffNode &HuffmanCompressor::getTree(FileReader &fileReader) {

    std::map<char, int> charFrequencies;
    std::string chunk;

    while ((chunk = fileReader.read(this->chunkSize, false)) != "") {
        for (int i = 0; i < chunk.length(); ++i) {
            charFrequencies[chunk[i]]++;
            ++this->inputFileSize;
        }
    }

    std::priority_queue<HuffNode*, std::vector<HuffNode*>, HuffNodesComparator> huffNodes;
    
    for (auto pair : charFrequencies) {
        HuffNode *node = new HuffNode(pair.first, pair.second);
        huffNodes.push(node);
    }

    //idk EOF frequency should be 0 or 1
    HuffNode *EOFNode = new HuffNode(this->HuffEOF, 0);
    huffNodes.push(EOFNode);

    while (huffNodes.size() > 1) {

        HuffNode *node1 = huffNodes.top();
        huffNodes.pop();

        HuffNode *node2 = huffNodes.top();
        huffNodes.pop();

        HuffNode *parentNode = new HuffNode(node1->getFrequency() + node2->getFrequency(), *node1, *node2);

        huffNodes.push(parentNode);
    }

    this->root = huffNodes.top();

    return *this->root;
}

void HuffmanCompressor::encode(FileReader &fileReader, FileWriter &fileWriter) {

    std::cout << "Building Huffman tree..." << std::endl;

    HuffNode &root = this->getTree(fileReader);
    std::string path = "";

    this->getCharCodes(root, path);

    std::cout << "Writing header..." << std::endl;

    // this->printCharCodes();

    this->writeHeader(fileWriter);
    std::string output;

    std::cout << "Encoding..." << std::endl;

    //to build Huffman tree we read the file
    //now get to the beginning and start actually encoding
    fileReader.rewind();

    std::string chunk;
    while ((chunk = fileReader.read(this->chunkSize, false)) != "") {
        for (int i = 0; i < chunk.length(); ++i) {
            output.append(this->charCodes[chunk[i]]);
        }
        fileWriter.fill(output);
        output = "";
    }

    fileWriter.fill(this->charCodes[this->HuffEOF]);

    this->encodedBodySize = fileWriter.write();
    std::cout << std::endl;
}

void HuffmanCompressor::getCharCodes(HuffNode &node, std::string &path) {

    if (&node.getLeftNode() == nullptr) {
        this->charCodes[node.getValue()] = path;
    } else {
        std::string temp = path;
        this->getCharCodes(node.getLeftNode(), path.append("0"));
        this->getCharCodes(node.getRightNode(), temp.append("1"));
    }
}

void HuffmanCompressor::decode(FileReader &fileReader, FileWriter &fileWriter) {

    std::cout << "Getting Huffman tree..." << std::endl;

    //get tree
    this->readHeader(fileReader);

    std::cout << "Decoding..." << std::endl;
    HuffNode temp = *this->root;
    std::string output;

    std::string chunk;
    bool foundEOF = false;
    while ((chunk = fileReader.read(this->chunkSize)) != "") {
        for (int i = 0; i < chunk.length(); i++) {
            if (chunk[i] == '0') temp = temp.getLeftNode();
            else temp = temp.getRightNode();

            if (&temp.getLeftNode() == nullptr) {
                int value = temp.getValue();

                if (value == this->HuffEOF) {
                    // std::cout << "Found EOF" << std::endl;
                    foundEOF = true;
                    break;
                }

                output.push_back((char)value);
                temp = *this->root;
            }
        }
        fileWriter.writeBytes(output);
        output = "";
        if (foundEOF) break;
    }

    if (!foundEOF) {
        std::cerr << "Something went wrong: no EOF!" << std::endl;
    }
}

void HuffmanCompressor::formHeader(HuffNode &node, std::string &header) {
    if (&node.getLeftNode() == nullptr) {
        header.push_back('1');
        int value = node.getValue();
        char shift = 8;
        for (; shift != -1; --shift) {
            int res = value & (1 << shift);
            header.push_back((res == 0) ? '0' : '1');
        }
    } else {
        header.push_back('0');
        this->formHeader(node.getLeftNode(), header);
        this->formHeader(node.getRightNode(), header);
    }
}

void HuffmanCompressor::writeHeader(FileWriter &fileWriter) {
    std::string header;
    formHeader(*this->root, header);

    fileWriter.fill(header);
    this->encodedHeaderSize = fileWriter.write();
}

void HuffmanCompressor::readHeader(FileReader &fileReader) {

    if (this->root != nullptr) delete this->root;
    this->root = new HuffNode();
    this->charCodes.clear();

    this->formTree(*this->root, fileReader);

    std::string path = "";
    this->getCharCodes(*this->root, path);
}

void HuffmanCompressor::printCharCodes() {
    for (auto pair : this->charCodes) {
        std::cout << pair.first << "[\'" << (char)pair.first << "\']" << " : " << pair.second << std::endl;
    }
}

void HuffmanCompressor::formTree(HuffNode &node, FileReader &fileReader) {
    bool bit = fileReader.readBit();
    if (bit) {
        int value = fileReader.readBits(9);
        node.setValue(value);
    } else {
        node.createLeftNode();
        node.createRightNode();

        this->formTree(node.getLeftNode(), fileReader);
        this->formTree(node.getRightNode(), fileReader);
    }
}

int HuffmanCompressor::compress(std::string &sourceFileName, std::string &destFileName) {

    FileReader fileReader;
    bool good = fileReader.open(sourceFileName);

    if (!good) return ErrorCodes::SourceFileNotFound;

    FileWriter fileWriter;
    good = fileWriter.open(destFileName);
    if (!good) return ErrorCodes::CannotCreateDestFile;

    this->encode(fileReader, fileWriter);

    std::cout << "Done" <<std::endl;

    return 0;
}

int HuffmanCompressor::decompress(std::string &sourceFileName, std::string &destFileName) {
    FileReader fileReader;
    bool good = fileReader.open(sourceFileName);
    if (!good) return ErrorCodes::SourceFileNotFound;

    FileWriter fileWriter;
    good = fileWriter.open(destFileName);
    if (!good) return ErrorCodes::CannotCreateDestFile;

    this->decode(fileReader, fileWriter);

    std::cout << "Done" <<std::endl;

    return 0;
}

void HuffmanCompressor::printCompressionInfo() {
    std::cout << "Input file size: " << this->inputFileSize << " bytes." << std::endl;
    std::cout << "Encoded header size: " << this->encodedHeaderSize << " bytes." << std::endl;
    std::cout << "Encoded body size: " << this->encodedBodySize << " bytes." << std::endl;
}