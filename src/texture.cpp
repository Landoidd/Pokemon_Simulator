#include "texture.h"

// "redeclare" variable so this file knows it exists and can use it
unordered_map<string, sf::Texture> texture::textures;

void texture::LoadTexture(string fileName) {
    string path = "C:\\Users\\trash\\CLionProjects\\Pokemon\\Images\\";
    path += fileName + ".png";
    textures[fileName].loadFromFile(path);
}

sf::Texture& texture::GetTexture(string textureName){

    //If texture doesn't exist load it first then return it
    if(textures.find(textureName) == textures.end()){
        LoadTexture(textureName);
    }
    return textures[textureName];
}

void texture::Clear() {
    textures.clear(); //get rid of all stored objects
}

