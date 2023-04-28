#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "SFML/Graphics.hpp"
#include "texture.h"
#include <set>
#include <cmath>
#include "SplayTree.h"
#include "hashTable.h"
using namespace std;


void read_pokemon_data_splay(string file_path, SplayTree& pokemon_data) {
    ifstream input_file(file_path);
    if (input_file.fail()) {
        cout << "Failed to open file." << endl;
        exit(1);
    }

    string header;
    getline(input_file, header);

    string line;
    while (getline(input_file, line)) {
        Pokemon pokemon;
        stringstream ss(line);
        string field;

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

        pokemon_data.insert(pokemon);

    }

    input_file.close();
}


string checkAlphabetical(string pokemon) {


    string capitalized = pokemon;
    if (!capitalized.empty() && std::isalpha(capitalized[0])) {
        capitalized[0] = std::toupper(capitalized[0]);
    }
    return capitalized;

}

map<string, set<string>> type_effectiveness = {
        {"Normal", {"Rock", "Steel"}},
        {"Fire", {"Grass", "Ice", "Bug", "Steel"}},
        {"Water", {"Fire", "Ground", "Rock"}},
        {"Electric", {"Water", "Flying"}},
        {"Grass", {"Water", "Ground", "Rock"}},
        {"Ice", {"Grass", "Ground", "Flying", "Dragon"}},
        {"Fighting", {"Normal", "Ice", "Rock", "Dark", "Steel"}},
        {"Poison", {"Grass", "Fairy"}},
        {"Ground", {"Fire", "Electric", "Poison", "Rock", "Steel"}},
        {"Flying", {"Grass", "Fighting", "Bug"}},
        {"Psychic", {"Fighting", "Poison"}},
        {"Bug", {"Grass", "Psychic", "Dark"}},
        {"Rock", {"Fire", "Ice", "Flying", "Bug"}},
        {"Ghost", {"Psychic", "Ghost"}},
        {"Dragon", {"Dragon"}},
        {"Dark", {"Psychic", "Ghost"}},
        {"Steel", {"Ice", "Rock", "Fairy"}},
        {"Fairy", {"Fighting", "Dragon", "Dark"}}
};



string determine_winning_pokemon(const Pokemon* pokemon1, const Pokemon* pokemon2, int level1, int level2) {
    // Define a threshold for the significant difference in total stats
    const int stat_difference_threshold = 50;

    int poke1Total = pokemon1->total;
    int poke2Total = pokemon2->total;

    // Compare the types of the two Pokémon
    set<string> pokemon1_types = { pokemon1->type1, pokemon1->type2 };

    set<string> pokemon2_types = { pokemon2->type1, pokemon2->type2 };

    // Check if either Pokémon has a type advantage over the other
    int pokemon1ADV = 0;
    int pokemon2ADV = 0;
    bool pokemon1_has_advantage = false;
    bool pokemon2_has_advantage = false;
    for (const auto& kv : type_effectiveness) { //iterates through all types
        if (kv.first == pokemon1->type1) {       //finds type that pokemon1 is
            for (const auto& effect : kv.second) {   //iterates through the advantages

                if (effect == pokemon2->type1) {      //if Pokémon 2 type is in Pokémon 1 advantages add an ADV
                    pokemon1ADV++;
                }

                if (effect == pokemon2->type2) {      //if Pokémon 2 type 2 is in Pokémon 1 advantages add an ADV
                    pokemon1ADV++;
                }
            }
        }
    }

    for (const auto& kv : type_effectiveness) {
        if (kv.first == pokemon2->type1) {
            for (const auto& effect : kv.second) {
                if (effect == pokemon1->type1) {
                    pokemon2ADV++;
                }
                if (effect == pokemon1->type2) {
                    pokemon1ADV++;
                }
            }
        }
    }

    int ADVdifference = abs(pokemon1ADV - pokemon2ADV);

    if (pokemon1ADV > pokemon2ADV) {
        pokemon1_has_advantage = true;
    }
    else if (pokemon1ADV < pokemon2ADV) {
        pokemon2_has_advantage = true;
    }

    if (ADVdifference == 0) {
        cout << "There is no type advantage!" << endl;
    }
    if (pokemon1_has_advantage && !pokemon2_has_advantage) {
        if (ADVdifference == 1) {
            cout << pokemon1->name << " is super effective against " << pokemon2->name << "." << endl;
            poke1Total += 300;
        }
        if (ADVdifference > 1) {
            cout << pokemon1->name << " is super effective against " << pokemon2->name << "." << endl;
            poke1Total += 350;
        }
    }
    else if (pokemon2_has_advantage && !pokemon1_has_advantage) {
        if (ADVdifference == 1) {
            cout << pokemon2->name << " is super effective against " << pokemon1->name << "." << endl;
            poke2Total += 300;
        }
        if (ADVdifference > 1) {
            cout << pokemon2->name << " is super effective against " << pokemon1->name << "." << endl;
            poke2Total += 350;
        }
    }
    poke1Total += 40 * level1;    //adds total stat for every level to account for Pokémon with higher levels
    poke2Total += 40 * level2;

    //compares total stats
    int total_stat_difference = abs(poke1Total - poke2Total);
    if (total_stat_difference >= stat_difference_threshold) {
        if (poke1Total > poke2Total) {
            cout << pokemon1->name << " won due to its immense total stat." << endl;
            return pokemon1->name;
        }
        else {
            cout << pokemon2->name << " won due to its immense total stat." << endl;
            return pokemon2->name;
        }
    }


    cout << "Both Pokemon have similar stats, so it was a tie!" << endl;
    return "tie";
}

void printStats(Pokemon* pokemon) {
    cout << "Pokedex Number: " << pokemon->pokedex_number << endl;
    cout << "Name: " << pokemon->name << endl;
    cout << "Type 1: " << pokemon->type1 << endl;
    cout << "Type 2: " << pokemon->type2 << endl;
    cout << "Total: " << pokemon->total << endl;
    cout << "HP: " << pokemon->hp << endl;
    cout << "Attack: " << pokemon->attack << endl;
    cout << "Defense: " << pokemon->defense << endl;
    cout << "Special Attack: " << pokemon->sp_atk << endl;
    cout << "Special Defense: " << pokemon->sp_def << endl;
    cout << "Speed: " << pokemon->speed << endl;
    cout << "Generation: " << pokemon->generation << endl;
    cout << "Legendary: " << (pokemon->legendary ? "Yes" : "No") << endl;
    cout << endl;
}


void loadStartMenu(sf::RenderWindow &window, sf::Sprite background, sf::Sprite battle, texture textures){
    /**== Load initial start menu ==**/
    background.setTexture(textures.GetTexture("Menu"));

    battle.setTexture(textures.GetTexture("Battle"));
    battle.setPosition(325, 325);
//    sprite3.setTexture(textures.GetTexture("Stats"));
//    sprite3.setPosition(500, 300);

    window.draw(background);
    window.draw(battle);
    window.display();
 //   window.draw(sprite3);
    /**== End of initial start menu ==**/
}

void loadGameMenu(sf::RenderWindow &window, texture &textures, string name1, string name2, string winner){

    window.clear();
    sf::Sprite battle;
    battle.setTexture(textures.GetTexture("Battle_Arena"));
    sf::Sprite poke1;
    poke1.setScale(3,3);
    sf::Sprite poke2;
    poke2.setScale(3,3);
    poke1.setTexture(textures.GetTexture(name1));
    poke1.setPosition(500, 240);
    poke2.setTexture(textures.GetTexture(name2));
    poke2.setPosition(250, 240);
    //Flip pokemon
    poke2.scale(-1, 1);
    sf::Sprite back_button;
    back_button.setTexture(textures.GetTexture("Back"));
    back_button.setPosition(650, 25);
    window.draw(battle);
   //window.draw(back_button);
    window.draw(poke1);
    window.draw(poke2);
    window.display();

    sf::Sprite victory;
    sf::Sprite explosion;
    explosion.setTexture(textures.GetTexture("explosion"));
    sf::Time time =  sf::seconds(3);
    sf::Time time2 =  sf::seconds(2);
    sleep(time);
    victory.setTexture(textures.GetTexture("victory"));
    if(name1 == winner){
        explosion.setPosition(-300,0);
        explosion.setScale(2, 1);
        window.draw(battle);
        window.draw(explosion);
        window.display();
        sleep(time2);
        window.clear();
        window.draw(battle);
        victory.setPosition(250, 100);
        window.draw(poke1);
        window.draw(victory);
        window.display();
        sleep(time);
    }
    else if (name2 == winner){
        explosion.setPosition(-300,0);
        explosion.setScale(2, 1);
        window.draw(battle);
        window.draw(explosion);
        window.display();
        sleep(time2);
        window.clear();
        window.draw(battle);
        victory.setPosition(250, 100);
        window.draw(poke2);
        window.draw(victory);
        window.display();
        sleep(time);
    }
    else{
        window.clear();
        window.draw(battle);
        window.draw(poke2);
        window.draw(poke1);
        window.display();
    }
    return;
}


int main() {

    texture textures;
    /**== Define pokemon Data Structure being tested ==**/
    cout << "What test do you want to implement? " << endl;
    cout << "1. Splay (Type Splay)" << endl;
    cout << "2. Hash Table (Type Hash)" << endl;
    string name;
    cin >> name;
    if(name == "Splay"){
        sf::RenderWindow window;
        window.create(sf::VideoMode(800, 600), "Pokemon Battle Simulator", sf::Style::Close);
        SplayTree pokemon_data;

        read_pokemon_data_splay("Pokemon/Pokemon.csv", pokemon_data);

        /**== Start Game ==**/

        while(window.isOpen()) {
            /**== Load initial start menu ==**/
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                window.clear();

                sf::Sprite battle_button;
                sf::Sprite background;

                loadStartMenu(window, background, battle_button, textures);


                /**== Determine if battle button was clicked ==**/
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        int x =  event.mouseButton.x / 64;
                        int y = event.mouseButton.y / 64;

                        if(x == 6 && y == 5){
                            /**== Load Game ==**/
                            string pokemon1_name, pokemon2_name;
                            int level1, level2;
                            cout << "Enter the name of Pokemon 1: ";
                            cin >> pokemon1_name;
                            pokemon1_name = checkAlphabetical(pokemon1_name);
                            cout << "Enter the level of Pokemon 1: ";
                            cin >> level1;

                            cout << "Enter the name of Pokemon 2: ";
                            cin >> pokemon2_name;
                            pokemon2_name = checkAlphabetical(pokemon2_name);
                            cout << "Enter the level of Pokemon 2: ";
                            cin >> level2;

                            Pokemon* pokemon1 = pokemon_data.searchTreeByName(pokemon1_name);
                            Pokemon* pokemon2 = pokemon_data.searchTreeByName(pokemon2_name);

                            if (pokemon1->pokedex_number == -1 || pokemon2->pokedex_number == -1) {
                                cout << "Incorrect pokemon name" << endl;
                                return -1;
                            }
                            Pokemon* pok1 = pokemon_data.searchTreeByName(pokemon1_name);
                            Pokemon* pok2 = pokemon_data.searchTreeByName(pokemon2_name);
                            string winner = determine_winning_pokemon(pok1, pok2, level1, level2);
                            loadGameMenu(window, textures, pokemon1_name, pokemon2_name, winner);
                            cout << "Type " << "'Stats'" << " for previous match stats" << endl;
                            string stats;
                            cin >> stats;
                            if(stats == "Stats"){
                                printStats(pokemon1);
                                printStats(pokemon2);
                            }
                        }
                    }
                }
            }
        }
    }
    else if(name == "Hash"){
        hashTable pokemon_data;
        int capacity = 50;  //how big the hash table will start as
        pokemon_data.resizeVec(capacity); //initializes size of hashtable
        vector<Pokemon> pokemons = pokemon_data.parseData("Pokemon/Pokemon.csv", pokemon_data);

        sf::RenderWindow window;
        window.create(sf::VideoMode(800, 600), "Pokemon Battle Simulator", sf::Style::Close);
        /**== Start Game ==**/
        while(window.isOpen()) {
            /**== Load initial start menu ==**/
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                window.clear();

                sf::Sprite battle_button;
                sf::Sprite background;

                loadStartMenu(window, background, battle_button, textures);


                /**== Determine if battle button was clicked ==**/
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        int x =  event.mouseButton.x / 64;
                        int y = event.mouseButton.y / 64;

                        if(x == 6 && y == 5){
                            /**== Load Game ==**/
                            string pokemon1_name, pokemon2_name;
                            int level1, level2;
                            cout << "Enter the name of Pokemon 1: ";
                            cin >> pokemon1_name;
                            pokemon1_name = checkAlphabetical(pokemon1_name);
                            cout << "Enter the level of Pokemon 1: ";
                            cin >> level1;

                            cout << "Enter the name of Pokemon 2: ";
                            cin >> pokemon2_name;
                            pokemon2_name = checkAlphabetical(pokemon2_name);
                            cout << "Enter the level of Pokemon 2: ";
                            cin >> level2;

                            Pokemon pokemon1 = pokemon_data.search(pokemon1_name);
                            Pokemon pokemon2 = pokemon_data.search(pokemon2_name);

                            if (pokemon1.pokedex_number == -1 || pokemon2.pokedex_number == -1) {
                                cout << "Incorrect pokemon name" << endl;
                                return -1;
                            }

                            Pokemon pok1 = pokemon_data.search(pokemon1_name);
                            Pokemon pok2 = pokemon_data.search(pokemon2_name);
                            string winner = determine_winning_pokemon(&pok1, &pok2, level1, level2);
                            loadGameMenu(window, textures, pokemon1_name, pokemon2_name, winner);
                            cout << "Type" << "'Stats'" << "for previous match stats" << endl;
                            string stats;
                            cin >> stats;
                            if(stats == "Stats"){
                                printStats(&pokemon1);
                                printStats(&pokemon2);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
