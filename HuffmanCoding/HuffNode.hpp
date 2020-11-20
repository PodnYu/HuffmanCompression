#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

class HuffNode {

private:

    //probably should be shared_ptrs
    HuffNode *leftNode;
    HuffNode *rightNode;
    int value;
    int frequency;

    //introduce this bool to prevent clearing [left/right]Node pointers
    //if this object was altered by copy constructor or assignment operator
    bool isACopy = false;

public: 
    HuffNode();
    HuffNode(int value, int freq);
    HuffNode(int freq, HuffNode &leftNode, HuffNode &rightNode);
    HuffNode(const HuffNode &other);
    ~HuffNode();
    HuffNode &getLeftNode() const;
    HuffNode &getRightNode() const;
    void setLeftNodeNull();
    void setRightNodeNull();
    void createLeftNode();
    void createRightNode();
    int getValue() const;
    int getFrequency() const;
    void setValue(int value);
    void getInfo();
    HuffNode &operator = (const HuffNode &other);

    // just a check whether all instances are destructed
    static int instanceCounter;

};

//a comparator for HuffNodes (used to create priority_queue)
class HuffNodesComparator {
public:
    bool operator () (HuffNode *node1, HuffNode *node2);
};
