#include "SplayTree.h"

NodePtr SplayTree::searchTreeHelper(NodePtr node, int key) {
    if (node == nullptr || key == node->data.pokedex_number) {
        return node;
    }

    if (key < node->data.pokedex_number) {
        return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
}

NodePtr SplayTree::searchTreeByNameHelper(NodePtr node, const string& pokemon_name) {
    if (node == nullptr || pokemon_name == node->data.name) {
        return node;
    }

    NodePtr left_search = searchTreeByNameHelper(node->left, pokemon_name);
    if (left_search != nullptr) {
        return left_search;
    }
    return searchTreeByNameHelper(node->right, pokemon_name);
}

void SplayTree::inOrderTraversal(NodePtr node) {
    if (node == nullptr) {
        return;
    }

    inOrderTraversal(node->left);
    cout << node->data.name << endl;
    inOrderTraversal(node->right);
}

// rotate left at node x
void SplayTree::leftRotate(NodePtr x) {
    NodePtr y = x->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// rotate right at node x
void SplayTree::rightRotate(NodePtr x) {
    NodePtr y = x->left;
    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

SplayTree::SplayTree(){
    root = nullptr;
}

void SplayTree::splay(NodePtr x) {
    while (x->parent) {
        if (!x->parent->parent) {
            if (x == x->parent->left) {
// zig rotation
                rightRotate(x->parent);
            }
            else {
// zag rotation
                leftRotate(x->parent);
            }
        }
        else if (x == x->parent->left && x->parent == x->parent->parent->left) {
// zig-zig rotation
            rightRotate(x->parent->parent);
            rightRotate(x->parent);
        }
        else if (x == x->parent->right && x->parent == x->parent->parent->right) {
// zag-zag rotation
            leftRotate(x->parent->parent);
            leftRotate(x->parent);
        }
        else if (x == x->parent->right && x->parent == x->parent->parent->left) {
// zig-zag rotation
            leftRotate(x->parent);
            rightRotate(x->parent);
        }
        else {
// zag-zig rotation
            rightRotate(x->parent);
            leftRotate(x->parent);
        }
    }
}



void SplayTree::printPokemonNames() {
    inOrderTraversal(this->root);
}




// search the tree for the key k
// and return the corresponding node
NodePtr SplayTree::searchTree(int k) {
    NodePtr x = searchTreeHelper(this->root, k);
    if (x) {
        splay(x);
    }
    return x;
}

Pokemon* SplayTree::searchTreeByName(const string& pokemon_name) {
    NodePtr x = searchTreeByNameHelper(this->root, pokemon_name);
    if (x) {
        splay(x);
        return &x->data;
    }
    return nullptr;
}



// insert the key to the tree in its appropriate position
void SplayTree::insert(Pokemon pokemon) {

    // normal BST insert
    NodePtr node = new Node;
    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->data = pokemon;
    NodePtr y = nullptr;
    NodePtr x = this->root;

    while (x != nullptr) {
        y = x;
        if (node->data.pokedex_number < x->data.pokedex_number) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    // y is parent of x
    node->parent = y;
    if (y == nullptr) {
        root = node;
    }
    else if (node->data.pokedex_number < y->data.pokedex_number) {
        y->left = node;
    }
    else {
        y->right = node;
    }

    // splay the node
    splay(node);
}

