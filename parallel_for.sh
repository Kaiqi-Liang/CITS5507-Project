#!/bin/sh
#SPATCH --account=courses0101
#SBATCH --partition=debug
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00

g++ -fopenmp -std=c++20 -pedantic -Wall -Wextra -Wconversion -Wshadow -Ofast -DPARALLEL -DPARALLEL_FOR main.cpp -o parallel_for

export OMP_NUM_THREADS=16 

srun ./parallel_for
