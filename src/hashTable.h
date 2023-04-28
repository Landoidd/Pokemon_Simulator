#pragma once
#include "iostream"
#include "vector"
#include "fstream"
#include "set"
#include "map"
#include "sstream"
#include "SplayTree.h"

using namespace std;


class hashTable {

private:
    vector<Pokemon> hashVec;
    int capacity = 50;
    int size=0; //amount of values in vector
    double maxLoadFactor=0.75;
    static Pokemon nullPokemon;
public:
    vector<Pokemon> parseData(string file_path,hashTable& hashTable);
    void insert(Pokemon pokemon);
    int quadraticProbe(int hashVal, int i);
    int hash(Pokemon pokemon, int modulo);
    void resizeVec(int num); //used for first time
    Pokemon search(string name);
};

