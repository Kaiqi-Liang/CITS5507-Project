#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mpi.h>
#ifdef OPENMP
	#include <omp.h>
#endif
#include <string>

#include "fish.hpp"

constexpr int MASTER = 0;
constexpr size_t NUM_FIELDS = 4;

MPI_Datatype create_mpi_struct() {
	MPI_Datatype MPI_FISH;
	MPI_Datatype types[NUM_FIELDS];
	std::fill_n(types, 4, MPI_DOUBLE);
	int blocklengths[NUM_FIELDS];
	std::fill_n(blocklengths, 4, 1);
	const MPI_Aint offsets[NUM_FIELDS] = {
	    offsetof(Fish, distance_),
	    offsetof(Fish, weight_),
	    offsetof(Fish, x_),
	    offsetof(Fish, y_),
	};
	MPI_Type_create_struct(NUM_FIELDS, blocklengths, offsets, types, &MPI_FISH);
	MPI_Type_commit(&MPI_FISH);
	return MPI_FISH;
}

int main() {
	std::vector<Fish> send_buf;
	int process_id, num_processes;
	MPI_Init(0, 0);
	const MPI_Datatype MPI_FISH = create_mpi_struct();
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	if (process_id == MASTER) {
		#ifdef OPENMP
			std::cout << "mpi with ";
		#else
			std::cout << "mpi + openmp with ";
		#endif
		std::cout << num_processes << " processes\n";
		send_buf = std::vector<Fish>(NUM_FISH);
	}
	const int num_fish_per_process = NUM_FISH / num_processes;
	std::vector<Fish> recv_buf(num_fish_per_process);
	MPI_Scatter(
	    send_buf.data(),
	    num_fish_per_process,
	    MPI_FISH,
	    recv_buf.data(),
	    num_fish_per_process,
	    MPI_FISH,
	    MASTER,
	    MPI_COMM_WORLD
	);
	for (std::size_t i = 0; i < NUM_STEPS; i++) {
		double max_difference = 0;
		double local_numerator = 0;
		double local_denominator = 0;
		#ifdef OPENMP
			#pragma omp parallel for reduction(max : max_difference) schedule(static)
		#endif
		for (int j = 0; j < num_fish_per_process; j++) {
			max_difference =
			    std::max(max_difference, recv_buf[j].difference());
		}

		double global_max_difference;
		MPI_Reduce(
		    &max_difference,
		    &global_max_difference,
		    1,
		    MPI_DOUBLE,
		    MPI_MAX,
		    MASTER,
		    MPI_COMM_WORLD
		);
		MPI_Bcast(
		    &global_max_difference,
		    1,
		    MPI_DOUBLE,
		    MASTER,
		    MPI_COMM_WORLD
		);

		#ifdef OPENMP
			#pragma omp parallel for schedule(static)
		#endif
		for (int j = 0; j < num_fish_per_process; j++) {
			recv_buf[j].action(i, global_max_difference);
		}

		#ifdef OPENMP
			#pragma omp parallel for reduction(+ : local_numerator, local_denominator) schedule(static)
		#endif
		for (int j = 0; j < num_fish_per_process; j++) {
			local_numerator += recv_buf[j].distance_ * recv_buf[j].weight_;
			local_denominator += recv_buf[j].distance_;
		}

		double global_numerator;
		double global_denominator;
		MPI_Reduce(
		    &local_numerator,
		    &global_numerator,
		    1,
		    MPI_DOUBLE,
		    MPI_SUM,
		    MASTER,
		    MPI_COMM_WORLD
		);
		MPI_Reduce(
		    &local_denominator,
		    &global_denominator,
		    1,
		    MPI_DOUBLE,
		    MPI_SUM,
		    MASTER,
		    MPI_COMM_WORLD
		);
		if (process_id == MASTER) {
			double barycentre = global_numerator / global_denominator;
			if (barycentre <= 0) {
				throw std::exception();
			}
			#ifdef DEBUG
				std::cout << std::fixed << barycentre << std::endl;
			#endif
		}
	}
	MPI_Finalize();
	return 0;
}
