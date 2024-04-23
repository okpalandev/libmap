#ifndef BITREE_H
#define BITREE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct BiTreeNode BiTreeNode;
struct BiTreeNode {
    int key;
    char *data;
    BiTreeNode *left;
    BiTreeNode *right;
};

typedef struct BiTree BiTree;

struct BiTree {
    BiTreeNode *root;
    size_t node_count;
};

BiTree* BiTree_new(const char* root_data);
BiTree* BiTree_bfs(BiTreeNode *root, char *data);
BiTree* BiTree_dfs(BiTreeNode *root, char *data, char *type);
BiTreeNode* BiTree_createNode(const char* data);
BiTreeNode* BiTree_insertNode(BiTreeNode *current,const char *data);
BiTreeNode* BiTree_deleteNode(BiTreeNode* root, const char* key);
BiTreeNode* BiTree_minValueNode(BiTreeNode* node);
BiTreeNode* BiTree_maxValueNode(BiTreeNode* node);

BiTreeNode* BiTree_reorder(BiTreeNode *root, char *type);
BiTree* BiTree_search(BiTreeNode *root, char *data, char *type);

bool BiTree_isfull(BiTreeNode *root);
bool BiTree_iscomplete(BiTreeNode *root);

int BiTree_depth(BiTreeNode* root);

void BiTree_serialize(FILE *fp, BiTreeNode* root, const char* algo);
BiTreeNode* BiTree_deserialize(FILE *fp);

void BiTree_destroy(BiTree* tree);
void BiTree_free(BiTreeNode *root);
#endif // BITREE_H
