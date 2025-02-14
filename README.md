Code Overview: Sorting Algorithm Performance Comparison

  This C++ program compares different sorting algorithms by measuring and outputting key performance metrics such as the number of comparisons, swaps, and execution time. It includes QuickSort (with three types of partitioning methods), ShakerSort, and provides functionality for generating random arrays and printing results to the console and a file.

Key Components:

  Sorting Algorithms:
    ShakerSort: A variation of bubble sort that sorts in both directions.
    QuickSort: Implements three partitioning methods:
      Partition (standard).
      MedianPartition (selects the median of the first, middle, and last element as pivot).
      HoarePartition (uses a different strategy for pivoting).
      
Data Generation:
    RandArr: Random array generation within a specified range.
    ReverseArr: Reverses an array to test sorting on worst-case data.
    
Performance Tracking:
    sort_stat struct: Stores the statistics for sorting (comparisons, swaps, execution time).
    TimeStart/TimeEnd: Used for capturing and calculating execution time.
    
Functionality:
    The program generates arrays of various sizes and tests the performance of sorting algorithms on            random, best-case (sorted), and worst-case (reversed) data.
    
A detailed report is generated in sort_results.txt with the sorting algorithm name, array size, swaps, comparisons, and execution time.
