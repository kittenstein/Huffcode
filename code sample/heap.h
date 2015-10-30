#ifndef HEAP_H
#define HEAP_H
#include <cstdint>
#include <iostream>
#include "Element.h"
#include "BinaryTreeNode.h"
#define NODES 1000

class Heap
{ 
private:
    BinaryTreeNode* H[NODES];
    int dimVect;
public:
    Heap();
    ~Heap();

    int getDim();

    bool compare(int poz1, int poz2);

    uint8_t representant(BinaryTreeNode*);

    int level(BinaryTreeNode*);
 
    int parent(int poz);
 
    int leftChild(int poz);
 
    int rightChild(int poz);
 
    void pushUp(int poz);
 
    void pushDown();
 
    void insert(BinaryTreeNode* x);
 
    BinaryTreeNode* peek();
 
    BinaryTreeNode* extractMin();
};

// constructor
Heap::Heap(){
    dimVect = 0;
}

// destructor
Heap::~Heap(){
   delete H[dimVect];
}

// returns the number of elements existing in heap
int Heap::getDim(){
    return dimVect;
}

// returns true if H[poz1] < H[poz2] and false otherwise
bool Heap::compare(int poz1, int poz2){
        // comparing probabilities
        if (H[poz1]->pinfo.probability < H[poz2]->pinfo.probability){
            return false;
        }
        else if (H[poz1]->pinfo.probability > H[poz2]->pinfo.probability){
            return true;
        }
        else {
            // comparing heights
            if (level(H[poz1]) > level(H[poz2]))
                return true;
            else if (level(H[poz1]) < level(H[poz2]))
                return false;
            else{
                // comparing the represensants
                if (representant(H[poz1]) < representant(H[poz2]))
                    return false;
                return true;
            }

        }
}

// returns the height of the tree (BinaryTreeNodes)
int Heap::level(BinaryTreeNode* x){
    // returns 1 if no descendants;
    if(!x->left_son && !x->right_son)
        return 1;
    // if it has both descendants, returns max of their heights
    // plus 1
    if (x->left_son && x->right_son)
        return std::max(level(x->left_son), level(x->right_son)) + 1;
    // if only left child, returns height + 1 
    if (x->left_son)
        return level(x->left_son) + 1;
    // if only right child, returns height + 1 
    return level(x->right_son) + 1;

}

// returneaza reprezentantul unui nod
uint8_t Heap::representant(BinaryTreeNode* x){
    // daca e frunza/ terminal, ii intoarce valoarea
    if (x->pinfo.isLeaf == true){
        return x->pinfo.c;
    }
    // returneaza minimul dintre reprezentantii copiilor nodului
    return std::min(representant(x->left_son), representant(x->right_son));
}

// returneaza pozitia parintelui
int Heap::parent(int poz){
    return int(poz / 2);
}

// returneaza pozitia copilului stang
int Heap::leftChild(int poz)
{
    return 2 * poz;
}

// returneaza pozitia copilului drept
int Heap::rightChild(int poz)
{
    return 2 * poz + 1;
}

// realizeaza functia de pushUp al unui nod din heap
void Heap::pushUp(int poz){
    int parent;
    BinaryTreeNode* valAux;
    parent = poz / 2;
    // permuta elementul cu parintele lui pana se 
    // respecta proprietatile unui heap
    while (poz > 1 && compare(parent, poz)){
        valAux = H[parent];
        H[parent] = H[poz];
        H[poz] = valAux;
        poz = parent;
        parent = poz / 2;
    }
}

// realizeaza functia de pushDown
void Heap::pushDown(){
    int poz = 1;
    BinaryTreeNode *valAux;
    while (1){
        if (rightChild(poz) > dimVect){
            if (leftChild(poz) > dimVect){
                // daca nu are descendenti, nu mai poate sa coboare
                break;
            }
            // daca are copil stang si acesta este mai mic decat el,
            // se interschimba nodurile
            else if (compare(poz, leftChild(poz))){
                valAux = H[leftChild(poz)];
                H[leftChild(poz)] = H[poz];
                H[poz] = valAux;
                poz = leftChild(poz);

            }
            else {
                break;
            }
        }
        // daca are ambii copii
        else{
            // daca copilul stang e mai mic decat cel drept si decat
            // parintele, interschimba nodurile
            if (compare(rightChild(poz), leftChild(poz)) 
                && compare(poz, leftChild(poz))){
                valAux = H[leftChild(poz)];
                H[leftChild(poz)] = H[poz];
                H[poz] = valAux;
                poz = leftChild(poz);
            }
            // daca copilul drept e mai mic decat cel stang si decat
            // parintele, interschimba nodurile
            else if (compare(leftChild(poz), rightChild(poz)) 
                && compare(poz, rightChild(poz))){
                valAux = H[rightChild(poz)];
                H[rightChild(poz)] = H[poz];
                H[poz] = valAux;
                poz = rightChild(poz);
            }
            else{
                break;
            }
        }
   }
}

// insereaza un nod in heap
void Heap::insert(BinaryTreeNode* x){
    // il adauga pe ultima pozitie
    dimVect++;
    H[dimVect] = x;
    // apeleaza metoda de pushUp
    pushUp(dimVect);
}

// returneaza primul element din heap
BinaryTreeNode* Heap::peek(){
    if (dimVect){
        return H[1];
    }
    BinaryTreeNode *x;
    return x;
}

// extrage cel mai mic element din heap
BinaryTreeNode* Heap::extractMin(){
    // daca heapul nu e gol
    if (dimVect >= 1){
        // salveaza elementul de pe prima pozitie
        BinaryTreeNode* minValue = H[1];
        // il inlocuieste cu ultima frunza
        H[1] = H[dimVect];
        dimVect--;
        // apeleaza metoda de pushDown
        if(dimVect != 1 && dimVect != 0)
            pushDown();
        return minValue;
    }
    BinaryTreeNode *x;
    return x;
}

#endif // HEAP_H
