#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitree.h"

// Function to create a new node and insert it into the binary tree
void insertNode(BiTree *tree, char *data) {
    BiTree_insertNode(tree->root, data);
}

// Function to delete a node from the binary tree
void deleteNode(BiTree *tree, char *data) {
    BiTree_deleteNode(tree->root, data);
}

// Function to create a new binary tree node
void createNode(BiTree *tree, char *data) {
    tree->root = BiTree_createNode(data);
}

// Function to perform breadth-first search (BFS) traversal and serialize the tree
void BFSAndSerialize(BiTree *tree, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file for serialization\n");
        return;
    }
    BiTree_serialize(fp, tree->root, "bfs");
    fclose(fp);
}

void printUsage() {
    printf("Usage: ./btree [options]\n");
    printf("Options:\n");
    printf("  --insert, -i <data>: Insert a node into the binary tree\n");
    printf("  --delete, -d <data>: Delete a node from the binary tree\n");
    printf("  --create, -c <data>: Create a new binary tree with a root node\n");
    printf("  --bfs, -b <filename>: Perform BFS traversal and serialize the tree to a file\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    BiTree tree;
    BiTree_new(&tree);

    if (strcmp(argv[1], "--create") == 0 || strcmp(argv[1], "-c") == 0) {
        if (argc != 3) {
            printf("Invalid arguments. Usage: ./btree --create <root_data>\n");
            return 1;
        }
        char *data = argv[2];
        createNode(&tree, data);
    } else if (strcmp(argv[1], "--insert") == 0 || strcmp(argv[1], "-i") == 0) {
        if (argc != 3) {
            printf("Invalid arguments. Usage: ./btree --insert <data>\n");
            return 1;
        }
        char *data = argv[2];
        insertNode(&tree, data);
    } else if (strcmp(argv[1], "--bfs") == 0 || strcmp(argv[1], "-b") == 0) {
        if (argc != 3) {
            printf("Invalid arguments. Usage: ./btree --bfs <filename>\n");
            return 1;
        }
        char *filename = argv[2];
        BFSAndSerialize(&tree, filename);
    } else {
        printf("Unknown option: %s\n", argv[1]);
        printUsage();
        return 1;
    }

    BiTree_destroy(&tree);

    return 0;
}
