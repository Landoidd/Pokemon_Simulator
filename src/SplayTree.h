#pragma once
#include "iostream"
#include "string"
using namespace std;

struct Pokemon {
    int pokedex_number;
    string name;
    string type1;
    string type2;
    int total;
    int hp;
    int attack;
    int defense;
    int sp_atk;
    int sp_def;
    int speed;
    int generation;
    bool legendary;
};

struct Node {
    Pokemon data; // holds the Pokémon object
    Node* parent; // pointer to the parent
    Node* left; // pointer to left child
    Node* right; // pointer to right child
};

typedef Node* NodePtr;
// class SplayTree implements the operations in Splay tree
class SplayTree {
private:
    NodePtr root;
    NodePtr searchTreeHelper(NodePtr node, int key);
    NodePtr searchTreeByNameHelper(NodePtr node, const string& pokemon_name);
    void inOrderTraversal(NodePtr node);
    void leftRotate(NodePtr x);
    void rightRotate(NodePtr x);
    void splay(NodePtr x);
public:
    SplayTree();
    void printPokemonNames();
    NodePtr searchTree(int k);
    Pokemon* searchTreeByName(const string& pokemon_name);
    void insert(Pokemon pokemon);
    NodePtr getRoot() {
        return this->root;
    }
};
