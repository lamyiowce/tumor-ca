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
