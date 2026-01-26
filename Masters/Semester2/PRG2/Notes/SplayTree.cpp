#include <iostream>
#include <utility>
#include <string>
#include <queue>

class SplayTree {
private:
    struct Node {
        int key;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
        explicit Node(int k) : key(k) {}
    };

    Node* root = nullptr;

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    void rotateLeft(Node* x) {
        // x must be right child of its parent p
        Node* p = x->parent;
        if (!p) return;
        Node* g = p->parent;

        p->right = x->left;
        if (x->left) x->left->parent = p;

        x->left = p;
        p->parent = x;

        x->parent = g;
        if (!g) {
            root = x;
        } else if (g->left == p) {
            g->left = x;
        } else {
            g->right = x;
        }
    }

    void rotateRight(Node* x) {
        // x must be left child of its parent p
        Node* p = x->parent;
        if (!p) return;
        Node* g = p->parent;

        p->left = x->right;
        if (x->right) x->right->parent = p;

        x->right = p;
        p->parent = x;

        x->parent = g;
        if (!g) {
            root = x;
        } else if (g->left == p) {
            g->left = x;
        } else {
            g->right = x;
        }
    }

    void splay(Node* x) {
        if (!x) return;
        while (x->parent) {
            Node* p = x->parent;
            Node* g = p->parent;

            if (!g) {
                // Zig
                if (p->left == x) rotateRight(x);
                else rotateLeft(x);
            } else if (g->left == p && p->left == x) {
                // Zig-Zig (left-left)
                rotateRight(p);
                rotateRight(x);
            } else if (g->right == p && p->right == x) {
                // Zig-Zig (right-right)
                rotateLeft(p);
                rotateLeft(x);
            } else if (g->left == p && p->right == x) {
                // Zig-Zag (left-right)
                rotateLeft(x);
                rotateRight(x);
            } else {
                // Zig-Zag (right-left)
                rotateRight(x);
                rotateLeft(x);
            }
        }
        root = x;
    }

    Node* subtreeMax(Node* n) const {
        while (n && n->right) n = n->right;
        return n;
    }

    Node* bstFindNode(int key) {
        Node* cur = root;
        Node* last = nullptr;
        while (cur) {
            last = cur;
            if (key < cur->key) cur = cur->left;
            else if (key > cur->key) cur = cur->right;
            else break;
        }
        // Splay the found node or last accessed node
        if (cur) splay(cur);
        else if (last) splay(last);
        return cur;
    }

    void printStructure(Node* n, const std::string& prefix, bool isLeft) const {
        if (!n) return;

        // Print right subtree first (appears on top)
        printStructure(n->right, prefix + (isLeft ? "│   " : "    "), false);

        // Print this node
        std::cout << prefix
                  << (isLeft ? "└── " : "┌── ")
                  << n->key;

        std::cout << "\n";

        // Print left subtree (appears at bottom)
        printStructure(n->left, prefix + (isLeft ? "    " : "│   "), true);
    }

public:
    SplayTree() = default;
    ~SplayTree() { destroy(root); }

    bool contains(int key) {
        return bstFindNode(key) != nullptr;
    }

    void insert(int key) {
        if (!root) {
            root = new Node(key);
            return;
        }
        Node* cur = root;
        Node* p = nullptr;
        while (cur) {
            p = cur;
            if (key < cur->key) cur = cur->left;
            else if (key > cur->key) cur = cur->right;
            else {
                splay(cur);
                return;
            }
        }
        Node* n = new Node(key);
        n->parent = p;
        if (key < p->key) p->left = n;
        else p->right = n;
        splay(n);
    }

    void erase(int key) {
        Node* x = bstFindNode(key);
        if (!x) return;

        Node* L = x->left;
        Node* R = x->right;
        if (L) L->parent = nullptr;
        if (R) R->parent = nullptr;

        delete x;
        root = nullptr;

        if (!L) {
            root = R;
            return;
        }

        root = L;
        Node* m = subtreeMax(root);
        splay(m);
        root->right = R;
        if (R) R->parent = root;
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
    SplayTree st;
    for (int x : {10,5,15,2,7,12,18,6})
        st.insert(x);

    std::cout << "\nStructure after inserts:\n";
    st.printStructure();

    st.contains(12); // splays 12 to root
    std::cout << "\nStructure after contains(12) (splay):\n";
    st.printStructure();

    st.erase(10);
    std::cout << "\nStructure after erase(10):\n";
    st.printStructure();

    return 0;
}
