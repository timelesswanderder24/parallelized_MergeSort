/*
    The Merge Sort to use for Operating Systems Assignment 1 2022
    written by Robert Sheehan

    Modified by: put your name here
    UPI: put your login here

    By submitting a program you are claiming that you and only you have made
    adjustments and additions to this code.
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>

#define SIZE    2

struct block {
    int size;
    int *first;
};

// void print_block_data(struct block *blk) {
//     printf("size: %d address: %p\n", blk->size, blk->first);
// }

/* Combine the two halves back together. */
void merge(struct block *left, struct block *right) {
	int combined[left->size + right->size];
	int dest = 0, l = 0, r = 0;
	while (l < left->size && r < right->size) {
		if (left->first[l] < right->first[r])
			combined[dest++] = left->first[l++];
		else
			combined[dest++] = right->first[r++];
	}
	while (l < left->size)
		combined[dest++] = left->first[l++];
	while (r < right->size)
		combined[dest++] = right->first[r++];
    memmove(left->first, combined, (left->size + right->size) * sizeof(int));
}

/* Merge sort the data. */
void merge_sort(struct block *my_data) {
    // print_block_data(my_data);
    if (my_data->size > 1) {
        struct block left_block;    //in memory this will be top half and bottom half
        struct block right_block;
        left_block.size = my_data->size / 2;  // half of the size that is assigned
        left_block.first = my_data->first;  //pointer to the first element of the whole array
        right_block.size = left_block.size + (my_data->size % 2);
        right_block.first = my_data->first + left_block.size; // add the first element pointer to the size of left block so that the pointer will move to the middle memory location
        merge_sort(&left_block);
        merge_sort(&right_block);
        merge(&left_block, &right_block);
    }
}

/* Check to see if the data is sorted. */
bool is_sorted(int data[], int size) {
    bool sorted = true;
    for (int i = 0; i < size - 1; i++) {
        if (data[i] > data[i + 1])
            sorted = false;
    }
    return sorted;
}

int main(int argc, char *argv[]) {
	long size;

	if (argc < 2) { //if argc is less than 2 then no arguments are supplied
		size = SIZE;
	} else {
		size = atol(argv[1]); //else take the paramenter that is given, atol will convert the string to long, since size is long
	}
    struct block start_block; //struct can hold structures
    int data[size];
    start_block.size = size; //assign the size to the start block
    start_block.first = data; //pointer to the first element, the name of the array
    for (int i = 0; i < size; i++) {
        data[i] = rand();         // assign random values to all the elements in the array
    }
    printf("starting---\n");
    merge_sort(&start_block);   //address where the start_block starts, will return a pointer
    printf("---ending\n");
    printf(is_sorted(data, size) ? "sorted\n" : "not sorted\n");
    exit(EXIT_SUCCESS);
}