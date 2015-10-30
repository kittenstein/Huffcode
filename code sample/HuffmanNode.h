#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

struct HuffmanSerializedNode {
	// retine daca nodul e terminal sau nu
    uint8_t isTerminal;
    union {
    	// daca nodul e terminal, ii retine valoarea/ caracterul
        uint8_t value;
        // daca nu, retine informatii despre descendenti
        struct {
          uint16_t leftChild;
          uint16_t rightChild;
        } __attribute__((__packed__)) childData;
    } __attribute__((__packed__));
} __attribute__((__packed__));

#endif // HUFFMANNODE_H
