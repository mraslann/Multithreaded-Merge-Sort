#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//struct for holding data at each node
typedef struct {
    int low;
    int high;
    int *array;
}node;

void *mergeSort(void *args);
void merge(int left, int mid, int right, int *array);

int main() {
    //read input file
    FILE *fp = fopen("testcases.txt", "r");
    if (fp == NULL) {
        printf("File not found");
        exit(0);
    }
    int size;
    fscanf(fp, "%d", &size);
    int *array = (int *) malloc(sizeof(int) * size);
    printf("Original array:\n");
    for (int i = 0; i < size; i++) {
        fscanf(fp, "%d", &array[i]);
        printf("%d ", array[i]);
    }
    printf("\n");
    fclose(fp);
    //creating the first thread that will start merge sort
    node *n = (node *) malloc(sizeof(node));
    n->low = 0;
    n->high = size - 1;
    n->array = array;
    //unique thread identifier
    pthread_t tid;
    //create the thread
    pthread_create(&tid, NULL, mergeSort, (void *) n);
    //join created thread with main thread
    pthread_join(tid, NULL);
    //print sorted array
    printf("Sorted array: \n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    free(array);
    return 0;
}

// recursive function of merge sort
// this is the routine each thread will execute
void *mergeSort(void *args) {
    node *n = (node *) args;
    if (n->low < n->high) {
        //finding middle element
        int mid = (n->low + n->high) / 2;
        //create two threads one to handle left part of array and other for the right part
        //two unique thread identifiers
        pthread_t tidLeft, tidRight;
        //state that the bothe the threads will be joined later after creation
        //initialize recursive step for the left thread
        node *stepLeft = (node *) malloc(sizeof(node));
        stepLeft->low = n->low;
        stepLeft->high = mid;
        stepLeft->array = n->array;
        //create the left thread
        pthread_create(&tidLeft, NULL, mergeSort, (void *) stepLeft);
        //initialize recursive step for the right thread
        node *stepRight = (node *) malloc(sizeof(node));
        stepRight->low = mid + 1;
        stepRight->high = n->high;
        stepRight->array = n->array;
        //create the right thread
        pthread_create(&tidRight, NULL, mergeSort, (void *) stepRight);
        //join left thread with thread which created it
        pthread_join(tidLeft, NULL);
        //join right thread with thread which created it
        pthread_join(tidRight, NULL);
        //merge left and right
        merge(n->low, mid, n->high, n->array);
        //free this recursive step
        free(n);
    }
    pthread_exit(NULL);
}
//merge function for merge sort algorithm
void merge(int left, int mid, int right, int *array) {
    int sizeLeft = mid - left + 1;
    int sizeRight = right - mid;
    int leftArray[sizeLeft];
    int rightArray[sizeRight];
    for (int i = 0; i < sizeLeft; i++)
        leftArray[i] = array[left + i];
    for (int j = 0; j < sizeRight; j++)
        rightArray[j] = array[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < sizeLeft && j < sizeRight) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            i++;
        } else {
            array[k] = rightArray[j];
            j++;
        }
        k++;
    }
    while (i < sizeLeft) {
        array[k] = leftArray[i];
        i++;
        k++;
    }
    while (j < sizeRight) {
        array[k] = rightArray[j];
        j++;
        k++;
    }
}
