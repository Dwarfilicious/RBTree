#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RBTree.h"

typedef enum {BLACK, RED} Color;

struct RBTree {
    struct RBNode *root;
};

struct RBNode {
    int value;
    Color color;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
};

/* Helper function: return a pointer to made node on success,
 * NULL on failure. */
struct RBNode *makeNode(int value) {
    struct RBNode *n = malloc(sizeof(struct RBNode));
    if (!n) {
        return NULL;
    }

    n->value = value;
    n->color = RED;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;

    return n;
}

struct RBTree *RBCreate(void) {
    struct RBTree *tree = malloc(sizeof(struct RBTree));
    if (!tree) {
        return NULL;
    }

    tree->root = NULL;

    return tree;
}

/* Helper function: returns the root of the input (sub)tree
 * after unbalanced tree insertion. */
struct RBNode *nodeInsert(struct RBNode *root, struct RBNode *newNode, int *duplicateFlag) {
    if (!root) {
        return newNode;
    }

    if (newNode->value < root->value) {
        root->left = nodeInsert(root->left, newNode, duplicateFlag);
        if (root->left) {
            root->left->parent = root;
        }
    } else if (newNode->value > root->value) {
        root->right = nodeInsert(root->right, newNode, duplicateFlag);
        if (root->right) {
            root->right->parent = root;
        }
    } else {
        *duplicateFlag = 1;
    }

    return root;
}

/* Helper function */
void leftRotate(struct RBTree *tree, struct RBNode *node) {
    if (!tree || !node) {
        return;
    }

    struct RBNode *right = node->right;
    node->right = right->left;
    if (right->left) {
        right->left->parent = node;
    }

    right->parent = node->parent;
    if (!node->parent) {
        tree->root = right;
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }

    right->left = node;
    node->parent = right;
}

/* Helper function */
void rightRotate(struct RBTree *tree, struct RBNode *node) {
    if (!tree || !node) {
        return;
    }

    struct RBNode *left = node->left;
    node->left = left->right;
    if (left->right) {
        left->right->parent = node;
    }

    left->parent = node->parent;
    if (!node->parent) {
        tree->root = left;
    } else if (node == node->parent->left) {
        node->parent->left = left;
    } else {
        node->parent->right = left;
    }

    left->right = node;
    node->parent = left;
}

/* Helper function: catches the simple cases where no fix or barely any fix
 * needs to be made. */
int catchSimpleCases(struct RBTree *tree, struct RBNode *node) {
    if (!tree || !node) {
        return 1;
    }
    if (node->color == BLACK) {
        return 1;
    }
    if (!node->parent) {
        node->color = BLACK;
        return 1;
    }
    if (node->parent->color == BLACK) {
        return 1;
    }

    return 0;
}

/* Helper function */
void uncleRedCaseColorSwap(struct RBNode *parent, struct RBNode *uncle,
                           struct RBNode *grandparent) {
    parent->color = BLACK;
    uncle->color = BLACK;
    grandparent->color = RED;
}

/* Helper function */
void lineCaseColorSwap(struct RBNode *parent, struct RBNode *grandparent) {
    Color temp = parent->color;
    parent->color = grandparent->color;
    grandparent->color = temp;
}

/* Helper function: restores red-black properties after insertion. */
void insertFixup(struct RBTree *tree, struct RBNode *node) {
    if (catchSimpleCases(tree, node)) {
        return;
    }

    struct RBNode *parent = node->parent;
    struct RBNode *grandparent = parent->parent;
    struct RBNode *uncle;
    if (grandparent->left == parent) {
        uncle = grandparent->right;
    } else {
        uncle = grandparent->left;
    }

    if (uncle && uncle->color == RED) {
        uncleRedCaseColorSwap(parent, uncle, grandparent);
        insertFixup(tree, grandparent);
        return;
    }

    // triangle case
    if (node == parent->left && parent == grandparent->right) {
        rightRotate(tree, parent);
        node = parent;
        parent = node->parent;
    } else if (node == parent->right && parent == grandparent->left) {
        leftRotate(tree, parent);
        node = parent;
        parent = node->parent;
    }

    // line case (always follows triangle case)
    if (node == parent->left && parent == grandparent->left) {
        rightRotate(tree, grandparent);
        lineCaseColorSwap(parent, grandparent);
        insertFixup(tree, parent);
    } else if (node == parent->right && parent == grandparent->right) {
        leftRotate(tree, grandparent);
        lineCaseColorSwap(parent, grandparent);
        insertFixup(tree, parent);
    }
}

int RBInsert(struct RBTree *tree, int value) {
    if (!tree) {
        return -1;
    }

    int duplicateFlag = 0;
    struct RBNode *newNode = makeNode(value);
    tree->root = nodeInsert(tree->root, newNode, &duplicateFlag);
    insertFixup(tree, newNode);

    if (duplicateFlag) {
        free(newNode);
        return 1;
    } else {
        return 0;
    }
}

/* Helper function: finds and returns the node with node->value == value. */
struct RBNode *nodeSearch(struct RBNode *node, int value) {
    if (!node) {
        return NULL;
    }

    if (node->value == value) {
        return node;
    } else if (value < node->value) {
        return nodeSearch(node->left, value);
    } else {
        return nodeSearch(node->right, value);
    }
}

int RBSearch(struct RBTree *tree, int value) {
    if (!tree) {
        return 0;
    }

    if (nodeSearch(tree->root, value)) {
        return 1;
    } else {
        return 0;
    }
}

/* Helper function */
void leafDelete(struct RBTree *tree, struct RBNode *node) {
    if (!tree || !node) {
        return;
    }

    if (!node->parent) {
        tree->root = NULL;
        free(node);
        return;
    }

    if (node == node->parent->left) {
        node->parent->left = NULL;
    } else {
        node->parent->right = NULL;
    }

    free(node);
}

/* Helper function: returns the predecessor of an input node. */
struct RBNode *findPredecessor(struct RBNode *node) {
    if (!node) {
        return NULL;
    }

    struct RBNode *predecessor = node->left;
    while (predecessor->right) {
        predecessor = predecessor->right;
    }

    return predecessor;
}

/* Helper function: returns the successor of an input node. */
struct RBNode *findSuccessor(struct RBNode *node) {
    if (!node) {
        return NULL;
    }

    struct RBNode *successor = node->right;
    while (successor->left) {
        successor = successor->left;
    }

    return successor;
}

/* Helper function: recursively moves the to be deleted value down to a leaf
 * by moving up the values of suitable predecessors and successors.
 * Returns the to be deleted leaf node. */
struct RBNode *recursiveDelete(struct RBNode *node) {
    if (!node) {
        return NULL;
    }

    if (!node->left && !node->right) {
        return node;
    } else if (!node->right) {
        struct RBNode *predecessor = findPredecessor(node);
        node->value = predecessor->value;
        return recursiveDelete(predecessor);
    } else {
        struct RBNode *successor = findSuccessor(node);
        node->value = successor->value;
        return recursiveDelete(successor);
    }
}

/* Helper function: returns the sibling of the input node. */
struct RBNode *findSibling(struct RBNode *node) {
    if (!node || !node->parent) {
        return NULL;
    }

    if (node == node->parent->left) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

/* Helper function: returns the case code of the relevant case in deleteFixup.
 * Format of the case code is
 * R for red sibling,
 * RB for near child being red and far child being black,
 * BR for far child being red,
 * BB for two black children. */
typedef enum {R, BB, RB, BR} CaseCode;
CaseCode findCaseCode(struct RBNode *sibling) {
    int isSiblingLeftChild;
    if (sibling == sibling->parent->left) {
        isSiblingLeftChild = 1;
    } else {
        isSiblingLeftChild = 0;
    }

    if (sibling->color == RED) {
        return R;
    }

    struct RBNode *nearChild = isSiblingLeftChild ? sibling->right : sibling->left;
    struct RBNode *farChild = isSiblingLeftChild ? sibling->left : sibling->right;
    if (nearChild && nearChild->color == RED) {
        return RB;
    }
    if (farChild && farChild->color == RED) {
        return BR;
    }

    return BB;
}

/* Helper function */
void siblingRedCase(struct RBTree *tree, struct RBNode *node, struct RBNode *sibling) {
    sibling->color = BLACK;
    node->parent->color = RED;
    if (node == node->parent->left) {
        leftRotate(tree, node->parent);
    } else {
        rightRotate(tree, node->parent);
    }
}

/* Helper function */
Color siblingBlackBlackChildrenCase(struct RBNode *node, struct RBNode *sibling) {
    Color originalColor = node->parent->color;
    node->parent->color = BLACK;
    sibling->color = RED;
    return originalColor;
}

/* Helper function */
void siblingBlackNearChildRedCase(struct RBTree *tree, struct RBNode *node,
                                  struct RBNode *sibling) {
    if (node == node->parent->left) {
        sibling->left->color = BLACK;
        sibling->color = RED;
        rightRotate(tree, sibling);
    } else {
        sibling->right->color = BLACK;
        sibling->color = RED;
        leftRotate(tree, sibling);
    }
}

/* Helper function */
void siblingBlackFarChildRedCase(struct RBTree *tree, struct RBNode *node,
                                 struct RBNode *sibling) {
    sibling->color = node->parent->color;
    node->parent->color = BLACK;
    if (node == node->parent->left) {
        sibling->right->color = BLACK;
        leftRotate(tree, node->parent);
    } else {
        sibling->left->color = BLACK;
        rightRotate(tree, node->parent);
    }
}

/* Helper function: restores red-black properties after deletion. */
void deleteFixup(struct RBTree *tree, struct RBNode *node) {
    if (!tree || !node) {
        return;
    }

    if (node->color == RED) {
        return;
    }

    struct RBNode *sibling = findSibling(node);
    if (!sibling) {
        return;
    }

    CaseCode caseCode = findCaseCode(sibling);
    switch (caseCode) {
        case R:
            siblingRedCase(tree, node, sibling);
            deleteFixup(tree, node);
            break;
        case BB:
            if (siblingBlackBlackChildrenCase(node, sibling) == BLACK) {
                deleteFixup(tree, node->parent);
            }
            break;
        case RB:
            siblingBlackNearChildRedCase(tree, node, sibling);
            deleteFixup(tree, node);
            break;
        case BR:
            siblingBlackFarChildRedCase(tree, node, sibling);
            break;
    }
}

int RBDelete(struct RBTree *tree, int value) {
    if (!tree) {
        return -1;
    }

    struct RBNode *toRemoveNode = nodeSearch(tree->root, value);
    if (!toRemoveNode) {
        return 1;
    }

    toRemoveNode = recursiveDelete(toRemoveNode);
    deleteFixup(tree, toRemoveNode);
    leafDelete(tree, toRemoveNode);

    return 0;
}

/* Helper function */
void nodePrint(struct RBNode *node) {
    if (!node) {
        return;
    }

    nodePrint(node->left);
    printf("%d\n", node->value);
    nodePrint(node->right);
}

void RBPrint(struct RBTree *tree) {
    if (!tree) {
        return;
    }

    nodePrint(tree->root);

    return;
}

/* Helper function */
int isBST(struct RBNode *root, struct RBNode *minNode, struct RBNode *maxNode) {
    if (!root) {
        return 1;
    }

    if (minNode && root->value < minNode->value) {
        return 0;
    }
    if (maxNode && root->value > maxNode->value) {
        return 0;
    }

    return isBST(root->left, minNode, root) && isBST(root->right, root, maxNode);
}

/* Helper function: returns -1 if double red property is violated,
 * 0 otherwise. */
int doubleRedCheck(struct RBNode *node) {
    if (!node) {
        return 0;
    }

    if (node->color == RED && node->parent && node->parent->color == RED) {
        return -1;
    }

    return doubleRedCheck(node->left) && doubleRedCheck(node->right);
}

/* Helper function: returns black depth of subtree rooted at node
 * or -1 if red-black property is violated. */
int blackDepthCheck(struct RBNode *node) {
    if (!node) {
        return 1;
    }

    int left = blackDepthCheck(node->left);
    int right = blackDepthCheck(node->right);

    if (left == -1 || right == -1) {
        return -1;
    }

    if (left != right) {
        return -1;
    }

    if (node->color == BLACK) {
        return ++left;
    } else {
        return left;
    }
}

int RBCheck(struct RBTree *tree) {
    if (!tree) {
        return -1;
    }

    if (!tree->root) {
        return 0;
    }

    if (!isBST(tree->root, NULL, NULL)) {
        return -1;
    }

    if (tree->root->color != BLACK) {
        return -1;
    }
    if (doubleRedCheck(tree->root) == -1) {
        return -1;
    }
    if (blackDepthCheck(tree->root) == -1) {
        return -1;
    }

    return 0;
}

/* Helper function: calls itself on all subsequent nodes recursively
 * and frees node. */
void nodeFree(struct RBNode *node) {
    if (!node) {
        return;
    }

    nodeFree(node->left);
    nodeFree(node->right);

    free(node);
}

void RBFree(struct RBTree *tree) {
    if (!tree) {
        return;
    }

    nodeFree(tree->root);
    free(tree);
}
