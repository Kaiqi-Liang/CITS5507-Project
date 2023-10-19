#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <mpi.h>
#include <string>

#include "fish.hpp"
#include "parallel.hpp"

constexpr int MASTER = 0;
constexpr size_t NUM_FIELDS = 4;

void write_fish(std::vector<Fish> const &school, std::string const &filename) {
	std::ofstream file(filename);
	std::copy(
	    school.cbegin(),
	    school.cend(),
	    std::ostream_iterator<Fish>(file)
	);
}

int main() {
	std::vector<Fish> send_buf;
	int process_id, num_processes;
	MPI_Init(0, 0);
	const MPI_Datatype MPI_FISH = create_mpi_struct();
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	if (process_id == MASTER) {
		send_buf = std::vector<Fish>(NUM_FISH);
		write_fish(send_buf, std::string{"before"});
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
	std::vector<Fish> master_buf;
	if (process_id == MASTER) {
		master_buf = std::vector<Fish>(NUM_FISH);
	}
	MPI_Gather(
	    recv_buf.data(),
	    num_fish_per_process,
	    MPI_FISH,
	    master_buf.data(),
	    num_fish_per_process,
	    MPI_FISH,
	    MASTER,
	    MPI_COMM_WORLD
	);
	assert(send_buf == master_buf);
	if (process_id == MASTER) {
		write_fish(master_buf, std::string{"after"});
	}
	MPI_Finalize();
	return 0;
}
