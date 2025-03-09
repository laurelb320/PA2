#include <iostream> 
#include <mpi.h>

#include "custom_collectives.h"

int custom_many2many(int *send_data, int *sendcounts, int** recv_data_ptr, int rank, int size) {
  /* write your code here */
}

void custom_allreduce_sum(int *local, int *global, int num_elem, int rank, int size) {
  //Initializes array for received value and step intervals
	int *received = (int*)malloc(num_elem *sizeof(int));
	int step = 1;
	
	//Copies local data into global
	for (int i = 0; i < num_elem; i++) {
		global[i] = local[i];
	}
	
	while (step < size) {
		//Uses XOR to determine the next step for hypercube communication
		int next = rank ^ step;
		
		if (next < size) {
			//Prevents block from causing deadlock by switching send/receive order
			if (rank < next) {
				MPI_Send(global, num_elem, MPI_INT, next, 0, MPI_COMM_WORLD);
				MPI_Recv(received, num_elem, MPI_INT, next, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			} else {
				MPI_Recv(received, num_elem, MPI_INT, next, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(global, num_elem, MPI_INT, next, 0, MPI_COMM_WORLD);
			}
			
			//Adds received value to global
			for (int i = 0; i < num_elem; i++) {
				global[i] += received[i];
			}
		}
		//Increments step
		step = step * 2;
	}
			
	free(received);
	return;
}
