#include <random>
#include <string>
#include <utility>
#include <iostream>

class Treap {
private:
    struct Node {
        int key;
        uint32_t pr;
        Node* left = nullptr;
        Node* right = nullptr;
        Node(int k, uint32_t p) : key(k), pr(p) {}
    };

    Node* root = nullptr;
    std::mt19937 rng;

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    // Right rotation:
    //      n              l
    //     / \            / \
    //    l   C   =>     A   n
    //   / \                / \
    //  A   B              B   C
    Node* rotateRight(Node* n) {
        Node* l = n->left;
        n->left = l->right;
        l->right = n;
        return l;
    }

    // Left rotation:
    //    n                 r
    //   / \               / \
    //  A   r     =>      n   C
    //     / \           / \
    //    B   C         A   B
    Node* rotateLeft(Node* n) {
        Node* r = n->right;
        n->right = r->left;
        r->left = n;
        return r;
    }

    bool contains(Node* n, int key) const {
        while (n) {
            if (key < n->key) n = n->left;
            else if (key > n->key) n = n->right;
            else return true;
        }
        return false;
    }

    Node* insert(Node* n, int key, uint32_t pr) {
        if (!n) return new Node(key, pr);

        if (key < n->key) {
            n->left = insert(n->left, key, pr);
            if (n->left && n->left->pr > n->pr) n = rotateRight(n);
        } else if (key > n->key) {
            n->right = insert(n->right, key, pr);
            if (n->right && n->right->pr > n->pr) n = rotateLeft(n);
        } else {
            // already exists: do nothing
        }
        return n;
    }

    Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->pr > b->pr) {
            a->right = merge(a->right, b);
            return a;
        } else {
            b->left = merge(a, b->left);
            return b;
        }
    }

    Node* erase(Node* n, int key) {
        if (!n) return nullptr;
        if (key < n->key) {
            n->left = erase(n->left, key);
        } else if (key > n->key) {
            n->right = erase(n->right, key);
        } else {
            Node* res = merge(n->left, n->right);
            delete n;
            return res;
        }
        return n;
    }

    void printStructure(Node* n, const std::string& prefix, bool isLeft) const {
        if (!n) return;

        // Right subtree first (appears on top)
        printStructure(n->right, prefix + (isLeft ? "│   " : "    "), false);

        // Node itself
        std::cout << prefix
                  << (isLeft ? "└── " : "┌── ")
                  << n->key << " [pr=" << n->pr << "]"
                  << "\n";

        // Left subtree (appears at bottom)
        printStructure(n->left, prefix + (isLeft ? "    " : "│   "), true);
    }

public:
    Treap() : rng(std::random_device{}()) {}
    ~Treap() { destroy(root); }

    bool contains(int key) const { return contains(root, key); }

    void insert(int key) {
        if (contains(key)) return;
        std::uniform_int_distribution<uint32_t> dist;
        uint32_t pr = dist(rng);
        root = insert(root, key, pr);
    }

    void erase(int key) {
        root = erase(root, key);
    }

    void printStructure() const {
        if (!root) {
            std::cout << "(empty)\n";
            return;
        }
        printStructure(root, "", true);
    }
};

// Example usage
int main() {
    Treap t;
    for (int x : {10,5,15,2,7,12,18,6}) t.insert(x);

    std::cout << "\nStructure after inserts:\n";
    t.printStructure();

    t.erase(10);

    std::cout << "\nStructure after erase(10):\n";
    t.printStructure();

    return 0;
}
