#!/bin/bash
# Example slurm running script.
#SBATCH -A G74-17
#SBATCH -N 1
#SBATCH -p topola
#SBATCH --cpus-per-task=24
#SBATCH --array=[1-5]
#SBATCH --output=slurm%A_%a.out

./tumor ../resources/tumors/out-vnw-tr1-st0-0a-initial.json \
../resources/protocols/example/${SLURM_ARRAY_TASK_ID} ./results/example/ ${SLURM_ARRAY_TASK_ID} 100
