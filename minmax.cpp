#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int arr[] = {3, 7, 2, 9, 4, 1};
    int n = 6;

    int min_val = arr[0];
    int max_val = arr[0];
    int sum = 0;

    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
        sum += arr[i];
    }

    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << (float)sum / n << endl;

    return 0;
}
