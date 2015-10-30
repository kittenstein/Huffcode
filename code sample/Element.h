#ifndef ELEMENT_H
#define ELEMENT_H

class Element{
public:
    int long long probability;
    uint8_t c;
    bool isLeaf;

    // constructor
    Element(){
        probability = 0;
        c = 0;
        isLeaf = false;
    }

    // assignment operator
    Element operator= (Element right){
        this->probability = right.probability;
        this->c = right.c;
        this->isLeaf = right.isLeaf;
        return *this;
    }
};

#endif // ELEMENT_H