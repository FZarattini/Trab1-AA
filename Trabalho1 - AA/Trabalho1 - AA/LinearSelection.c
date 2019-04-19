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

//Ordena o vetor passado -- O(SET_SIZE^2) = O(constante)
void QuickSort(int *numbers, int left, int right) {
	int i, j, x, y;

	i = left;
	j = right;
	x = numbers[(left + right) / 2];

	while (i <= j) {
		while (numbers[i] < x && i < right) {
			i++;
		}
		while (numbers[j] > x && j > left) {
			j--;
		}
		if (i <= j) {
			y = numbers[i];
			numbers[i] = numbers[j];
			numbers[j] = y;
			i++;
			j--;
		}
	}

	if (j > left) {
		quick_sort(numbers, left, j);
	}
	if (i < right) {
		quick_sort(numbers, i, right);
	}
}

//Encontra a mediana de um vetor - O(SET_SIZE^2) e O(setQuant^2) == O(constante)
int FindMedian(int size, int *numbers) {
	int temp;
	int i, j;
	
	for (i = 0; i < size - 1; i++) {
		for (j = i + 1; j < size; j++) {
			if (numbers[j] < numbers[i]) {
				
				temp = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = temp;
			}
		}
	}

	if (size % 2 == 0) {
		return((numbers[size / 2] + numbers[size / 2 - 1]) / 2.0);
	}
	else {
		return numbers[size / 2];
	}
}

int MOM(numbers, k) {
	int i, j, median, mm;
	float setQuant;
	int * medians;
	int ** arrays;

	//Encontra a quantidade de Sets deve ser criado
	setQuant = k / 5;
	setQuant = ceil(setQuant);


	//Aloca array de arrays - Sets
	arrays = malloc(setQuant * sizeof(int*));

	//Aloca array de medianas
	medians = malloc(setQuant * sizeof(int));

	//Alocação e divisão em Sets de 5
	for (i = 0; i < setQuant; i++) {	//O(setQuant)
		arrays[i] = malloc(SET_SIZE * sizeof(int));
		memcpy(arrays[i], numbers + (i*SET_SIZE), SET_SIZE * sizeof(int));
		QuickSort(arrays[i], 0, SET_SIZE - 1);		//O(SET_SIZE^2) no pior caso = O(constante)
		median = FindMedian(SET_SIZE, arrays[i]);
		medians[i] = median;
	}


	//Encontra a mediana das medianas
	median = MOM(medians, setQuant / 2);

	return median;
}

int LinearSelection(int * numbers, int k) {
	int i, check, mom, countLeft = 0, countRight = 0;
	int* left;
	int* right;

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

	//------------LinearSelection----------------- 

	//Encontra a mediana das medianas
	mom = MOM(numbers, k);

	//Organiza numeros do vetor original em torno da mediana das medianas - O(n)
	for (i = 0; i < n; i++) {
		if (numbers[i] < mom) {
			//insere numbers[i] em left
			countLeft++;
			left = (int*)realloc(left, countLeft * sizeof(int));
			left[countLeft - 1] = numbers[i];
		}
		else {
			//insere numbers[i] em right
			countRight++;
			right = (int*)realloc(right, countRight * sizeof(int));
			right[countRight - 1] = numbers[i];
		}
	}

	if (countLeft == k - 1) {
		return mom;
	}
	else if (countLeft > k - 1) {
		return MOM(left, k);
	}
	else {
		return MOM(right, k - countLeft - 1);
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

