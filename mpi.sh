#!/bin/sh 
#SBATCH --account=courses0101
#SBATCH --partition=debug
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=1
#SBATCH --time=01:00:00
#SBATCH --exclusive
#SBATCH --mem-per-cpu=32G
mpic++ -fopenmp -std=c++20 -pedantic -Wall -Wextra -Wconversion -Wshadow -Ofast parallel.cpp -o parallel
perf stat -e cpu-clock srun ./parallel
