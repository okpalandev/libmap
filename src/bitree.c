#include "bitree.h"

BiTree* BiTree_new(const char* root_data) {
  // Allocate memory for the tree
  BiTree *tree = malloc(sizeof(BiTree));
  if (tree == NULL) {
    fprintf(stderr, "%s\n", "Failed to create a new BiTree");
    return NULL;
  }

  // Allocate memory for the root node
  BiTreeNode *root = malloc(sizeof(BiTreeNode));
  if (root == NULL) {
    fprintf(stderr, "%s\n", "Failed to create a node for BiTree");
    free(tree); // Free previously allocated memory for tree
    return NULL;
  }

  // Initialize the root node
  root->key = 0;  // Assuming initial key/index is 0
  root->data = strdup(root_data); // Duplicate the string for data
  root->left = NULL;
  root->right = NULL;

  // Set the root of the tree
  tree->root = root;
  tree->node_count = 1;

  return tree;
};

BiTreeNode* BiTree_insertNode(BiTreeNode *current, const char *data) {
    // If the tree is empty, create a new node as the root
    if (current == NULL) {
        return BiTree_createNode(data);
    }

    // If the data is less than the current node's data, go left
    if (strcmp(data, current->data) < 0) {
        current->left = BiTree_insertNode(current->left, data);
    }
    // If the data is greater than the current node's data, go right
    else if (strcmp(data, current->data) > 0) {
        current->right = BiTree_insertNode(current->right, data);
    }

    // Return the unchanged node pointer
    return current;
}

BiTreeNode* BiTree_deleteNode(BiTreeNode* root, const char* key) {
    // Base case: If the tree is empty, return NULL
    if (root == NULL) {
        return NULL;
    }

    // Traverse the tree to find the node with the specified key
    if (strcmp(root->data, key) > 0) {
        root->left = BiTree_deleteNode(root->left, key);
    } else if (strcmp(root->data, key) < 0) {
        root->right = BiTree_deleteNode(root->right, key);
    } else {
        // Case 1: Node to be deleted has no children
        if (root->left == NULL && root->right == NULL) {
            free(root->data);
            free(root);
            root = NULL;
        }
        // Case 2: Node to be deleted has one child
        else if (root->left == NULL) {
            BiTreeNode* temp = root;
            root = root->right;
            free(temp->data);
            free(temp);
        } else if (root->right == NULL) {
            BiTreeNode* temp = root;
            root = root->left;
            free(temp->data);
            free(temp);
        }
        // Case 3: Node to be deleted has two children
        else {
            // Find the inorder successor (smallest node in the right subtree)
            BiTreeNode* temp = BiTree_minValueNode(root->right);

            // Copy the inorder successor's data to this node
            strcpy(root->data, temp->data);

            // Delete the inorder successor
            root->right = BiTree_deleteNode(root->right, temp->data);
        }
    }
    return root;
}


BiTreeNode* BiTree_minValueNode(BiTreeNode* node) {
    BiTreeNode* current = node;
    // Loop down to find the leftmost leaf
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

BiTreeNode* BiTree_maxValueNode(BiTreeNode* node) {
    BiTreeNode* current = node;
    // Loop down to find the rightmost leaf
    while (current->right != NULL) {
        current = current->right;
    }
    return current;
}

// Function to create a new binary tree node
BiTreeNode* BiTree_createNode(const char* data) {
    BiTreeNode* newNode = (BiTreeNode*)malloc(sizeof(BiTreeNode));
    newNode->data = strdup(data); // Duplicate the data string
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to perform breadth-first search (BFS) traversal
// Returns a BiTree* containing the subtree where the data is found, or NULL if not found
BiTree* BiTree_bfs(BiTreeNode *root, char *data) {
    if (root == NULL) {
        return NULL;
    }

    // Allocate memory for the array to store nodes at each level
    BiTreeNode **level_nodes = (BiTreeNode **)malloc(sizeof(BiTreeNode *));
    if (level_nodes == NULL) {
        fprintf(stderr, "Memory allocation failed for level nodes.\n");
        exit(EXIT_FAILURE);
    }

    level_nodes[0] = root; // Initialize the first level with the root node
    int level_count = 1; // Number of nodes at the current level

    while (level_count > 0) {
        BiTreeNode *found_node = NULL; // Pointer to the node where data is found

        // Allocate memory for the array to store nodes at the next level
        BiTreeNode **next_level_nodes = NULL;
        int next_level_count = 0; // Number of nodes at the next level

        for (int i = 0; i < level_count; i++) {
            BiTreeNode *current = level_nodes[i];

            // Check if the current node matches the specified data
            if (strcmp(current->data, data) == 0) {
                found_node = current;
                break;
            }

            // Add left child to the next level array if it exists
            if (current->left != NULL) {
                next_level_nodes = (BiTreeNode **)realloc(next_level_nodes, (next_level_count + 1) * sizeof(BiTreeNode *));
                if (next_level_nodes == NULL) {
                    fprintf(stderr, "Memory reallocation failed for next level nodes.\n");
                    exit(EXIT_FAILURE);
                }
                next_level_nodes[next_level_count++] = current->left;
            }

            // Add right child to the next level array if it exists
            if (current->right != NULL) {
                next_level_nodes = (BiTreeNode **)realloc(next_level_nodes, (next_level_count + 1) * sizeof(BiTreeNode *));
                if (next_level_nodes == NULL) {
                    fprintf(stderr, "Memory reallocation failed for next level nodes.\n");
                    exit(EXIT_FAILURE);
                }
                next_level_nodes[next_level_count++] = current->right;
            }
        }

        // Free the memory allocated for the previous level nodes array
        free(level_nodes);

        // If data is found at current level, create a subtree and return it
        if (found_node != NULL) {
            BiTree *subtree = (BiTree *)malloc(sizeof(BiTree));
            if (subtree == NULL) {
                fprintf(stderr, "Memory allocation failed for subtree.\n");
                exit(EXIT_FAILURE);
            }
            subtree->root = found_node; // Set the root of the subtree
            subtree->node_count = 1; // Initialize node count
            return subtree;
        }

        // Move to the next level
        level_nodes = next_level_nodes;
        level_count = next_level_count;
    }

    free(level_nodes); // Free the memory allocated for the last level nodes array
    return NULL; // Data not found, return NULL
}

// Function to perform depth-first search (DFS) traversal using a stack
BiTree* BiTree_dfs(BiTreeNode *root, char *data, char *type) {
    if (root == NULL) {
        return NULL;  // Base case: If the root is NULL, return NULL
    }

    // Set default traversal type to "preorder" if not provided
    if (type == NULL) {
        type = "preorder";
    }

    // Create an array to act as a stack for DFS traversal
    BiTreeNode **stack = (BiTreeNode **)malloc(sizeof(BiTreeNode *));
    if (stack == NULL) {
        fprintf(stderr, "Memory allocation failed for stack.\n");
        exit(EXIT_FAILURE);
    }
    int top = -1; // Initialize top of stack

    // Push the root node onto the stack
    stack[++top] = root;

    while (top >= 0) {
        // Pop the top node from the stack
        BiTreeNode *current = stack[top--];

        // Check if the current node matches the specified data
        if (strcmp(current->data, data) == 0) {
            free(stack); // Free the memory allocated for the stack
            // Create a new BiTree* containing the subtree where the data is found
            BiTree *subtree = (BiTree *)malloc(sizeof(BiTree));
            if (subtree == NULL) {
                fprintf(stderr, "Memory allocation failed for subtree.\n");
                exit(EXIT_FAILURE);
            }
            subtree->root = current; // Set the root of the subtree
            subtree->node_count = 1; // Initialize node count
            return subtree;
        }

        // Push the right child onto the stack if it exists
        if (current->right != NULL) {
            stack = (BiTreeNode **)realloc(stack, (top + 2) * sizeof(BiTreeNode *));
            if (stack == NULL) {
                fprintf(stderr, "Memory reallocation failed for stack.\n");
                exit(EXIT_FAILURE);
            }
            stack[++top] = current->right;
        }

        // Push the left child onto the stack if it exists
        if (current->left != NULL) {
            stack = (BiTreeNode **)realloc(stack, (top + 2) * sizeof(BiTreeNode *));
            if (stack == NULL) {
                fprintf(stderr, "Memory reallocation failed for stack.\n");
                exit(EXIT_FAILURE);
            }
            stack[++top] = current->left;
        }
    }

    free(stack); // Free the memory allocated for the stack
    return NULL; // Data not found, return NULL
}




// Function to reorder the binary tree based on the specified traversal type
BiTreeNode* BiTree_reorder(BiTreeNode* root, char* type) {
    if (root == NULL || type == NULL) {
        return NULL; // Return NULL for invalid input
    }

    // Determine the traversal type
    if (strcmp(type, "preorder") == 0) {
        // No change needed for preorder traversal
        return root; // Return the original root
    } else if (strcmp(type, "inorder") == 0) {
        // Traverse the tree in inorder using BiTree_dfs
        BiTree* subtree = BiTree_dfs(root, NULL, "inorder");
        if (subtree != NULL) {
            return subtree->root; // Return the root of the subtree
        } else {
            return NULL; // Return NULL if traversal fails
        }
    } else if (strcmp(type, "postorder") == 0) {
        // Traverse the tree in postorder using BiTree_dfs
        BiTree* subtree = BiTree_dfs(root, NULL, "postorder");
        if (subtree != NULL) {
            return subtree->root; // Return the root of the subtree
        } else {
            return NULL; // Return NULL if traversal fails
        }
    } else {
        // Unsupported traversal type
        fprintf(stderr, "Unsupported traversal type: %s\n", type);
        return NULL; // Return NULL for unsupported traversal type
    }
}


// Function to search for a node in a binary tree based on the specified traversal type
BiTree* BiTree_search(BiTreeNode *root, char *data, char *type) {
    if (root == NULL || data == NULL || type == NULL) {
        return NULL; // Return NULL for invalid input
    }

    // Determine the traversal type
    if (strcmp(type, "bfs") == 0) {
        // Perform BFS traversal
        return BiTree_bfs(root, data);
    } else if (strcmp(type, "dfs") == 0) {
        // Perform DFS traversal
        return BiTree_dfs(root, data, NULL); // Pass NULL for type parameter
    } else {
        // Unsupported traversal type
        fprintf(stderr, "Unsupported traversal type: %s\n", type);
        return NULL; // Return NULL for unsupported traversal type
    }
}


// Function to calculate the depth of a binary tree
int BiTree_depth(BiTreeNode* root) {
    // Base case: if the root is NULL, the depth is 0
    if (root == NULL) {
        return 0;
    } else {
        // Calculate the depth of the left and right subtrees recursively
        int left_depth = BiTree_depth(root->left);
        int right_depth = BiTree_depth(root->right);

        // Return the maximum depth among the left and right subtrees, plus 1 for the root
        return 1 + (left_depth > right_depth ? left_depth : right_depth);
    }
}

// Function to perform depth-first search (DFS) traversal and serialize the tree
void BiTree_serialize(FILE *fp, BiTreeNode* root, const char* algo) {
    if (root == NULL) {
        fprintf(fp, "#\n"); // Represent NULL node and move to the next line
        return;
    }

    if (strcmp(algo, "bfs") == 0) {
        // Perform breadth-first serialization (not implemented here)
    } else if (strcmp(algo, "dfs") == 0) {
        // Perform depth-first serialization
        fprintf(fp, "%d %s\n", root->key, root->data); // Serialize node data and move to the next line
        BiTree_serialize(fp, root->left, algo); // Serialize left subtree
        BiTree_serialize(fp, root->right, algo); // Serialize right subtree
    } else {
        fprintf(stderr, "Unsupported serialization algorithm: %s\n", algo);
    }
}



//  Function to check if a binary tree is full
bool BiTree_isfull(BiTreeNode *root) {
    // Base case: If the tree is empty (root is NULL), it is full
    if (root == NULL) {
        return true;
    }

    // If the current node is a leaf node (both children are NULL), it is full
    if (root->left == NULL && root->right == NULL) {
        return true;
    }

    // If both children are present, recursively check if their subtrees are full
    if (root->left != NULL && root->right != NULL) {
        return BiTree_isfull(root->left) && BiTree_isfull(root->right);
    }

    // If only one child is present, the tree is not full
    return false;
}
// Function to destroy a binary tree, including its nodes and associated data
void BiTree_destroy(BiTree* tree) {
    if (tree == NULL || tree->root == NULL) {
        return;  // Base case: If the tree or its root is NULL, there's nothing to destroy
    }

    BiTree_free(tree->root); // Free memory associated with the nodes and their data
    free(tree); // Free memory associated with the tree structure itself
}

// Function to free memory associated with the nodes of a binary tree
void BiTree_free(BiTreeNode *root) {
    if (root == NULL) {
        return; // Base case: If the root is NULL, there's nothing to free
    }

    // Free memory associated with left and right subtrees recursively
    BiTree_free(root->left);
    BiTree_free(root->right);

    // Free memory associated with the current node's data
    free(root->data);

    // Free memory associated with the current node
    free(root);
}


// Function to check if a binary tree is complete
bool BiTree_iscomplete(BiTreeNode *root) {
    // Base case: If the tree is empty (root is NULL), it is complete
    if (root == NULL) {
        return true;
    }

    // Create a queue to perform level-order traversal
    BiTreeNode **queue = (BiTreeNode **)malloc(sizeof(BiTreeNode *));
    int front = 0; // Front of the queue
    int rear = -1; // Rear of the queue

    // Enqueue the root node
    queue[++rear] = root;

    // Flag to track whether we've encountered a non-full node
    bool non_full_node_encountered = false;

    // Perform level-order traversal
    while (front <= rear) {
        // Dequeue a node from the front of the queue
        BiTreeNode *current = queue[front++];

        // If we've encountered a non-full node previously and the current node is not a leaf,
        // the tree is not complete
        if (non_full_node_encountered && (current->left != NULL || current->right != NULL)) {
            free(queue); // Free the memory allocated for the queue
            return false;
        }

        // If the current node has a left child, enqueue it
        if (current->left != NULL) {
            queue[++rear] = current->left;
        } else {
            // If the current node doesn't have a left child, set the flag
            non_full_node_encountered = true;
        }

        // If the current node has a right child, enqueue it
        if (current->right != NULL) {
            queue[++rear] = current->right;
        } else {
            // If the current node doesn't have a right child, and it's not the last level,
            // the tree is not complete
            if (front <= rear) {
                free(queue); // Free the memory allocated for the queue
                return false;
            }
        }
    }

    // Free the memory allocated for the queue
    free(queue);

    // If we reach this point, the tree is complete
    return true;
}


// Function to deserialize a binary tree
BiTreeNode* BiTree_deserialize(FILE *fp) {
    if (fp == NULL) {
        fprintf(stderr, "File pointer is NULL.\n");
        return NULL;
    }

    char key_str[20];
    char data_str[100]; // Assuming the maximum length of data string
    if (fscanf(fp, "%s", key_str) == 1) {
        if (strcmp(key_str, "#") == 0) {
            return NULL; // NULL node encountered
        }

        int key = atoi(key_str);
        fscanf(fp, "%s", data_str); // Read data
        BiTreeNode* root = (BiTreeNode*)malloc(sizeof(BiTreeNode));
        root->key = key;
        root->data = strdup(data_str); // Duplicate the data string
        root->left = BiTree_deserialize(fp);
        root->right = BiTree_deserialize(fp);
        return root;
    }
    return NULL;
}
