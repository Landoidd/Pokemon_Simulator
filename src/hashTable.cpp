#include "hashTable.h"
#include "SplayTree.h"
Pokemon hashTable::nullPokemon = { -1, "", "", "", 0, 0, 0, 0, 0, 0, 0, false };

void hashTable::insert(Pokemon pokemon) { //inserts, probes

    if (size != 0) { //prevents it from trying this when size is still 0
        if ((double)size / capacity >= maxLoadFactor) {  // all for resizing
            int newCapacity = capacity * 2;
            vector<Pokemon> newHash(newCapacity); //new vector with the new capacity as size
            for (int i = 0; i < capacity; i++) {
                if (!hashVec[i].name.empty()) {           //only does everything if it's not empty

                    int hashVal = hash(hashVec[i],newCapacity);

                    if (newHash[hashVal].name.empty()) { //if the hash value is empty take it
                        newHash[hashVal] = hashVec[i];
                    }
                    else { //otherwise need to linear probe

                        while (!newHash[hashVal].name.empty()) {
                            if (hashVal == (newCapacity - 1)) {
                                hashVal = 1;
                            }
                            hashVal = hashVal + 1;
                        }
                        newHash[hashVal] = hashVec[i];
                    }
                }
            }
            size++;
            capacity = newCapacity;
            hashVec = newHash;

        }
    }

    //then after resizing you would add the Pokémon

    int hashVal = hash(pokemon,capacity);

    if (hashVec[hashVal].name.empty()) {
        hashVec[hashVal] = pokemon;
        size++;  //increment by size every time we add a Pokémon
    }
    else {
        while (!hashVec[hashVal].name.empty()) {
            if (hashVal == (capacity-1)) {
                hashVal = 1;
            }
            hashVal = hashVal + 1; //try to do the modulo so it stays within bounds
        }
        hashVec[hashVal] = pokemon;
        size++;
    }


}

int hashTable::hash(Pokemon pokemon,int modulo) { //hash function

    int hashVal = 0;
    //need to create unique hash
    hashVal += pokemon.name.length();
    hashVal += pokemon.pokedex_number;
    hashVal += pokemon.total;
    hashVal += pokemon.type1.length();
    hashVal += pokemon.type2.length();
    return hashVal%hashVec.size();
}



vector<Pokemon> hashTable::parseData(string file_path,hashTable& hashTable) {
    vector<Pokemon> pokemon_data;

    ifstream input_file(file_path);
    if (input_file.fail()) {
        cout << "Failed to open file." << endl;
        exit(1);
    }
    // Discard the first row
    string header;
    getline(input_file, header);

    // Read each subsequent row and parse the data into a Pokémon object
    string line;
    while (getline(input_file, line)) {
        Pokemon pokemon;
        stringstream ss(line);
        string field;

        // Parse each field and store it in the corresponding Pokémon attribute
        getline(ss, field, ',');
        pokemon.pokedex_number = stoi(field);

        getline(ss, field, ',');
        pokemon.name = field;

        getline(ss, field, ',');
        pokemon.type1 = field;

        getline(ss, field, ',');
        pokemon.type2 = field;

        getline(ss, field, ',');
        pokemon.total = stoi(field);

        getline(ss, field, ',');
        pokemon.hp = stoi(field);

        getline(ss, field, ',');

        pokemon.attack = stoi(field);

        getline(ss, field, ',');
        pokemon.defense = stoi(field);

        getline(ss, field, ',');
        pokemon.sp_atk = stoi(field);

        getline(ss, field, ',');
        pokemon.sp_def = stoi(field);

        getline(ss, field, ',');
        pokemon.speed = stoi(field);

        getline(ss, field, ',');
        pokemon.generation = stoi(field);

        getline(ss, field);
        pokemon.legendary = (field == "True" || field == "TRUE" || field == "true");

        // Add the Pokémon object to the vector
        hashTable.insert(pokemon);
    }

    input_file.close();
    return pokemon_data;
}

void hashTable::resizeVec(int num) {
    hashVec.resize(num);
}

Pokemon hashTable::search(string pokemon) {
    for (int i = 0; i < capacity; i++) {
        if (hashVec[i].name == pokemon) {
            return hashVec[i];
        }
    }
    return nullPokemon;
}

