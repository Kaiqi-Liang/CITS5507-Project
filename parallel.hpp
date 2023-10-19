#include <mpi.h>

#include "fish.hpp"

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
