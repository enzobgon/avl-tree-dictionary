#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD 50
#define MAX_MEANING 300

typedef struct Node {
    char word[MAX_WORD];
    char meaning[MAX_MEANING];
    struct Node *right;
    struct Node *left;
    struct Node *parent;
    int balanceFactor; // fb
} Node;

// Create a new node
Node *createNode(char *word, char *meaning) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode->word, word);
    strcpy(newNode->meaning, meaning);
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->parent = NULL;
    newNode->balanceFactor = 0;
    return newNode;
}

// Calculate Height
int getHeight(Node *node) {
    if (node == NULL) {
        return 0;
    } else {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
    }
}

// Calculate Depth
int getDepth(Node *node) {
    int depth = 0;
    while (node->parent != NULL) {
        depth++;
        node = node->parent;
    }
    return depth;
}

// Calculate Balance Factor
void calculateBalanceFactor(Node *node) {
    if (node != NULL) {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        node->balanceFactor = leftHeight - rightHeight;
    }
}

// Right Rotation
Node *rotateRight(Node *node) {
    Node *temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL)
        temp->right->parent = node;
    temp->right = node;
    temp->parent = node->parent;
    node->parent = temp;
    calculateBalanceFactor(node);
    calculateBalanceFactor(temp);
    return temp;
}

// Left Rotation
Node *rotateLeft(Node *node) {
    Node *temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL)
        temp->left->parent = node;
    temp->parent = node->parent;
    temp->left = node;
    node->parent = temp;
    calculateBalanceFactor(node);
    calculateBalanceFactor(temp);
    return temp;
}

// Right-Left Rotation
Node *rotateRightLeft(Node *node) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

// Left-Right Rotation
Node *rotateLeftRight(Node *node) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

// Balance the tree
Node *balanceTree(Node *node) {
    if (node == NULL) return NULL;

    calculateBalanceFactor(node);
    
    // Left heavy
    if (node->balanceFactor > 1) {
        if (node->left->balanceFactor >= 0)
            return rotateRight(node);
        else
            return rotateLeftRight(node);
    } 
    // Right heavy
    else if (node->balanceFactor < -1) {
        if (node->right->balanceFactor <= 0)
            return rotateLeft(node);
        else
            return rotateRightLeft(node);
    }
    return node;
}

Node *search(Node *node, char *word) {
    if (node == NULL) {
        printf("Word '%s' not found.\n", word);
        return NULL;
    }
    if (strcmp(word, node->word) == 0) {
        printf("Found: %s (Depth: %d)\nMeaning: %s\n", node->word, getDepth(node), node->meaning);
        return node;
    } else if (strcmp(word, node->word) < 0) {
        return search(node->left, word);
    } else {
        return search(node->right, word);
    }
}

Node *insert(Node *node, char *word, char *meaning) {
    if (node == NULL) {
        return createNode(word, meaning);
    }
    if (strcmp(word, node->word) < 0) {
        node->left = insert(node->left, word, meaning);
        node->left->parent = node;
    } else if (strcmp(word, node->word) > 0) {
        node->right = insert(node->right, word, meaning);
        node->right->parent = node;
    } else {
        printf("Word '%s' already exists.\n", word);
        return node;
    }
    return balanceTree(node);
}

void insertFromFile(Node **root, const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", fileName);
        return;
    }

    FILE *output = fopen("insertion_time.csv", "w");
    if (output == NULL) {
        printf("Error creating output log.\n");
        fclose(file);
        return;
    }

    fprintf(output, "WordCount,ExecutionTime(ms)\n");

    char word[MAX_WORD];
    int count = 0;
    clock_t start, end;
    double totalTime = 0.0;

    while (fscanf(file, "%49s", word) != EOF) {
        start = clock();
        *root = insert(*root, word, "Imported from file");
        end = clock();

        totalTime += ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
        fprintf(output, "%d,%.2f\n", count++, totalTime);
    }

    fclose(file);
    fclose(output);
    printf("Words from %s inserted successfully. Log saved to insertion_time.csv\n", fileName);
}

// Logic for node removal (simplified for brevity, keeping structure)
Node *removeNode(Node *node, char *word) {
    if (node == NULL) return NULL;

    if (strcmp(word, node->word) < 0) {
        node->left = removeNode(node->left, word);
    } else if (strcmp(word, node->word) > 0) {
        node->right = removeNode(node->right, word);
    } else {
        // Node found
        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;
        }
        if (node->left == NULL) {
            Node *temp = node->right;
            temp->parent = node->parent;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            Node *temp = node->left;
            temp->parent = node->parent;
            free(node);
            return temp;
        }
        Node *temp = node->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }
        strcpy(node->word, temp->word);
        strcpy(node->meaning, temp->meaning);
        node->right = removeNode(node->right, temp->word);
    }
    return balanceTree(node);
}

void printInOrder(Node *node) {
    if (node != NULL) {
        printInOrder(node->left);
        printf("%s: %s (Depth = %d)\n", node->word, node->meaning, getDepth(node));
        printInOrder(node->right);
    }
}

void freeTree(Node *node) {
    if (node != NULL) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}

int main() {
    Node *root = NULL;
    int op;
    char word[MAX_WORD], meaning[MAX_MEANING], fileName[MAX_WORD];

    printf("AVL Dictionary Initialized. Waiting for commands...\n");

    do {
        if (scanf("%d", &op) == EOF) break;
        switch (op) {
            case 0: break;
            case 1: // Create
                if (root == NULL) printf("Tree created.\n");
                else printf("Tree already exists.\n");
                break;
            case 2: // Remove
                scanf("%s", word);
                root = removeNode(root, word);
                break;
            case 3: // Insert
                scanf("%s", word);
                getchar(); // Consume newline
                fgets(meaning, sizeof(meaning), stdin);
                meaning[strcspn(meaning, "\n")] = 0; // Remove newline char
                root = insert(root, word, meaning);
                break;
            case 4: // Search
                scanf("%s", word);
                search(root, word);
                break;
            case 5: // Print
                printInOrder(root);
                break;
            case 6: // Load File
                scanf("%s", fileName);
                insertFromFile(&root, fileName);
                break;
            default:
                printf("Invalid operation.\n");
        }
    } while (op != 0);

    freeTree(root);
    return 0;
}
