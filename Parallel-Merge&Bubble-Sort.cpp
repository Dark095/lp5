#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
using namespace std;

void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        #pragma omp parallel for
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> left(n1), right(n2);

    for (int i = 0; i < n1; i++) left[i] = arr[l + i];
    for (int j = 0; j < n2; j++) right[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < n1) arr[k++] = left[i++];
    while (j < n2) arr[k++] = right[j++];
}

void sequentialMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void parallelMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);

            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

void printArray(const vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    int n = arr.size();

    // Sequential Bubble Sort
    vector<int> arrBubbleSeq = arr;
    auto start = chrono::high_resolution_clock::now();
    sequentialBubbleSort(arrBubbleSeq);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> seqBubbleTime = end - start;
    cout << "Sequential Bubble Sort: ";
    printArray(arrBubbleSeq);
    cout << "Time taken (Sequential Bubble Sort): " << seqBubbleTime.count() << " seconds" << endl;

    // Parallel Bubble Sort
    vector<int> arrBubblePar = arr;
    start = chrono::high_resolution_clock::now();
    parallelBubbleSort(arrBubblePar);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> parBubbleTime = end - start;
    cout << "Parallel Bubble Sort: ";
    printArray(arrBubblePar);
    cout << "Time taken (Parallel Bubble Sort): " << parBubbleTime.count() << " seconds" << endl;

    // Sequential Merge Sort
    vector<int> arrMergeSeq = arr;
    start = chrono::high_resolution_clock::now();
    sequentialMergeSort(arrMergeSeq, 0, n - 1);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> seqMergeTime = end - start;
    cout << "Sequential Merge Sort: ";
    printArray(arrMergeSeq);
    cout << "Time taken (Sequential Merge Sort): " << seqMergeTime.count() << " seconds" << endl;

    // Parallel Merge Sort
    vector<int> arrMergePar = arr;
    start = chrono::high_resolution_clock::now();
    parallelMergeSort(arrMergePar, 0, n - 1);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> parMergeTime = end - start;
    cout << "Parallel Merge Sort: ";
    printArray(arrMergePar);
    cout << "Time taken (Parallel Merge Sort): " << parMergeTime.count() << " seconds" << endl;

    return 0;
}
