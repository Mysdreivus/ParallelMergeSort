#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 20

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

void merge(int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    int L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = array[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = array[m + 1+ j]; 
  
    /* Merge the temp arrays back into array[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            array[k] = L[i]; 
            i++; 
        } 
        else { 
            array[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) { 
        array[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) { 
        array[k] = R[j]; 
        j++; 
        k++; 
    } 
} 

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void* mergeSort(void* args) {
    StartEndIndexes sei = *((StartEndIndexes*)args);

    if (sei.start < sei.end) {

        int middle = (sei.start + sei.end) / 2;
        StartEndIndexes th_sei, th2_sei;
        th_sei.start = sei.start;
        th_sei.end = middle;

        th2_sei.start = middle + 1;
        th2_sei.end = sei.end;
        
        pthread_t thread_one, thread_two;
        pthread_create(&thread_one, NULL, &mergeSort, &th_sei);
        pthread_create(&thread_two, NULL, &mergeSort, &th2_sei);
        (void) pthread_join(thread_one, NULL);
        (void) pthread_join(thread_two, NULL);

        merge(sei.start, middle, sei.end);
    }
    return NULL;
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;
    
    pthread_t tid;
    
    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);
    
    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);
    
    // 3. Create a thread for merge sort.
     
    pthread_create(&tid, NULL, &mergeSort, &sei);
    
    // 4. Wait for mergesort to finish.
    
    pthread_join(tid, NULL);

    
    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);
}