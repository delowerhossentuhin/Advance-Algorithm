#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

int kadane(const vector<int>& arr) {
    int maxSum = arr[0];
    int currentSum = 0;

    for (int num : arr) {
        currentSum += num;
        maxSum = max(maxSum, currentSum);
        if (currentSum < 0) {
            currentSum = 0;
        }
    }
    return maxSum;
}

int main() {
    vector<int> arr(1000, -1); // Large array
    arr[5000] = 10;

    auto start = high_resolution_clock::now();
    int result = kadane(arr);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Result: " << result << endl;
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}
