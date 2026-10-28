
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

/* =========================
   SEPARATE CHAINING
   ========================= */

typedef struct Node {
    int key;
    int value;
    struct Node *next;
} Node;

Node *chainTable[SIZE] = {NULL};

/* =========================
   OPEN ADDRESSING
   ========================= */

typedef struct {
    int key;
    int value;
    int status;  /* 0 = Empty, 1 = Occupied, 2 = Deleted */
} Entry;

Entry openTable[SIZE];

/* =========================
   HASH FUNCTION
   ========================= */

int hashFunction(int key)
{
    int index = key % SIZE;
    return (index < 0) ? index + SIZE : index;
}

/* =========================
   INITIALIZE OPEN TABLE
   ========================= */

void initializeOpenTable(void)
{
    int i;

    for (i = 0; i < SIZE; i++) {
        openTable[i].status = 0;
    }
}

/* =========================
   CHECK DUPLICATE KEY
   ========================= */

int keyExists(int key)
{
    int index = hashFunction(key);
    int i;
    Node *temp;

    /* Search separate chaining */
    temp = chainTable[index];

    while (temp != NULL) {
        if (temp->key == key)
            return 1;

        temp = temp->next;
    }

    /* Search open addressing */
    for (i = 0; i < SIZE; i++) {
        int pos = (index + i) % SIZE;

        if (openTable[pos].status == 0)
            break;

        if (openTable[pos].status == 1 &&
            openTable[pos].key == key) {
            return 1;
        }
    }

    return 0;
}

/* =========================
   INSERT USING CHAINING
   ========================= */

void insertChaining(int key, int value)
{
    int index = hashFunction(key);
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newNode->key = key;
    newNode->value = value;
    newNode->next = chainTable[index];
    chainTable[index] = newNode;

    printf("Key inserted using Separate Chaining at index %d.\n",
           index);
}

/* =========================
   SEARCH USING CHAINING
   ========================= */

int searchChaining(int key)
{
    int index = hashFunction(key);
    Node *temp = chainTable[index];

    while (temp != NULL) {
        if (temp->key == key) {
            printf("Key found using Separate Chaining!\n");
            printf("Key   : %d\n", temp->key);
            printf("Value : %d\n", temp->value);
            printf("Index : %d\n", index);
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}

/* =========================
   DELETE USING CHAINING
   ========================= */

int deleteChaining(int key)
{
    int index = hashFunction(key);
    Node *temp = chainTable[index];
    Node *prev = NULL;

    while (temp != NULL) {
        if (temp->key == key) {
            if (prev == NULL)
                chainTable[index] = temp->next;
            else
                prev->next = temp->next;

            free(temp);
            printf("Key deleted successfully.\n");
            return 1;
        }

        prev = temp;
        temp = temp->next;
    }

    return 0;
}

/* =========================
   INSERT USING OPEN ADDRESSING
   LINEAR PROBING
   ========================= */

int insertOpenAddressing(int key, int value)
{
    int startIndex = hashFunction(key);
    int index;
    int firstDeleted = -1;
    int i;

    for (i = 0; i < SIZE; i++) {
        index = (startIndex + i) % SIZE;

        if (openTable[index].status == 2) {
            if (firstDeleted == -1)
                firstDeleted = index;
        }
        else if (openTable[index].status == 0) {
            if (firstDeleted != -1)
                index = firstDeleted;

            openTable[index].key = key;
            openTable[index].value = value;
            openTable[index].status = 1;

            printf("Key inserted using Open Addressing at index %d.\n",
                   index);
            return 1;
        }
    }

    /* A deleted slot may exist even if there are no empty slots. */
    if (firstDeleted != -1) {
        openTable[firstDeleted].key = key;
        openTable[firstDeleted].value = value;
        openTable[firstDeleted].status = 1;

        printf("Key inserted using Open Addressing at index %d.\n",
               firstDeleted);
        return 1;
    }

    printf("Open Addressing table is full!\n");
    return 0;
}

/* =========================
   SEARCH USING OPEN ADDRESSING
   ========================= */

int searchOpenAddressing(int key)
{
    int startIndex = hashFunction(key);
    int index;
    int i;

    for (i = 0; i < SIZE; i++) {
        index = (startIndex + i) % SIZE;

        if (openTable[index].status == 0)
            return 0;

        if (openTable[index].status == 1 &&
            openTable[index].key == key) {
            printf("Key found using Open Addressing!\n");
            printf("Key   : %d\n", openTable[index].key);
            printf("Value : %d\n", openTable[index].value);
            printf("Index : %d\n", index);
            return 1;
        }
    }

    return 0;
}

/* =========================
   DELETE USING OPEN ADDRESSING
   ========================= */

int deleteOpenAddressing(int key)
{
    int startIndex = hashFunction(key);
    int index;
    int i;

    for (i = 0; i < SIZE; i++) {
        index = (startIndex + i) % SIZE;

        if (openTable[index].status == 0)
            return 0;

        if (openTable[index].status == 1 &&
            openTable[index].key == key) {
            openTable[index].status = 2;
            printf("Key deleted successfully.\n");
            return 1;
        }
    }

    return 0;
}

/* =========================
   INSERT OPERATION
   ========================= */

void insert(void)
{
    int key, value, choice;

    printf("\nEnter key: ");
    if (scanf("%d", &key) != 1) {
        printf("Invalid input!\n");
        return;
    }

    printf("Enter value: ");
    if (scanf("%d", &value) != 1) {
        printf("Invalid input!\n");
        return;
    }

    printf("Hash index = %d\n", hashFunction(key));

    if (keyExists(key)) {
        printf("Duplicate key! Key already exists.\n");
        return;
    }

    printf("\nChoose collision resolution technique:\n");
    printf("1. Separate Chaining\n");
    printf("2. Open Addressing (Linear Probing)\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        return;
    }

    switch (choice) {
        case 1:
            insertChaining(key, value);
            break;

        case 2:
            insertOpenAddressing(key, value);
            break;

        default:
            printf("Invalid choice!\n");
    }
}

/* =========================
   SEARCH OPERATION
   ========================= */

void search(void)
{
    int key;

    printf("\nEnter key to search: ");
    if (scanf("%d", &key) != 1) {
        printf("Invalid input!\n");
        return;
    }

    if (searchOpenAddressing(key))
        return;

    if (searchChaining(key))
        return;

    printf("Key not found.\n");
}

/* =========================
   DELETE OPERATION
   ========================= */

void deleteKey(void)
{
    int key;

    printf("\nEnter key to delete: ");
    if (scanf("%d", &key) != 1) {
        printf("Invalid input!\n");
        return;
    }

    if (deleteOpenAddressing(key))
        return;

    if (deleteChaining(key))
        return;

    printf("Key not found.\n");
}

/* =========================
   FREE CHAINING MEMORY
   ========================= */

void freeChaining(void)
{
    int i;
    Node *temp, *next;

    for (i = 0; i < SIZE; i++) {
        temp = chainTable[i];

        while (temp != NULL) {
            next = temp->next;
            free(temp);
            temp = next;
        }

        chainTable[i] = NULL;
    }
}

/* =========================
   MAIN FUNCTION
   ========================= */

int main(void)
{
    int choice;

    initializeOpenTable();

    while (1) {
        printf("\n=========================\n");
        printf("       HASH TABLE\n");
        printf("=========================\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Exit\n");
        printf("=========================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            break;
        }

        switch (choice) {
            case 1:
                insert();
                break;

            case 2:
                search();
                break;

            case 3:
                deleteKey();
                break;

            case 4:
                freeChaining();
                printf("Program terminated.\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    freeChaining();
    return 0;
}