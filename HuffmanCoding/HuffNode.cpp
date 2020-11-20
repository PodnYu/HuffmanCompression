#include "HuffNode.hpp"
int HuffNode::instanceCounter = 0;

HuffNode::HuffNode() {
    this->leftNode = nullptr;
    this->rightNode = nullptr;
    this->value = 0;
    this->frequency = 0;
    
    instanceCounter++;
}

HuffNode::HuffNode(int value, int freq) : HuffNode() {
    this->value = value;
    this->frequency = freq;
}

HuffNode::HuffNode(int freq, HuffNode &leftNode, HuffNode &rightNode) : HuffNode() {
    this->frequency = freq;
    this->leftNode = &leftNode;
    this->rightNode = &rightNode;
}

//Copy constructor
/*
    Not very 'true' copy constructor because it just copies pointers.
    After that two objects (this and other) point to the same [left/right]Node.
    In destructor it will cause some troubles because the last alive object
    will try to free already freed memory.

    This behaviour was added to be able to iterate through the Huffman tree
    for example in HuffmanCompressor#decode method and not to copy entire subtree starting from this node.

    I don't want to use move constructor or move operator because I need the initial object to remain the same.

    Fix for now is a bool this->isACopy which is set in copy constructor and assignment operator to true.
    See this->operator = and this->~HuffNode
*/
HuffNode::HuffNode(const HuffNode &other) : HuffNode() {
    this->value = other.getValue();
    this->frequency = other.getFrequency();
    this->leftNode = &other.getLeftNode();
    this->rightNode = &other.getRightNode();
    this->isACopy = true;
}

//assignment operator
/*
    See copy constructor this->HuffNode(const HuffNode &other)

    For now, problem with double pointer deleting is 'solved' but
    I want to iterate through Huffman tree without copying this node's subtree.
    With this in mind, I should get nextNode with its [left/right]Node pointers.
    I do it here. But. I shouldn't clear that pointers in destructor, because, otherwise,
    It will invalidate initial object's (other) pointers.

    So, without delete this->[left/right]Node it's a leak here if using this operator beyond the iterating context.
    If it's just to iterate like temp = temp.getLeftNode(); and actual Huffman tree remains unchanged,
    freeing root node will free entire tree.
*/
HuffNode &HuffNode::operator = (const HuffNode &other) {

    // hotfix to avoid memory leak
    if (!this->isACopy) {
        delete this->leftNode;
        delete this->rightNode;
    }

    this->value = other.getValue();
    this->frequency = other.getFrequency();

    this->leftNode = &other.getLeftNode();
    this->rightNode = &other.getRightNode();

    this->isACopy = true;

    return *this;
}

HuffNode::~HuffNode() {

    if (!this->isACopy) {
        if (this->leftNode != nullptr) {
            delete this->leftNode;
        }
        if (this->rightNode != nullptr) {
            delete this->rightNode;
        }
    }

    this->leftNode = nullptr;
    this->rightNode = nullptr;
    --instanceCounter;
}

int HuffNode::getValue() const {
    return this->value;
}

int HuffNode::getFrequency() const {
    return this->frequency;
}

HuffNode &HuffNode::getLeftNode() const {
    return *this->leftNode;
}

HuffNode &HuffNode::getRightNode() const {
    return *this->rightNode;
}

void HuffNode::createLeftNode() {
    this->leftNode = new HuffNode();
}

void HuffNode::createRightNode() {
    this->rightNode = new HuffNode();
}

void HuffNode::setLeftNodeNull() {
    this->leftNode = nullptr;
}

void HuffNode::setRightNodeNull() {
    this->rightNode = nullptr;
}

void HuffNode::setValue(int value) {
    this->value = value;
}

void HuffNode::getInfo() {
    std::cout << "value: " << this->getValue() << ", frequency: " << this->getFrequency() << ", address: " << this << std::endl;
}



bool HuffNodesComparator::operator () (HuffNode *node1, HuffNode *node2) {
    return (*node1).getFrequency() > (*node2).getFrequency();
}