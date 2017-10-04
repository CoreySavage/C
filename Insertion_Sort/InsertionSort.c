//
//  main.c
//  insertionSort
//
//  Corey Savage
//  CS 325
//  Code Modified from:http://quiz.geeksforgeeks.org/insertion-sort/
//


// C program for insertion sort
#include<stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


/* Function to sort an array using insertion sort*/
void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i-1;
        
        /* Move elements of arr[0..i-1], that are
         greater than key, to one position ahead
         of their current position */
        while (j >= 0 && arr[j] > key)
        {
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = key;
    }
}

// A utility function ot print an array of size n
void printArray(int arr[], int n)
{
    int i;
    for (i=0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}



/* Driver program to test insertion sort */
int main()
{
    srand(time(NULL));

    clock_t start_t, end_t, total_t;
    int i;
    // Change this number to change array size
    int size = 5000;
    float total;
    int arr[size];
    int n = sizeof(arr)/sizeof(arr[0]);
    
    
    for (i=0;i<size;i++) {
        arr[i] = rand() % 10001;
    }
    printf("Sorting had begun\n");
    start_t = clock();
    
    insertionSort(arr, n);
    
    end_t = clock();
    total = ((float) end_t - (float) start_t) / CLOCKS_PER_SEC;
    printf("\nSorting finished\nTime: %f\n", total);
    
    
    
    return 0;
    
    
    
    
}
