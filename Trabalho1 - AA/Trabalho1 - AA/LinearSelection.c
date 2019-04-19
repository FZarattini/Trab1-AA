#include <stdio.h>
#define n 100
#define SET_SIZE 5


//Checa se vetor está ordenado -- O(n)
int CheckSorted(int * numbers) {
	int i;

	for (i = 0; i < n; i++) {
		if (numbers[i] > numbers[i++])
			return 0;
	}
	return 1;
}

int LinearSelection(int * numbers, int k) {
	int i,j, check;
	float setQuant;
	int ** arrays;

	//Checa se posição é maior ou menor que o tamanho do vetor
	if (k < 1 || k > (n + 1)) {
		printf("ERROR: index out of bounds!");
		return -1;
	}

	//Checa se vetor está ordenado e se estiver retorna o k-ésimo menor
	check = CheckSorted(numbers);

	if (check == 1) {
		return numbers[k - 1];
	}

	//LinearSelection abaixo
	setQuant = k / 5;
	setQuant = ceil(setQuant);

	//Aloca array de arrays - Sets de 5
	arrays = malloc(setQuant * sizeof(int*));

	for (i = 0; i < setQuant; i++) {
		arrays[i] = malloc(SET_SIZE *sizeof(int));
	}

	//Divisao em Sets de 5
	for (i = 0; i < setQuant; i++) {
		memcpy(arrays[i], numbers + (i*SET_SIZE), SET_SIZE * sizeof(int));
	}

}

int main (void) {
	int k = 2;
	int numbers[n];
	int result;

	result = LinearSelection(numbers, k);

	printf("O k-ésimo menor número da entrada é %d", result);

	return 0;
}

