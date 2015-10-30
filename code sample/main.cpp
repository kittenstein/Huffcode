#include <cstdint>
#include "heap.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HuffmanNode.h"

#define NODES 1000
#define CHARS 3000

static int st;


void preorder(BinaryTreeNode* node, HuffmanSerializedNode HSN[], int poz);
void DFS(HuffmanSerializedNode HSN[], int poz, std::string str, std::string count[]);

int main(int argc, char* argv[])
{
    // declararea fisierelor
    FILE *in, *out;

    // compresia
    if (argv[1][1] == 'c'){
        // declararea unui dector de aparitii care
        // retine de cate ori apare fiecare caracter
        long long int count[CHARS];
        memset(count, 0, CHARS);
        long long unsigned int nr = 0;
        uint32_t lSize[1];
        uint8_t c[1];
        // deschiderea fisierului de intrare
        in = fopen(argv[2],"r");
        fseek (in, 0 , SEEK_END);
        // stabilirea dimensiunii fisierului initial
        lSize[0] = ftell (in);
        rewind (in);
        // modificarea numelui pentru fisierul de iesire
        char *name = argv[2];
        strcat(name, ".cmp");
        // deschiderea fisierului de iesire
        out = fopen(name,"w");

        // citire caracterelor din fisier pentru stabilirea
        // numarului de aparitii al fiecaruia
        while(int value = fread(c, 1, sizeof(char), in)){
            count[(int)c[0]]++;
            nr++;
        }

        // declararea heap-ului
        Heap heap;
        // declararea si alocarea memoriei pentru un array de noduri
        BinaryTreeNode **e;
        e = new BinaryTreeNode*[NODES];

        int k = 0;

        for (int i = 0; i <= 256; i++)
            if (count[i]){
                // initializarea nodurilor cu informatii
                e[k] = new BinaryTreeNode;
                e[k]->pinfo.probability = count[i];
                e[k]->pinfo.c = (uint8_t)i;
                e[k]->pinfo.isLeaf = true;

                // adaugarea nodurilor initiale in heap
                heap.insert(e[k]);
                k++;
            }

       	BinaryTreeNode *x, *y;

        // construirea arborelui Huffman, ce va ramane in memorie
        // pe prima pozitie din heap
        while (heap.getDim() != 1){
            x = heap.extractMin();
            y = heap.extractMin();

            // crearea unei radacini ce devine parinte pentru
            // cele doua elemente extrase din heap
            BinaryTreeNode *root;
            root = new BinaryTreeNode;
            root->pinfo.probability = x->pinfo.probability + y->pinfo.probability;
            root->pinfo.isLeaf = false;
            root->left_son = x;
            root->right_son = y;

            // inserarea in heap a radacinii
            heap.insert(root);
            
        }

        // declararea vectorului de noduri Huffman serializate
        HuffmanSerializedNode HSN[2 * k - 1];
        memset(HSN, 0, sizeof(HuffmanSerializedNode)*(2 * k - 1));
        // salvarea elementelor arborelui in vectorul de noduri 
        // printr-o parcurgere in preordine
        preorder(heap.peek(), HSN, 0);

        std::string str;
        std::string vect[CHARS];

        // salvarea caracterelor intr-un vector (ex. vect[(char)'a'] = 1010)
        DFS(HSN, 0, str, vect);

        delete[] e;

        long long int aux[1];
        // numarul de noduri, unde k este numarul de frunze
        aux[0] = k * 2 - 1;
        // afisarea: numar de noduri, arbore serializat, 
        // dimensiunea fisierului initial
        fwrite(aux, sizeof(uint16_t), 1, out);
        fwrite(HSN, sizeof(HuffmanSerializedNode), 2 * k - 1, out);
        fwrite(lSize, sizeof(uint32_t), 1, out);
        // intoarcerea la inceputul fisierului
        fseek(in, 0, SEEK_SET);
        str.clear();
        uint8_t a[1];
        uint8_t j = 1;
        a[0] = 0;
        // variabila ce retine daca s-au codificat 8 biti
        int cnt = 0;
        // recitirea fisierului si codificarea caracterelor
        while(int value = fread(c, 1, sizeof(uint8_t), in)){
            // daca nu au fost codificati 8 biti
            if (cnt != 8){
                // daca adaugarea sirului de codificare pentru caracterul
                // curent nu depaseste 8 biti
                if (vect[c[0]].length() + cnt < 8){
                    // se adauga caracterele din sirul de codificare
                    for (int i = 0; i < vect[c[0]].length(); i++){
                        if (vect[c[0]][i] == '1'){
                            a[0] = a[0] | (1 << cnt);
                        }
                        cnt ++;
                    }
                }
                // daca prin adaugarea sirului de cofificare se depasesc 8 biti
                else {
                    for (int i = 0; i < vect[c[0]].length(); i++){
                        // cand s-au gasit 8 biti
                        if (cnt == 8){
                            // se afiseaza in fisier
                            fwrite(a, sizeof(uint8_t), 1, out);
                            // se reinitializeaza contorul
                            cnt = 0;
                            a[0] = 0;
                            i--;
                        }
                        else{
                            // se continua codificarea
                            if (vect[c[0]][i] == '1'){
                                a[0] = a[0] | (1 << cnt);
                            }
                            cnt ++;
                        }
                    }
                }
            }
            else {
                // daca au fost gasiti 8 biti, se afiseaza in fisier
                fwrite(a, sizeof(uint8_t), 1, out);
                // se reinitializeaza contorul
                cnt = 0;
                a[0] = 0;
                // intoarcerea pe pozita anterioara in fisier
                fseek(in, -(sizeof(uint8_t)), SEEK_CUR);
            }
        }
        // scrierea ultimilor 8 biti
        if (a)
            fwrite(a, sizeof(uint8_t), 1, out);

        fclose(in);
        fclose(out);
    }
    // decompresia
    else{
        uint32_t lSize[1];
        lSize[0] = 0;
        // deschiderea fisierului de intrare
        in = fopen(argv[2],"r");
        // modificarea numelui pentru fisierul de iesire
        char *name = argv[2];
        char *name2;
        name2 = new char[100];
        strcpy(name2, "decompressed_");
        strcpy(name + strlen(name) - 4, "");
        strcat(name2, name);
        // deschiderea fisierului de iesire
        out = fopen(name2, "w");
        uint16_t nrNoduri[1];
        int value;
        // citirea numarului de noduri din vectorul de noduri
        value = fread(nrNoduri, sizeof(uint16_t), 1, in);
        // declararea vectorului
        HuffmanSerializedNode HSN[nrNoduri[0]];
        memset(HSN, 0, sizeof(HuffmanSerializedNode)*(nrNoduri[0]));
        // citirea vectorului
        value = fread(HSN, sizeof(HuffmanSerializedNode), nrNoduri[0], in);
        // citirea dimensiunii fisierului initial
        value = fread(lSize, sizeof(uint32_t), 1, in);
        uint8_t a[1];
        int k = 0, x, size = 0;
        uint8_t litera[0];
        // se citesc grupuri de 8 biti pana la sfarsitul fisierului
        while (value = fread(a, sizeof(uint8_t), 1, in)){
            int counter = 0;
            while (counter < 8 && size < lSize[0]){
                // daca s-a ajuns la frunza, se afiseaza litera decodificata
                if (HSN[k].isTerminal){
                    litera[0] = HSN[k].value;
                    fwrite(litera, sizeof(uint8_t), 1, out);
                    k = 0;
                    size ++;
                }
                else{ 
                    // se merge prin vector, conform sirului de biti
                    // 0 -> copilul stang, 1 -> copilul drept
                    x = int(a[0]) % 2;
                    if (x){
                        k = HSN[k].childData.rightChild;
                    }
                    else 
                        k = HSN[k].childData.leftChild;
                    counter ++;
                    a[0] = a[0] / 2;
                }
            }
        }
        fclose(in);
        fclose(out);
        delete[] name2;
    }
    return 0;
}

void preorder(BinaryTreeNode* node, HuffmanSerializedNode HSN[], int poz){
    // se adauga in vector nodul curent
    HSN[poz].isTerminal = node->pinfo.isLeaf;
    if (HSN[poz].isTerminal){
        HSN[poz].value = (uint8_t)node->pinfo.c;
        return;
    }
    // se stabileste copilul stang ca fiind elementul urmator
    HSN[poz].childData.leftChild = (uint16_t)++st;
    // se apeleaza functia de parcurgere
    preorder(node->left_son, HSN, st);
    // se adauga copilul drept
    HSN[poz].childData.rightChild = (uint16_t)++st;
    // se apeleaza functia de parcurgere
    preorder(node->right_son, HSN, st);

}

void DFS(HuffmanSerializedNode HSN[], int poz, std::string str, std::string count[]){
    // daca s-a ajuns la un nod terminal, salveaza in vector
    // sirul de biti corespunzator caracterului
    if (HSN[poz].isTerminal){ 
        count[(int)HSN[poz].value] = str;
    }
    else{
        // se formeaza sirul de caractere, adaugand 0 pentru
        // copil stang si 1 pentru copil drept
        str += '0';
        DFS(HSN, HSN[poz].childData.leftChild, str, count);
        if (str.length())
            str.erase(str.length()-1, 1);
        str += '1';
        DFS(HSN, HSN[poz].childData.rightChild, str, count);
        if (str.length())
            str.erase(str.length()-1, 1);
    }
}