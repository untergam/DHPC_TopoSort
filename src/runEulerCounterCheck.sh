#!/usr/bin/env bash
# chmod +x run.sh

#==================
# GENERAL SETTINGS
#==================
# Number of threads
THREADS_MIN=$1
THREADS_STEP=1
THREADS_MAX=$2

# Number of repetitions for each experiment
NREP=1

# Executables
declare -a EXE=('./toposort_omp_locallist_opt0_an0.exe' './toposort_omp_locallist_opt2_an0' './toposort_omp_bitset_opt1_an0' './toposort_omp_bitset_opt0_an0' './toposort_omp_dynamic_nobarrier_opt0_an0' './toposort_omp_dynamic_nobarrier_opt2_an0')
declare -a GRAPH_TYPES=('r')
NODE_DEGREE=30
RESULTS_DIR='resultsCounterCheck'

if [ ! -d "$RESULTS_DIR" ]; then
	mkdir "$RESULTS_DIR"
fi

# Problem sizes
N_SSC=1000000    #10^6

#==================================
# STRONG SCALING BENCHMARKS
#==================================
for (( i=0; i<$NREP; i=i+1 ))
do
# Benchmark parallel versions
	for (( t=$THREADS_MIN; t<=$THREADS_MAX; t=t+$THREADS_STEP ))
	do
		export OMP_NUM_THREADS=$t
		for gt in "${GRAPH_TYPES[@]}"
		do
			for exe in "${EXE[@]}"
			# Strong scaling
			eval "./$exe $gt $N_SSC $RESULTS_DIR $NODE_DEGREE"
		done
	done
done

