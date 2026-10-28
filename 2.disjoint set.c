
#include <stdio.h>

#define MAX 20

int parent[MAX];
int rank[MAX];

/* MAKE-SET: Create a separate set for each element */
void makeSet(int n)
{
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
        rank[i] = 0;
    }
}

/* FIND: Find the representative of the set */
int find(int x)
{
    if (parent[x] != x)
    {
        parent[x] = find(parent[x]);  // Path compression
    }

    return parent[x];
}

/* UNION: Merge two sets using union by rank */
void unionSet(int x, int y)
{
    int rootX = find(x);
    int rootY = find(y);

    if (rootX == rootY)
    {
        printf("%d and %d are already in the same set.\n", x, y);
        return;
    }

    if (rank[rootX] < rank[rootY])
    {
        parent[rootX] = rootY;
    }
    else if (rank[rootX] > rank[rootY])
    {
        parent[rootY] = rootX;
    }
    else
    {
        parent[rootY] = rootX;
        rank[rootX]++;
    }

    printf("Sets containing %d and %d have been merged.\n", x, y);
}

/* Display parent and rank of each element */
void display(int n)
{
    printf("\nElement\tParent\tRank\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\n", i, parent[i], rank[i]);
    }
}

int main(void)
{
    int n, choice, x, y;

    printf("Enter number of elements (1-%d): ", MAX);

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
    {
        printf("Invalid number of elements!\n");
        return 1;
    }

    makeSet(n);

    while (1)
    {
        printf("\n--- DISJOINT SET ---\n");
        printf("1. FIND\n");
        printf("2. UNION\n");
        printf("3. Display\n");
        printf("4. Exit\n");

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            return 1;
        }

        switch (choice)
        {
            case 1:
                printf("Enter element (0-%d): ", n - 1);

                if (scanf("%d", &x) != 1 || x < 0 || x >= n)
                {
                    printf("Invalid element!\n");
                    break;
                }

                printf("Representative of %d = %d\n", x, find(x));
                break;

            case 2:
                printf("Enter two elements (0-%d): ", n - 1);

                if (scanf("%d %d", &x, &y) != 2 ||
                    x < 0 || x >= n || y < 0 || y >= n)
                {
                    printf("Invalid elements!\n");
                    break;
                }

                unionSet(x, y);
                break;

            case 3:
                display(n);
                break;

            case 4:
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }
}
