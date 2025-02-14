#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>
#include <iomanip>

#define ENABLE_ShakerSort 1
#define PRINT_LIMIT_SIZE 16

using namespace std;
using namespace chrono;

struct sort_stat
{
	sort_stat() : comparisons(0), swaps(0), duration(0) {}
	long long int comparisons;
	long long int swaps;
	long long int duration;
};

/*enum partition_type
{		//энумерейшен
	ePartition ,   // можно назначить стартовое значение ePartition = 1 и отсчтет будет с единички
	eMedianPartition,
	eHoarePartition
};*/

void RandArr(int* arr, int size, int min, int max);
void PrintArr(int* arr, int size);
int* CreateDublicateArr(int* arr, int size);

void PrintCaptionResult(ofstream& outFile);
void PrintSortResult(int* arr, int size, sort_stat* stat, string alg, ofstream& outFile);

void Compare(int size, int min, int max, ofstream& outFile);
void CompareSortMethods(int* arr, int size, ofstream& outFile, string title);

void ShakerSort(int* arr, int size, sort_stat* stat);

void QuickSortPartition(int* arr, int size, int low, int high, sort_stat* stat);  //, partition_type part_type);
void QuickSortMedian(int* arr, int size, int low, int high, sort_stat* stat);
void QuickSortHoare(int* arr, int size, int low, int high, sort_stat* stat);

int Partition(int* arr, int size, int low, int high, sort_stat* stat);
int MedianPartition(int* arr, int size, int low, int high, sort_stat* stat);
int HoarePartition(int* arr,int size, int low, int high, sort_stat* stat);


void RandArr(int* arr, int size, int min, int max)
{
	//auto begin = stedy_clock::now();
	for (int i = 0; i < size; i++)
	{
		arr[i] = rand() % (max - min + 1) + min;
		cout << arr[i] << " ";
	}
	cout << endl;
}

void ReverseArr(int* arr, int size)
{
	cout << "\nМассив убывающий: " << endl;
	for (int i = 0; i < size / 2; i++)
	{
		int tmp = arr[i];
		arr[i] = arr[size - i - 1];
		arr[size - i - 1] = tmp;
		//cout << arr[i];
	}
	cout << endl;
}

int Partition(int* arr,int size, int low, int high, sort_stat *stat)
{
	int pivot = arr[high];  // опорный элемент последний 
	int i = low - 1;		// индекс меньшего элемента 
	
	for (int j = low; j < high; j++)
	{
		stat->comparisons++;
		if (arr[j] <= pivot)
		{
			i++;
			int tmp = arr[i];  //замена через временнную переменную
			arr[i] = arr[j];
			arr[j] = tmp;
			stat->swaps++;	//увеличиваем счетчик пересылок
		}
	}
	//перемещаем опорный элемент 
	int tmp = arr[i+1];
	arr[i + 1] = arr[high];
	arr[high] = tmp;
	stat->swaps++;
	return i + 1;  //возврат индекса опорного элемента
}

int MedianPartition(int* arr,int size, int low, int high, sort_stat *stat)
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
	stat->swaps++;
	int tmp = arr[medianIndex];
	arr[medianIndex] = arr[high];
	arr[high] = tmp;
	//swap(arr[medianIndex], arr[high]);
	return Partition(arr, size, low, high, stat);
}

int HoarePartition(int* arr,int size, int low, int high, sort_stat *stat)
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

		int tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
		stat->swaps++;
	}
}

/**/void Swap(int& cell1, int& cell2)
{
	int tmp = cell1;
	cell1 = cell2;
	cell2 = tmp;
}

#if ENABLE_ShakerSort
void ShakerSort(int* arr, int size, sort_stat *stat)
{
	int left = 0, right = size - 1;
	bool swapped = true;

	auto start = steady_clock::now();
	while (swapped)
	{
		swapped = false;
	
		for (int i = left; i < right; i++)
		{
			stat->comparisons++;
			if (arr[i] > arr[i + 1])
			{
				int tmp = arr[i + 1];
				arr[i + 1] = arr[i];
				arr[i] = tmp;
				//Swap(arr[i+1],arr[i])
				swapped = true;
				stat->swaps++;
			}
		}
		right--;
	
		for (int i = right; i > left; i--)
		{
			stat->comparisons++;
			if (arr[i] < arr[i - 1])
			{
				int tmp = arr[i - 1]; // можно без  int tmp , Библотека algorithm
				arr[i - 1] = arr[i];  //swap(arr[], arr[i+1])
				arr[i] = tmp;
				//Swap(arr[i-1],arr[i])
				swapped = true;
				stat->swaps++;
			}
		}
		left++;

		// Вывод промежуточного результата
		if( size <= PRINT_LIMIT_SIZE) 
			PrintArr(arr, size);		

		if (!swapped) // swapped = false;
			break;  // Если перестановок не было, массив отсортиров
	}
	auto end = steady_clock::now();
	stat->duration = duration_cast<microseconds>(end - start).count();
	//cout<< "\nВремя выполнения ShakerSort = "<< stat->duration << " микросекунд"<<endl;
	cout << "\nКоличество сравнений: " << stat->comparisons << "\nКоличество пересылок: " << stat->swaps << "\n";
}
#endif

void PrintArr(int* arr, int size)
{
	if (size <= PRINT_LIMIT_SIZE) {
		for (int i = 0; i < size; i++)
		{
			cout << arr[i] << " ";
		}
		cout << endl;
	}
}

void QuickSort(int* arr, int size, int low, int high, sort_stat* stat)  //partition_type
{

void QuickSortPartition(int* arr, int size, int low, int high, sort_stat *stat)  //partition_type
{
	auto start = steady_clock::now();
	if (low < high) {
		int p; // индекс рабиения
		p = Partition(arr, size, low, high, stat);
		QuickSortPartition(arr, size, low, p - 1, stat);
		QuickSortPartition(arr, size, p + 1, high, stat);
	}

	auto end = steady_clock::now();
	stat->duration = duration_cast<microseconds>(end - start).count();
	//cout << "Время выполнения QuickSortPartition = " << stat->duration << " микросекунд" << endl;
	//if ((low == 0) && (high == (size - 1)) && (size <= PRINT_LIMIT_SIZE)) {
	//	PrintArr(arr, size);
	//}
	//cout << "\nКоличество сравнений: " << stat->comparisons<< "\nКоличество пересылок: " << stat->swaps << "\n";
}

void QuickSortMedian(int* arr, int size, int low, int high, sort_stat* stat) //partition_type
{
	auto start = steady_clock::now();
	if (low < high) {
		int p; // индекс рабиения
		if (size <= PRINT_LIMIT_SIZE) {
			PrintArr(arr, size);
			cout << "\n";
		}
		p = MedianPartition(arr, size, low, high, stat);
		QuickSortMedian(arr, size, low, p - 1, stat);
		QuickSortMedian(arr, size, p + 1, high, stat);
	}
	auto end = steady_clock::now();
	stat->duration = duration_cast<microseconds>(end - start).count();
	//cout << "\nВремя выполнения QuickSortMedian =  " << stat->duration << " микросекунд"<<endl;
	if ((low == 0) && (high == (size - 1)) && (size <= PRINT_LIMIT_SIZE)) {
		PrintArr(arr, size);
		cout << "\nКоличество сравнений: " << stat->comparisons << "\nКоличество пересылок: " << stat->swaps << "\n";
	}
}

void QuickSortHoare(int* arr, int size, int low, int high, sort_stat* stat ) //partition_type part_type
{
	auto start = steady_clock::now();
	if (low < high) {
		int p; // индекс рабиения
		if (size <= PRINT_LIMIT_SIZE) {
			PrintArr(arr, size);
			cout << "\n";
		}
		p = HoarePartition(arr, size, low, high, stat);
		QuickSortHoare(arr, size, low, p, stat );
		QuickSortHoare(arr, size, p + 1, high, stat );
	}
	auto end = steady_clock::now();
	stat->duration = duration_cast<microseconds>(end - start).count();
	//cout << "Время выполнения QuickSortHoare  " << stat->duration << " микросекунд" << endl;
	if ((low == 0) && (high == (size - 1)) && (size <= PRINT_LIMIT_SIZE)) {
		PrintArr(arr, size);
		cout << "\nКоличество сравнений: " << stat->comparisons << "\nКоличество пересылок: " << stat->swaps << "\n";
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
	outFile << left << setw(15) << alg
		<< setw(12) << size
		<< setw(12) << stat->swaps
		<< setw(15) << stat->comparisons
		<< setw(15) << stat->duration << endl;
}


void Compare( int size, int min, int max, ofstream& outFile)
{
	int* arr = new int[size];

	if (size <= PRINT_LIMIT_SIZE) {
		cout<< "Генерация случайного массива: \n";
		RandArr(arr,size,min,max);
	}

	CompareSortMethods(arr, size, outFile, "\nСлучайный массив: ");
	//CompareSortMethods(arr, size, outFile, "\nНаилучший случай: ");
	//ReverseArr(arr,size);
	//CompareSortMethods(arr, size, outFile, "\nНаихудший случай: ");
	delete[] arr;
}

void CompareSortMethods(int* arr, int size, ofstream& outFile, string title)
{
	const int N = 4;
	sort_stat stat[N];   // в конструкторе инициализируется нулями
	//duration<double, micro> durations[N]; // для хранения времени

	int* res[N] = {
		CreateDublicateArr(arr, size),
		CreateDublicateArr(arr, size),
		CreateDublicateArr(arr, size),
		CreateDublicateArr(arr, size)
	}; 

	int low = 0, high = size - 1;
	
	if( size <= PRINT_LIMIT_SIZE ) {
		cout<<"\nДля массива элементов N = "<<size <<"\n"<<title;
		PrintArr(arr, size);
	}

	//Замер времени для ShakerSort
    //ShakerSort(res[0], size, &stat[0]);
    // Замер времени для QuickSort с разными разбиениями
	QuickSortPartition(res[1], size, low, high, &stat[1]);
	//QuickSortMedian(res[2], size, low, high, &stat[2]);
	//QuickSortHoare(res[3], size, low, high, &stat[3]);

 	//ShakerSort(res[0], size, &stat[0]);
	//QuickSort(res[1], low, high, &stat[1], ePartition);  //& при вызове функции & оператор взятия адреса
	//QuickSort(res[2], low, high, &stat[2], eMedianPartition);	
	//QuickSort(res[3], low, high, &stat[3], eHoarePartition);  
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
	//cout<<"\nСравнений: "<<stat.comparisons<<".\nПерестановок: "<<stat.swaps ;
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
	//Compare(500, 0, 1000, outFile);
	//Compare(1000, 0, 5000, outFile);
	//Compare(10000, 0, 50000, outFile);
	//Compare(50000, 0, 100000, outFile);
	
	outFile.close();

	return 0;
}

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
