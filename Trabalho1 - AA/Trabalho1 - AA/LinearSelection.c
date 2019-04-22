#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define n 25
#define SET_SIZE 5


//Checa se vetor está ordenado -- O(n)
int CheckSorted(int * numbers) {
	int i;

	for (i = 0; i < n-1; i++) {
		if (numbers[i] > numbers[i+1])
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
		QuickSort(numbers, left, j);
	}
	if (i < right) {
		QuickSort(numbers, i, right);
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

int MOM(int * numbers, int k, int size) {
	int i, j, median, mm, countLeft = 0, countRight = 0;
	double setQuant;
	int * medians;
	int ** arrays;	

	//Encontra a quantidade de Sets deve ser criado
	setQuant = ceil(size / 5);

	if (setQuant <= 1.0) {
		mm = FindMedian(size, numbers);
	}
	else {


		//Aloca array de arrays - Sets
		arrays = (int**)malloc(setQuant * sizeof(int*));

		//Aloca array de medianas
		medians = (int*)malloc(setQuant * sizeof(int));

		//Alocação e divisão em Sets de 5
		for (i = 0; i < setQuant; i++) {	//O(setQuant)
			arrays[i] = (int*)malloc(SET_SIZE * sizeof(int));
			memcpy(arrays[i], numbers + (i*SET_SIZE), SET_SIZE * sizeof(int));
			QuickSort(arrays[i], 0, SET_SIZE - 1);		//O(SET_SIZE^2) no pior caso = O(constante)
			median = FindMedian(SET_SIZE, arrays[i]);
			medians[i] = median;
		}

		QuickSort(medians, 0, setQuant - 1);

		mm = FindMedian(setQuant, medians);
	}

	for (i = 0; i < size; i++) {
		if (numbers[i] < mm) {
			countLeft++;
		}
		else if (numbers[i] > mm) {
			countRight++;
		}
	}

	int * left = malloc(countLeft * sizeof(int));
	int * right = malloc(countRight * sizeof(int));
	countLeft = 0;
	countRight = 0;

	for (i = 0; i < size; i++) {
		if (numbers[i] < mm) {
			//insere numbers[i] em left
			countLeft++;
			left[countLeft-1] = numbers[i];
		}
		else if(numbers[i] > mm){
			//insere numbers[i] em right
			countRight++;
			right[countRight-1] = numbers[i];
		}
	}

	if (countLeft == k - 1) {
		return mm;
	}
	else if (countLeft > k - 1) {
		return MOM(left, k, countLeft);
	}
	else {
		return MOM(right, k - countLeft - 1, countRight);
	}
}

void CountingSort(int* numbers, int size, int max)
{
	int total = 0, i, oldCount;
	int* result = (int*)malloc(size * sizeof(int));
	int* count = (int*)malloc(max * sizeof(int));

	for (i = 0; i < max; i++) {
		count[i] = 0;
	}

	for (i = 0; i < size; i++)
		count[numbers[i]]++;

	for (i = 0; i < max; i++) {
		oldCount = count[i];
		count[i] = total;
		total += oldCount;
	}

	for (i = 0; i < size; i++) {
		result[count[numbers[i]]] = numbers[i];
		count[numbers[i]]++;
	}

	for (i = 0; i < size; i++) {
		numbers[i] = result[i];
	}
}

int CheckRepeating(int* numbers, int size) {
	int i;
	int* count = (int*)calloc(sizeof(int), (size - 2));

	for (i = 0; i < size; i++) {
		if (count[numbers[i]] == 1) {
			return 1;
		}
		else {
			count[numbers[i]]++;
		}
	}

	return 0;
}

int GetMax(int* numbers, int size) {
	int max = 0;

	for (int i = 0; i < size; i++) {
		if (numbers[i] > max) {
			max = numbers[i];
		}
	}
	return max;
}

int RemoveDuplicates(int* numbers, int size) {
	int aux = 0, i;
	for (i = 1; i < size; i++) {
		if (numbers[aux] != numbers[i]) {
			numbers[++aux] = numbers[i]; 
		}
	}
	return aux + 1;
}

int LinearSelection(int * numbers, int k, int size, int repeating) {
	int i, check, mom, countLeft = 0, countRight = 0, result, max;
	int* left = (int*)malloc(sizeof(int));
	int* right = (int*)malloc(sizeof(int));
	int* count = (int*)calloc(sizeof(int), (size - 2));


	//Checa se posição é maior ou menor que o tamanho do vetor
	if (k < 1 || k > size) {
		printf("ERROR: index out of bounds! \n");
		return -1;
	}

	//Checa se vetor está ordenado e se estiver retorna o k-ésimo menor

	if (CheckSorted(numbers) == 1) {
		return numbers[k - 1];
	}

	//Se houver números repetidos na entrada, ordena com Counting Sort, remove duplicatas e retorna k-ésimo pelo índice - O(n
	if (repeating == 1) {
		max = GetMax(numbers, size);

		CountingSort(numbers, size, max + 1);
		size = RemoveDuplicates(numbers, size);

		return numbers[k - 1];
	}

	return MOM(numbers, k, size);

}


int main (void) {
	int k, size,i, count = 0, repeating, max;
	//int numbers[n] = {1,3,3,9,0,4,6,5,10,11,15,16,16,13,14,19,12,12,21,22,25,24,23,18,7};	
	int * numbers;
	int result;

	printf("Quantos numeros terao a entrada?\n");
	scanf_s("%d", &size);

	printf("Forneca o valor de k!\n");
	scanf_s("%d", &k);

	numbers = (int*)malloc(size * sizeof(int));

	printf("Forneca %d numeros inteiros!\n", size);

	while (count < size) {
		scanf_s("%d", &numbers[count]);
		count++;
	}

	fflush(stdin);

	repeating = CheckRepeating(numbers, size);

	result = LinearSelection(numbers, k, size, repeating);

	printf("O k-ésimo menor numero da entrada eh %d \n", result);

	return 0;
}

