/*
    The Merge Sort to use for Operating Systems Assignment 1 2022
    written by Robert Sheehan

    Modified by: Imashi Kinigama
    UPI: ikin507

    By submitting a program you are claiming that you and only you have made
    adjustments and additions to this code.
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/resource.h>
#include <stdbool.h>
#include<sys/wait.h>
#include<sys/mman.h>

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
        struct block left_block;
        struct block right_block;
        left_block.size = my_data->size / 2;
        left_block.first = my_data->first;
        right_block.size = left_block.size + (my_data->size % 2);
        right_block.first = my_data->first + left_block.size;
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
	void *shared;

	struct rlimit stack_size;

	getrlimit(RLIMIT_STACK, &stack_size);

	stack_size.rlim_cur = 1100000000;

	setrlimit(RLIMIT_STACK, &stack_size);
	
	if (argc < 2) {
		size = SIZE;
	} else {
		size = atol(argv[1]);
	}
    struct block start_block;
    int data[size];
    start_block.size = size;
    start_block.first = data;
    for (int i = 0; i < size; i++) {
        data[i] = rand();
    }
    
    int *shared_data = mmap(NULL, (sizeof(int) * start_block.size), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int p = fork();
    
    struct block right_part;
    right_part.first = data+(size/2);
    right_part.size = (size/2)+(size%2);
    struct block left_part;
    left_part.first = data;
    left_part.size = (size/2);
    
    if(p==0){
	    merge_sort(&left_part);
	    for(int i=0;i<(left_part.size);i++){
	    	*(shared_data+i) = data[i];
	    }
	    exit(0);
	    
    }
    
    else if (p>0){
    	    printf("starting---\n");
	    merge_sort(&right_part);
	    wait(NULL);      //wait for the child process
	    for(int i=0;i<(left_part.size);i++){
	    	data[i] = *(shared_data+i);
	    }
	    merge(&left_part, &right_part);
	    
	    printf("---ending\n");
	    printf(is_sorted(data, size) ? "sorted\n" : "not sorted\n");
    }
    
}

