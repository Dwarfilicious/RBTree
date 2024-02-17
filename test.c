/* Author: Bas Zoeteman
 * Date of Creation: 23/12/2023
 * Code to test the validity of the red-black tree properties. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "RBTree.h"

#define MAX 1000000

/* Tests initialization and freeing of the red-black tree. */
int initializationTest(void) {
    printf("Testing initialization and freeing of red-black tree: ");

    struct RBTree *tree = RBCreate();
    if (!tree) {
        printf("Failed to create tree.\n");
        return -1;
    }

    RBFree(tree);
    printf("Success.\n");
    return 0;
}

/* Tests simple ordered insertions without taking into account duplicates. */
int insertTest(void) {
    printf("Testing simple ordered insertions: ");

    struct RBTree *tree = RBCreate();
    if (!tree) {
        printf("Failed to create tree.\n");
        return -1;
    }

    for (int i = 0; i < 10; i++) {
        if (RBInsert(tree, i) == -1) {
            printf("Failed to insert value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        return -1;
    }

    RBFree(tree);
    printf("Success.\n");
    return 0;
}

/* Tests whether duplicate inserts are handled correctly. */
int duplicateTest(void) {
    printf("Testing duplicate insert detection: ");

    struct RBTree *tree = RBCreate();
    if (!tree) {
        printf("Failed to create tree.\n");
        return -1;
    }

    for (int i = 0; i < 10; i++) {
        if (RBInsert(tree, i) == -1) {
            printf("Failed to insert value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (RBInsert(tree, i) != 1) {
            printf("Failed to detect duplicate %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        return -1;
    }

    RBFree(tree);
    printf("Success.\n");
    return 0;
}

/* Tests whether the searches correctly find added values and
 * whether they do not find non-present values. */
int searchTest(void) {
    printf("Testing search method: ");

    struct RBTree *tree = RBCreate();
    if (!tree) {
        printf("Failed to create tree.\n");
        return -1;
    }

    for (int i = 0; i < 10; i++) {
        if (RBInsert(tree, i) == -1) {
            printf("Failed to insert value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (RBSearch(tree, i) == 0) {
            printf("Failed to find value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    for (int i = 10; i < 20; i++) {
        if (RBSearch(tree, i) == 1) {
            printf("Found non-present value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        return -1;
    }

    RBFree(tree);
    printf("Success.\n");
    return 0;
}

/* Tests the deletion of values without taking into account non-present values. */
int deleteTest(void) {
    printf("Testing simple ordered deletions: ");

    struct RBTree *tree = RBCreate();
    if (!tree) {
        printf("Failed to create tree.\n");
        return -1;
    }

    for (int i = 0; i < 10; i++) {
        if (RBInsert(tree, i) == -1) {
            printf("Failed to insert value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (RBDelete(tree, i) == -1) {
            printf("Failed to delete value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        return -1;
    }

    RBFree(tree);
    printf("Success.\n");
    return 0;
}

/* Tests whether the deletion correctly handles non-present values. */
int deleteNonPresentTest(void) {
    printf("Testing deletion of non-present values: ");

    struct RBTree *tree = RBCreate();
    if (!tree) {
        printf("Failed to create tree.\n");
        return -1;
    }

    for (int i = 0; i < 10; i++) {
        if (RBInsert(tree, i) == -1) {
            printf("Failed to insert value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    for (int i = 10; i < 20; i++) {
        if (RBDelete(tree, i) != 1) {
            printf("Attempted to delete non-present value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        return -1;
    }

    RBFree(tree);
    printf("Success.\n");
    return 0;
}

/* Tests the red-black tree structure with many ordered values.
 * Returns 0 on success, a printed error and -1 on failure. */
int manyOrderedValuesTest(void) {
    printf("Testing red-black tree with %d ordered values: ", MAX);

    struct RBTree *tree = RBCreate();
    if (!tree) {
        printf("Failed to create tree.\n");
        return -1;
    }

    int i;
    for (i = 0; i < MAX; i++) {
        if (RBInsert(tree, i) == -1) {
            printf("Failed to insert value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        return -1;
    }

    for (i = 0; i < MAX; i++) {
        if (RBSearch(tree, i) == 0) {
            printf("Failed to find value %d.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    for (i = 0; i < 0.5*MAX; i++) {
        int deleteErrorCode = RBDelete(tree, i);
        if (deleteErrorCode == -1) {
            printf("Failed to delete value %d.\n", i);
            RBFree(tree);
            return -1;
        } else if (deleteErrorCode == 1) {
            printf("Value %d could not be found in the tree.\n", i);
            RBFree(tree);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        return -1;
    }

    RBFree(tree);
    printf("Success.\n");
    return 0;
}

/* Tests the red-black tree structure with many random values.
 * Returns 0 on success, a printed error and -1 on failure. */
int manyRandomValuesTest(void) {
    printf("Testing red-black tree with %d random values: ", MAX);

    struct RBTree *tree = RBCreate();
    if (!tree) {
        printf("Failed to create tree.\n");
        return -1;
    }

    srand((unsigned int)time(NULL));

    int i;
    int *values = malloc(sizeof(int) * MAX);
    for (i = 0; i < MAX; i++) {
        int value = rand() % MAX;
        values[i] = value;
    }

    for (i = 0; i < MAX; i++) {
        if (RBInsert(tree, values[i]) == -1) {
            printf("Failed to insert value %d.\n", values[i]);
            RBFree(tree);
            free(values);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        free(values);
        return -1;
    }

    for (i = 0; i < MAX; i++) {
        if (RBSearch(tree, values[i]) == 0) {
            printf("Failed to find value %d.\n", values[i]);
            RBFree(tree);
            free(values);
            return -1;
        }
    }

    for (i = 0; i < 0.5*MAX; i++) {
        int deleteErrorCode = RBDelete(tree, values[i]);
        if (deleteErrorCode == -1) {
            printf("Failed to delete value %d.\n", values[i]);
            RBFree(tree);
            return -1;
        }
    }

    if (RBCheck(tree) == -1) {
        printf("Tree is not a valid red-black tree.\n");
        RBFree(tree);
        free(values);
        return -1;
    }

    RBFree(tree);
    free(values);
    printf("Success.\n");
    return 0;
}

int main(void) {
    if (initializationTest()) {
        return -1;
    }
    if (insertTest()) {
        return -1;
    }
    if (duplicateTest()) {
        return -1;
    }
    if (searchTest()) {
        return -1;
    }
    if (deleteTest()) {
        return -1;
    }
    if (deleteNonPresentTest()) {
        return -1;
    }
    if (manyOrderedValuesTest()) {
        return -1;
    }
    if (manyRandomValuesTest()) {
        return -1;
    }

    printf("All tests succeeded.\n");

    return 0;
}
