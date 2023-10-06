#!/bin/sh
#SPATCH --account=courses0101
#SBATCH --partition=debug
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=4
#SBATCH --time=01:00:00
export OMP_NUM_THREADS=4
g++ -fopenmp -std=c++20 -pedantic -Wall -Wextra -Wconversion -Wshadow -Ofast -DPARALLEL_PARTITION main.cpp -o parallel_partition
srun ./parallel_partition
