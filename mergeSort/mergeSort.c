//
//  main.c
//  mergeSort
//
//  Corey Savage
//  CS 325
//  Code Modified from:http://quiz.geeksforgeeks.org/merge-sort/
//


/* C program for Merge Sort */
#include<stdlib.h>
#include<stdio.h>
#include <time.h>

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    
    /* create temp arrays */
    int L[n1], R[n2];
    
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    /* Copy the remaining elements of L[], if there
     are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    /* Copy the remaining elements of R[], if there
     are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
 sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
        
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        
        merge(arr, l, m, r);
    }
}

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

/* Driver program to test above functions */
int main()
{
    srand(time(NULL));
    
    clock_t start_t, end_t, total_t;
    int i;
    // Change this number to change array size
    int size = 5000;
    float total;
    int arr[size];
    int arr_size = sizeof(arr)/sizeof(arr[0]);
    
    for (i=0;i<size;i++) {
        arr[i] = rand() % 10001;
    }
    printf("Sorting had begun\n");
    start_t = clock();
    
    mergeSort(arr, 0, arr_size - 1);
    
    end_t = clock();
    total = ((float) end_t - (float) start_t) / CLOCKS_PER_SEC;
    printf("\nSorting finished\nTime: %f\n", total);
    
    
    
    return 0;
}
