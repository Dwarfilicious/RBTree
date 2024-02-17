/* Author: Bas Zoeteman
 * Date of Creation: 23/12/2023
 * Header file for a red-black binary search tree datastructure.
 * Structure is naturally ordered, i.e. min to max order.
 * Structure can only contain unique values of type int. */

#ifndef RBTREE_H
#define RBTREE_H

struct RBTree;

/* Create a new red-black tree, return a pointer to the tree
 * on success, NULL on failure. */
struct RBTree *RBCreate(void);

/* Insert a value into the tree, return 0 on success, -1 on failure.
 * If the data is already present in the tree, leave the tree unchanged
 * and return 1. */
int RBInsert(struct RBTree *tree, int value);

/* Search for a value in the tree, return 1 when the value is present
 * or 0 when the value is not found. */
int RBSearch(struct RBTree *tree, int value);

/* Delete a value from the tree, return 0 on success, -1 on failure.
 * If the data is not present in the tree, leave the tree unchanged
 * and return 1. */
int RBDelete(struct RBTree *tree, int value);

/* Print the tree in order, return 0 on success, -1 on failure. */
void RBPrint(struct RBTree *tree);

/* Check if the tree is a valid red-black tree, return 0 on success,
 * -1 on failure. */
int RBCheck(struct RBTree *tree);

/* Free the tree and all of its nodes. */
void RBFree(struct RBTree *tree);

#endif /* RBTREE_H */
