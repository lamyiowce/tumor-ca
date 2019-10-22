# Tumor growth cellular automaton simulation in C++ and results appoximation with ML

Authors:
Rafał Banaś
Julia Bazińska
Olaf Łobożewicz
Agnieszka Strzałka

Project done as a Bachelor thesis in Computer at University of Warsaw. 

## Thesis abstract
Existing research provides a numerical method based on cellular automata to simulate growth
of EMT6/Ro tumor spheroid under varying radiotherapy treatment protocols. As the space
of possible protocols is very vast, the model has been used to conduct a heuristic genetic
algorithm search for an optimal dosage and timing of irradiation. However, current imple-
mentation of the simulation makes the search computationally costly, which limits the extent
of explored solutions.
We propose two alternative method for treatment protocol evaluation. Firstly, we reim-
plemented the cellular automaton in C++. It allowed to speed up the computations 20 times,
while preserving the numerical accuracy. Secondly, we conducted 150 000 simulations under
varying irradiation schedules. The results were used as training data for machine learning
regression algorithms to predict the total number of cencerous cells. We trained neural net-
works with dense, convolutional and recurrent architectures. The best result achieved was
with LSTM layers and gave mean squared error of 0.65 on normalized data.

## Usage
Build tumor-simulation executable. In project's main folder run:
```
mkdir build
cd build
cmake ..
make tumor
```
Usage:
```
./tumor <automaton_file> <protocol_file> <results_dir> <experiment_id> <nsteps>
```
Where:
- `<automaton_file>` is a file that holds initial tumor representation (one such
file is available at `resources/tumors/`);
- `<protocol_file>` is a file with radiation protocols to use, where one protocol
is represented by two lines: the first one is a list of space-separated radiation
doses (in Gy), and the second one is a list of simulation step numbers at which the radiation doses should
be applied (examples at `resources/protocols/`);
- `<nsteps>` – number of steps of the simulation to run, one step corresponds to 6 seconds of
simulated process; `<nsteps> = 144000` runs a 10-day simulation

Example:
```
./tumor ../resources/tumors/out-vnw-tr1-st0-0a-initial.json ../resources/protocols/example/6.csv ./results/example/ 1 100
```
Simulation results will be stored in
`results/example/1/out/1_<protocol_number>.csv`.
To extract just the number of alive cells from each result, run the following:
```
python3 ../misc/aliveCellCount.py results/example/1/out/
```
Alive cell count will be stored in
`results/example/1/out/aliveCount/1_<protocol_number>.csv`.
