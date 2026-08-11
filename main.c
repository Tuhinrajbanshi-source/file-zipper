#include <stdio.h>
#include <stdlib.h>

/* =========================
   HUFFMAN NODE
   ========================= */

struct Node
{
    unsigned char data;
    int frequency;

    struct Node *left;
    struct Node *right;
};


/* =========================
   MIN HEAP
   ========================= */

struct MinHeap
{
    int size;
    int capacity;

    struct Node **array;
};


/* =========================
   CREATE A NEW NODE
   ========================= */

struct Node *createNode(unsigned char data, int frequency)
{
    struct Node *newNode;

    newNode = (struct Node *)malloc(sizeof(struct Node));

    newNode->data = data;
    newNode->frequency = frequency;

    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}


/* =========================
   CREATE MIN HEAP
   ========================= */

struct MinHeap *createMinHeap(int capacity)
{
    struct MinHeap *heap;

    heap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

    heap->size = 0;
    heap->capacity = capacity;

    heap->array = (struct Node **)malloc(
        capacity * sizeof(struct Node *)
    );

    return heap;
}


/* =========================
   SWAP TWO NODES
   ========================= */

void swapNodes(struct Node **a, struct Node **b)
{
    struct Node *temp;

    temp = *a;
    *a = *b;
    *b = temp;
}


/* =========================
   INSERT INTO MIN HEAP
   ========================= */

void insertMinHeap(struct MinHeap *heap, struct Node *node)
{
    int i;

    i = heap->size;

    heap->array[i] = node;
    heap->size++;

    /*
       Move the new node upward
       until the Min Heap property
       is restored.
    */

    while (i != 0 &&
           heap->array[(i - 1) / 2]->frequency >
           heap->array[i]->frequency)
    {
        swapNodes(
            &heap->array[i],
            &heap->array[(i - 1) / 2]
        );

        i = (i - 1) / 2;
    }
}


/* =========================
   MIN HEAPIFY
   ========================= */

void minHeapify(struct MinHeap *heap, int index)
{
    int smallest;
    int left;
    int right;

    smallest = index;

    left = 2 * index + 1;
    right = 2 * index + 2;

    /*
       Check left child.
    */

    if (left < heap->size &&
        heap->array[left]->frequency <
        heap->array[smallest]->frequency)
    {
        smallest = left;
    }

    /*
       Check right child.
    */

    if (right < heap->size &&
        heap->array[right]->frequency <
        heap->array[smallest]->frequency)
    {
        smallest = right;
    }

    /*
       If a child is smaller,
       swap and continue downward.
    */

    if (smallest != index)
    {
        swapNodes(
            &heap->array[index],
            &heap->array[smallest]
        );

        minHeapify(heap, smallest);
    }
}


/* =========================
   EXTRACT MINIMUM NODE
   ========================= */

struct Node *extractMin(struct MinHeap *heap)
{
    struct Node *minimum;
    struct Node *lastNode;

    /*
       If heap is empty.
    */

    if (heap->size <= 0)
    {
        return NULL;
    }

    /*
       Minimum element is always
       at index 0.
    */

    minimum = heap->array[0];

    /*
       Take the last node.
    */

    lastNode = heap->array[heap->size - 1];

    /*
       Reduce heap size.
    */

    heap->size--;

    /*
       Put last node at the root
       and restore Min Heap.
    */

    if (heap->size > 0)
    {
        heap->array[0] = lastNode;

        minHeapify(heap, 0);
    }

    return minimum;
}


/* =========================
   BUILD MIN HEAP FROM
   CHARACTER FREQUENCIES
   ========================= */

struct MinHeap *buildMinHeapFromFrequency(int frequency[])
{
    struct MinHeap *heap;

    int i;

    /*
       Maximum possible byte values = 256
    */

    heap = createMinHeap(256);

    /*
       Check all 256 possible values.
    */

    for (i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            struct Node *newNode;

            /*
               Create a node for every
               character that occurs.
            */

            newNode = createNode(
                (unsigned char)i,
                frequency[i]
            );

            /*
               Insert the node into
               the Min Heap.
            */

            insertMinHeap(heap, newNode);
        }
    }

    return heap;
}


/* =========================
   MAIN FUNCTION
   ========================= */

struct Node *buildHuffmanTree(int frequency[])
{
    struct MinHeap *heap;

    struct Node *left;
    struct Node *right;
    struct Node *parent;

    int i;

    heap = buildMinHeapFromFrequency(frequency);

    /*
       Continue until only one node
       remains in the heap.
    */

    while (heap->size > 1)
    {
        /*
           Extract two nodes with
           smallest frequencies.
        */

        left = extractMin(heap);
        right = extractMin(heap);

        /*
           Create a new internal node.

           Its frequency is the sum
           of the two extracted nodes.
        */

        parent = createNode(
            '$',
            left->frequency + right->frequency
        );

        /*
           Connect the two nodes
           as children.
        */

        parent->left = left;
        parent->right = right;

        /*
           Put the new parent back
           into the Min Heap.
        */

        insertMinHeap(heap, parent);
    }

    /*
       The final node is the
       root of the Huffman Tree.
    */

    return extractMin(heap);
}

void printTree(struct Node *root, int level)
{
    int i;

    if (root == NULL)
    {
        return;
    }

    for (i = 0; i < level; i++)
    {
        printf("    ");
    }

    if (root->left == NULL && root->right == NULL)
    {
        if (root->data == ' ')
        {
            printf("[space] : %d\n", root->frequency);
        }
        else if (root->data == '\n')
        {
            printf("[newline] : %d\n", root->frequency);
        }
        else
        {
            printf("%c : %d\n",
                   root->data,
                   root->frequency);
        }
    }
    else
    {
        printf("$ : %d\n", root->frequency);
    }

    printTree(root->left, level + 1);
    printTree(root->right, level + 1);
}

int main()
{
    FILE *file;

    int ch;
    int frequency[256] = {0};

    struct Node *root;

    /*
       Open sample file
    */

    file = fopen("sample.txt", "r");

    if (file == NULL)
    {
        printf("Could not open the file.\n");
        return 1;
    }

    /*
       Count frequencies
    */

    while ((ch = fgetc(file)) != EOF)
    {
        frequency[(unsigned char)ch]++;
    }

    fclose(file);

    /*
       Build Huffman Tree
    */

    root = buildHuffmanTree(frequency);

    /*
       Display Huffman Tree
    */

    printf("Huffman Tree:\n\n");

    printTree(root, 0);

    return 0;
}
