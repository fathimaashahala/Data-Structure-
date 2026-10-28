
#include <stdio.h>
#include <stdlib.h>

#define M 4  /* Order of the B-Tree */

typedef struct BTreeNode
{
    int keys[M - 1];
    struct BTreeNode *child[M];
    int n;
    int leaf;
} BTreeNode;

BTreeNode *root = NULL;

/* Create a new node */
BTreeNode *createNode(int leaf)
{
    BTreeNode *t = calloc(1, sizeof(BTreeNode));

    if (t == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    t->leaf = leaf;
    return t;
}

/* Search for a key */
int search(BTreeNode *x, int key)
{
    if (x == NULL)
        return 0;

    int i = 0;

    while (i < x->n && key > x->keys[i])
        i++;

    if (i < x->n && key == x->keys[i])
        return 1;

    if (x->leaf)
        return 0;

    return search(x->child[i], key);
}

/* Split a full child */
void splitChild(BTreeNode *parent, int i)
{
    BTreeNode *y = parent->child[i];
    BTreeNode *z = createNode(y->leaf);

    /*
       For order 4, a full node has 3 keys.
       Promote the middle key to the parent.
    */
    int mid = y->n / 2;
    int j;

    z->n = y->n - mid - 1;

    for (j = 0; j < z->n; j++)
        z->keys[j] = y->keys[mid + 1 + j];

    if (!y->leaf)
    {
        for (j = 0; j <= z->n; j++)
            z->child[j] = y->child[mid + 1 + j];
    }

    int median = y->keys[mid];
    y->n = mid;

    for (j = parent->n; j > i; j--)
    {
        parent->keys[j] = parent->keys[j - 1];
        parent->child[j + 1] = parent->child[j];
    }

    parent->keys[i] = median;
    parent->child[i + 1] = z;
    parent->n++;
}

/* Insert into a node that is not full */
void insertNonFull(BTreeNode *x, int key)
{
    int i = x->n - 1;

    if (x->leaf)
    {
        while (i >= 0 && key < x->keys[i])
        {
            x->keys[i + 1] = x->keys[i];
            i--;
        }

        x->keys[i + 1] = key;
        x->n++;
    }
    else
    {
        while (i >= 0 && key < x->keys[i])
            i--;

        i++;

        if (x->child[i]->n == M - 1)
        {
            splitChild(x, i);

            if (key > x->keys[i])
                i++;
        }

        insertNonFull(x->child[i], key);
    }
}

/* Insert a key into the B-Tree */
void insert(int key)
{
    if (search(root, key))
    {
        printf("Duplicate key not allowed.\n");
        return;
    }

    if (root == NULL)
    {
        root = createNode(1);
        root->keys[0] = key;
        root->n = 1;
        printf("Key inserted successfully.\n");
        return;
    }

    if (root->n == M - 1)
    {
        BTreeNode *s = createNode(0);
        s->child[0] = root;

        root = s;
        splitChild(s, 0);
    }

    insertNonFull(root, key);
    printf("Key inserted successfully.\n");
}

/* Inorder traversal */
void traverse(BTreeNode *x)
{
    if (x == NULL)
        return;

    int i;

    for (i = 0; i < x->n; i++)
    {
        if (!x->leaf)
            traverse(x->child[i]);

        printf("%d ", x->keys[i]);
    }

    if (!x->leaf)
        traverse(x->child[i]);
}

/* Free all allocated nodes */
void freeTree(BTreeNode *x)
{
    if (x == NULL)
        return;

    if (!x->leaf)
    {
        for (int i = 0; i <= x->n; i++)
            freeTree(x->child[i]);
    }

    free(x);
}

/* Main menu */
int main(void)
{
    int choice, key;

    while (1)
    {
        printf("\n--- B-TREE (ORDER 4) ---\n");
        printf("1. Insert\n");
        printf("2. Traversal\n");
        printf("3. Search\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            freeTree(root);
            return 1;
        }

        switch (choice)
        {
            case 1:
                printf("Enter key: ");

                if (scanf("%d", &key) != 1)
                {
                    printf("Invalid input!\n");
                    freeTree(root);
                    return 1;
                }

                insert(key);
                break;

            case 2:
                printf("B-Tree traversal: ");
                traverse(root);
                printf("\n");
                break;

            case 3:
                printf("Enter key to search: ");

                if (scanf("%d", &key) != 1)
                {
                    printf("Invalid input!\n");
                    freeTree(root);
                    return 1;
                }

                if (search(root, key))
                    printf("Key found.\n");
                else
                    printf("Key not found.\n");

                break;

            case 4:
                printf("Program terminated.\n");
                freeTree(root);
                root = NULL;
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}
