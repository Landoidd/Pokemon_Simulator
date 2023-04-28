#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include "string"
using namespace std;

class texture{
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string fileName);
    static unordered_map<string, sf::Texture> mine_textures;
    static map<int, sf::Sprite> digits;
public:
    static sf::Texture& GetTexture(string textureName);
    static sf::Texture& findMineTexture(string textureName);
    static void Clear();
};