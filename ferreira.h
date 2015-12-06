
#include <stdlib.h>
#include "teste.h"

Tjob *Crossover(Tjob *solution1, Tjob *solution2, int size) {
		int pos, i;
		pos = Randomize(0.60 * size);

		for(i = 0; i < pos; i++) {
			solution2[i] = solution1[i];
		}

	return solution2;
}

Tjob *Mutacao(Tjob *solution, int size) {
	Tjob jobAux;
	int troc1, troc2;
	
	troc1 = Randomize(size);
	troc2 = Randomize(size);

	jobAux = solution[troc1];

 	solution[troc1] = solution[troc2];
 	solution[troc2] = jobAux;
	
	return solution;
}

replace;