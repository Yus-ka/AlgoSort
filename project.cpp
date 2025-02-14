#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <iomanip>

#define ENABLE_ShakerSort 1
#define PRINT_LIMIT_SIZE 16
#define PRINT_INTERMEDIATE_RESULT 1

#pragma comment(linker, "/STACK:50048576")

using namespace std;
using namespace chrono;

int G_arrSize;

struct sort_stat
{
	sort_stat() : comparisons(0), swaps(0), duration(0) {}
	long long int comparisons;
	long long int swaps;
	long long int duration;
	steady_clock::time_point start;
};

void TimeStart(sort_stat& stat)
{
	stat.start = steady_clock::now();
}
void TimeEnd(sort_stat& stat)
{
	auto end = steady_clock::now();
	stat.duration = duration_cast<microseconds>(end - stat.start).count();
}

enum partition_type
{	
	ePartition ,   // можно назначить стартовое значение ePartition = 1 и отсчтет будет с единички
	eMedianPartition,
	eHoarePartition
};

void RandArr(int* arr, int size, int min, int max);
void PrintArr(int* arr, int size);
int* CreateDublicateArr(int* arr, int size);

void Swap(int& cell1, int& cell2, sort_stat* stat);

void PrintCaptionResult(ofstream& outFile);
void PrintSortResult(int* arr, int size, sort_stat* stat, string alg, ofstream& outFile);

void Compare(int size, int min, int max, ofstream& outFile);
void CompareSortMethods(int* arr, int size, ofstream& outFile, string title);

void ShakerSort(int* arr, int size, sort_stat* stat);

void QuickSort(int* arr, int low, int high, sort_stat* stat, partition_type part_type);

int Partition(int* arr, int low, int high, sort_stat* stat);
int MedianPartition(int* arr, int low, int high, sort_stat* stat);
int HoarePartition(int* arr, int low, int high, sort_stat* stat);


void RandArr(int* arr, int size, int min, int max)
{
	for (int i = 0; i < size; i++)
	{
		arr[i] = rand() % (max - min + 1) + min;
	}
}

void ReverseArr(int* arr, int size)
{
	for (int i = 0; i < size / 2; i++)
	{
		int tmp = arr[i];
		arr[i] = arr[size - i - 1];
		arr[size - i - 1] = tmp;
	}
}

int Partition(int* arr, int low, int high, sort_stat *stat)
{
	int pivot = arr[high];  // опорный элемент последний 
	int i = low - 1;		// индекс меньшего элемента 
	
	for (int j = low; j < high; j++)
	{
		stat->comparisons++;
		if (arr[j] < pivot)
		{
			i++;
			//Swap(arr[i], arr[j],stat);
			int tmp = arr[i];
			arr[i ] = arr[j];
			arr[j] = tmp;
			stat->swaps++;
		}
	}
	//перемещаем опорный элемент 
	//Swap(arr[i + 1], arr[high],stat);
	int tmp = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = tmp;
	stat->swaps++;

	return i + 1;  //возврат индекса опорного элемента
}

int MedianPartition(int* arr, int low, int high, sort_stat *stat)
{
	int mid = low +(high - low)/2;
	int medianIndex = mid;
	if ((arr[low] < arr[mid]) ^ (arr[low]< arr[high]))
	{
		medianIndex = low;
	} else if ((arr[mid] < arr[low]) ^ (arr[mid] < arr[high])) {
		medianIndex = mid;
	}	else {
		medianIndex = high;
	}
	int tmp = arr[medianIndex];
	arr[medianIndex] = arr[high];
	arr[high] = tmp;
	stat->swaps++;
	//Swap(arr[medianIndex], arr[high],stat);

	return Partition(arr, low, high, stat);
}

int HoarePartition(int* arr, int low, int high, sort_stat *stat)
{
	int pivot = arr[low];
	int i = low - 1;
	int j = high + 1;
	
	while (true)
	{
		do{
			i++;
			stat->comparisons++;
		} while (arr[i] < pivot);

		do{
			j--;
			stat->comparisons++;
		} while (arr[j] > pivot);\
		
		if (i >= j)
			return j;

		//Swap(arr[i], arr[j],stat);
		int tmp = arr[i];
		arr[i ] = arr[j];
		arr[j] = tmp;
		stat->swaps++;
	}
}

void Swap(int& cell1, int& cell2, sort_stat* stat)
{
	int tmp = cell1;
	cell1 = cell2;
	cell2 = tmp;

	stat->swaps++;
}

#if ENABLE_ShakerSort
void ShakerSort(int* arr, int size, sort_stat *stat)
{
	int left = 0, right = size - 1;
	bool swapped = true;

	while (swapped)
	{
		swapped = false;
	
		for (int i = left; i < right; i++)
		{
			stat->comparisons++;
			if (arr[i] > arr[i + 1])
			{
				//Swap(arr[i+1],arr[i], stat);
				int tmp = arr[i+1];
				arr[i +1 ] = arr[i];
				arr[i] = tmp;
				stat->swaps++;
				swapped = true;
			}
		}
		right--;
	
		for (int i = right; i > left; i--)
		{
			stat->comparisons++;
			if (arr[i] < arr[i - 1])
			{
				//Swap(arr[i - 1], arr[i], stat);
				int tmp = arr[i - 1];
				arr[i - 1] = arr[i];
				arr[i] = tmp;
				stat->swaps++;
				swapped = true;
				
			}
		}
		left++;

		if (!swapped) // swapped = false;
			break;  // Если перестановок не было, массив отсортиров

#if	PRINT_INTERMEDIATE_RESULT
		if( size <= PRINT_LIMIT_SIZE) 
			PrintArr(arr, size);		
#endif
	}
}
#endif

void PrintArr(int* arr, int size)
{

	if (size > PRINT_LIMIT_SIZE) 
		size = PRINT_LIMIT_SIZE;

	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

void PrintArrQuick(int* arr, int low, int high)
{
	for(int i = 0; i < low ; i++)
	{
		cout << left << setw(4) <<" ";
	}

	for (int i = low; i <= high; i++)
	{
		cout << left << setw(4) << arr[i];
	}
	cout << endl;
}

void QuickSort(int* arr, int low, int high, sort_stat* stat, partition_type part_type)
{

	if (low < high) {
#if	PRINT_INTERMEDIATE_RESULT
		
		if (G_arrSize <= PRINT_LIMIT_SIZE) {
			PrintArrQuick(arr, low, high);
		}
#endif
		int p; // индекс рабиения
		if( part_type == ePartition) {
			p = Partition(arr ,low , high, stat);
            QuickSort(arr, low, p - 1, stat, part_type);
            QuickSort(arr, p + 1, high, stat, part_type);
		} else if (part_type == eMedianPartition) {
			p = MedianPartition(arr ,low , high, stat);
            QuickSort(arr, low, p -1, stat, part_type);
            QuickSort(arr, p + 1, high, stat, part_type);
		} else if (part_type == eHoarePartition) {
			p = HoarePartition(arr,low, high, stat);
            QuickSort(arr, low, p, stat, part_type);
            QuickSort(arr, p + 1, high, stat, part_type);
		}		
	}
}

int* CreateDublicateArr(int* arr, int size)
{
	int* copy_arr = new int[size];

	for (int i = 0; i < size; i++)
	{
		copy_arr[i] = arr[i];
	}
	return copy_arr;
}

void PrintCaptionResult(ofstream& outFile)
{
	outFile << left << setw(15) << "Algorithm"
        << setw(12) << "Array_Size"
        << setw(12) << "Swaps "
        << setw(15) << "Comparisons" 
        << setw(15) << "Time(microseconds)"
		<< "\n";
}

void PrintSortResult(int* arr, int size, sort_stat* stat, string alg, ofstream& outFile)
{
	// Вывод статистики на экран
	
	cout << "\n" <<left << setw(15)<<  alg
		<< setw(15)<< "Сравнений: " 
		<< setw(10) << stat->comparisons
		<< setw(15)<< "Перестановок: "
		<< setw(10) << stat->swaps
		<< setw(15) << "Время выполнения: "
		<< setw(10) << stat->duration;

	// Вывод статистики в файл
	outFile << left << setw(15) << alg
		<< setw(12) << size
		<< setw(12) << stat->swaps
		<< setw(15) << stat->comparisons
		<< setw(15) << stat->duration << endl;
}

void PrintTitle(int size, string title)
{
#if	PRINT_INTERMEDIATE_RESULT
	// Вывод промежуточного результата
	if (size <= PRINT_LIMIT_SIZE)
	{
		cout << "\n" << title<<"\n";
	}
#endif
}


void Compare( int size, int min, int max, ofstream& outFile)
{
	G_arrSize = size;
	int* arr = new int[size];

	cout << endl;
	cout << endl;
	cout << "Генерация случайного массива для N= " << size << "\n";
	RandArr(arr,size,min,max);
	PrintArr(arr, size);
		
	CompareSortMethods(arr, size, outFile, "\nСлучайный массив: ");
	CompareSortMethods(arr, size, outFile, "\nНаилучший случай: ");
	ReverseArr(arr,size);
	CompareSortMethods(arr, size, outFile, "\nНаихудший случай: ");
	delete[] arr;
}

void CompareSortMethods(int* arr, int size, ofstream& outFile, string title)
{
	const int N = 4;
	sort_stat stat[N];   // в конструкторе инициализируется нулями

	int* res[N] = {
		CreateDublicateArr(arr, size),
		CreateDublicateArr(arr, size),
		CreateDublicateArr(arr, size),
		CreateDublicateArr(arr, size)
	}; 

	int low = 0, high = size - 1;
	cout << "\n" << title;
	PrintArr(arr, size);

	PrintTitle(size, "ShakerSort");
	TimeStart(stat[0]);
	ShakerSort(res[0], size, &stat[0]);
	TimeEnd(stat[0]);

	PrintTitle(size, "QS_Partition");
	TimeStart(stat[1]);
	QuickSort(res[1], low, high, &stat[1], ePartition);  //& при вызове функции & оператор взятия адреса
	TimeEnd(stat[1]);
	
	PrintTitle(size, "QS_Medium");
	TimeStart(stat[2]);
	QuickSort(res[2], low, high, &stat[2], eMedianPartition);
	TimeEnd(stat[2]);
	
	PrintTitle(size, "QS_Hoare");
	TimeStart(stat[3]);
	QuickSort(res[3], low, high, &stat[3], eHoarePartition);  
	TimeEnd(stat[3]);
	
	//Вернуть сортированный масссив 
	for(int i = 0; i < size; i++)
	{
		arr[i] = res[1][i];
	}

	outFile<<title<<"Для элементов N = "<< size << endl;
	PrintSortResult(res[0], size, &stat[0], "ShakerSort", outFile);
	PrintSortResult(res[1], size, &stat[1], "QS_Partition", outFile);
	PrintSortResult(res[2], size, &stat[2], "QS_Medium", outFile);
	PrintSortResult(res[3], size, &stat[3], "QS_Hoare", outFile);
	
	outFile<<endl;
	//cout<< "\nОтсортированный массив  :\n";

	for (int i = 0; i < N; i++)
	{
		delete[] res[i];
	}
}

int main()
{
	setlocale(0, "ru");
	srand(time(nullptr));

	ofstream outFile("sort_results.txt");
	PrintCaptionResult(outFile);

	Compare(15, 0 , 150, outFile);
	Compare(500, 0, 2000, outFile);
	Compare(1000, 0, 5000, outFile);
	Compare(10000, 0, 50000, outFile);
	Compare(50000, 0, 200000, outFile);

	outFile.close();

	return 0;
}


// итерация
	/*qsort(res[1], size, sizeof(int), [](const void* a, const void* b) {
        return (*(int*)a - *(int*)b); 
    });*/

/*void BubbleSort(int* arr, int size)
{
	while (size > 1)
	{
		bool swapped = false;
		for (int i = 0; i < size - 1 ; i++)
		{
			if (arr[i] > arr[i + 1])
			{
				int tmp = arr[i + 1];
				arr[i + 1] = arr[i];
				arr[i] = tmp;
				swapped = true;
			}
		}
		for (int j = 0; j < size; j++) {
			cout << arr[j] << " ";
		}
		cout << endl;
		if (swapped == false)
			break;
	}
	//size -= 1;
}*/
