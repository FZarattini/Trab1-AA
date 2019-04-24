#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define SET_SIZE 5
#define MAX_VALUE 100000


//Checa se vetor está ordenado -- O(n)
int CheckSorted(int * numbers, int size) {
	int i;

	for (i = 0; i < size-1; i++) {
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

	free(result);
	free(count);
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

int RemoveDuplicates(int * numbers, int size) {
	int aux = 0, i;
	if (size == 0) return 0;
	for (i = 1; i < size; i++) {
		if (numbers[aux] != numbers[i]) {
			numbers[++aux] = numbers[i];
		}
	}
	return aux + 1;
}

int MOM(int * numbers, int k, int size) {
	int i, j, median, mm, countLeft = 0, countRight = 0, max, maxLeft, maxRight;
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
			max = GetMax(arrays[i], SET_SIZE); //O(SET_SIZE) no pior caso = O(constante)
			CountingSort(arrays[i], SET_SIZE, max + 1);	//O(SET_SIZE) no pior caso = O(constante)
			median = FindMedian(SET_SIZE, arrays[i]); //O(SET_SIZE) no pior caso = O(constante)
			medians[i] = median;
		}

		max = GetMax(medians, setQuant); //O(setQuant) -> O(n/5) -> O(n)
		CountingSort(medians, setQuant, max + 1); //O(setQuant) -> O()

		mm = MOM(medians, setQuant / 2, setQuant);
	}

	for (i = 0; i < size; i++) {
		if (numbers[i] < mm) {
			countLeft++;
		}
		else if(numbers[i] > mm) {
			countRight++;
		}
	}

	int * left = malloc(countLeft * sizeof(int));
	int * right = malloc(countRight * sizeof(int));
	countLeft = 0;
	countRight = 0;

	for (i = 0; i < size; i++) {
		if (numbers[i] < mm) {
			left[countLeft] = numbers[i];
			countLeft++;
		}
		else if(numbers[i] > mm) {
			right[countRight] = numbers[i];
			countRight++;
		}
	}

	//Ordena com Counting Sort e Remove duplicatas nas listas left e right - O(n)
	maxLeft = GetMax(left, countLeft);
	maxRight = GetMax(right, countRight);
	CountingSort(left, countLeft, maxLeft + 1);
	CountingSort(right, countRight, maxRight + 1);

	countLeft = RemoveDuplicates(left, countLeft);
	countRight = RemoveDuplicates(right, countRight);

	if (countLeft == k - 1) {
		return mm;
	}
	else if (countLeft > k - 1) {
		return MOM(left, k, countLeft);
	}
	else if(countLeft < k - 1) {
		return MOM(right, k - countLeft - 1, countRight);
	}
}

int LinearSelection(int * numbers, int k, int size) {
	int i, check, mom, countLeft = 0, countRight = 0, result, max;
	int* array = (int*)malloc(size * sizeof(int));

	for (i = 0; i < size; i++) {
		array[i] = numbers[i];
	}

	//Checa se posição é maior ou menor que o tamanho do vetor
	if (k < 1 || k > size) {
		printf("ERROR: index out of bounds! \n");
		return -1;
	}

	//Checa se vetor está ordenado e se estiver retorna o k-ésimo menor

	if (CheckSorted(numbers, size) == 1) {
		return numbers[k - 1];
	}

	return MOM(array, k, size);

}

//SORTSELECTION PARA TESTE

void Swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

int* BubbleSort(int* numbers, int size)
{
 	int i, j;
	int* result = (int*)malloc(size*sizeof(int));

	for (i = 0; i < size; i++) {
		result[i] = numbers[i];
	}

	for (i = 0; i < size - 1; i++)

		// Last i elements are already in place    
		for (j = 0; j < size - i - 1; j++)
			if (result[j] > result[j + 1])
				Swap(&result[j], &result[j + 1]);

	return result;
}

int SortSelection(int* numbers, int k, int size) {
	int* result = (int*)malloc(size * sizeof(int));

	result = BubbleSort(numbers, size);
	return result[k - 1];
}


int main (void) {
	int k, size, i, j, n, count = 0, repeating, max, instanceQuant, dummy;
	int** instances;
	int result;
	clock_t start, end;
	double cpu_time_usedSort, cpu_time_usedK, mediaSort = 0, mediaK = 0;

	printf("Forneca a quantidade de instancias a serem geradas!\n");
	scanf_s("%d", &instanceQuant);

	printf("Forneca o tamanho n das instancias!\n");
	scanf_s("%d", &n);

	printf("Forneca o valor de k! Insira 0 se deseja k = n/2 !\n");
	scanf_s("%d", &k);

	if (k == 0) {
		k = floor(n / 2);
	}
	
	instances = (int**)malloc(instanceQuant * sizeof(int*));
	size = n;

	for (i = 0; i < instanceQuant; i++) {
		instances[i] = (int*)malloc(n * sizeof(int));
	}

	for (i = 0; i < instanceQuant; i++) {
		for (j = 0; j < n; j++) {
			instances[i][j] = rand() % MAX_VALUE;
		}
	}
	
	for (i = 0; i < instanceQuant; i++) {
		start = clock();
		result = SortSelection(instances[i], k, size);
		end = clock();

		cpu_time_usedSort = (double)(end - start) / CLOCKS_PER_SEC;
		mediaSort += cpu_time_usedSort;

		printf("O %d-esimo menor numero da entrada eh %d. SortSelection demorou %f segundos\n", k, result, cpu_time_usedSort);
	}
	mediaSort = mediaSort / instanceQuant;
	printf("Media de tempo para o SortSelection: %f\n", mediaSort);
	
	for (i = 0; i < instanceQuant; i++) {
		start = clock();
		result = LinearSelection(instances[i], k, size);
		end = clock();

		cpu_time_usedK = (double)(end - start) / CLOCKS_PER_SEC;
		mediaK += cpu_time_usedK;

		printf("O %d-esimo menor numero da entrada eh %d. K-Selection demorou %f segundos\n", k, result, cpu_time_usedK);

	}
	mediaK = mediaK / instanceQuant;
	printf("Media de tempo para o K-Selection: %f\n", mediaK);

	free(instances);
	scanf_s("%d", &dummy); //Para impedir o console de fechar após término do programa

	return 0;
}

