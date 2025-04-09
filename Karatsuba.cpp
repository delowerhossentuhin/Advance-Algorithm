#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace chrono;
// Function to generate a random number of given length
string generateRandomNumber(int length) {
    string num;
    num += (rand() % 9 + 1) + '0'; // Ensure first digit is non-zero
    for (int i = 1; i < length; i++) {
        num += (rand() % 10) + '0';
    }
    return num;
}
// Function to add two large numbers represented as strings
string addStrings(const string &num1, const string &num2) {
    string result;
    int carry = 0, sum = 0;
    int len1 = num1.size(), len2 = num2.size();
    int i = len1 - 1, j = len2 - 1;
    while (i >= 0 || j >= 0 || carry > 0) {
        sum = carry;
        if (i >= 0) sum += num1[i--] - '0';
        if (j >= 0) sum += num2[j--] - '0';
        result.push_back((sum % 10) + '0');
        carry = sum / 10;
    }
    reverse(result.begin(), result.end());
    return result;
}
// Function to subtract two large numbers represented as strings
string subtractStrings(const string &num1, const string &num2) {
    string result;
    int borrow = 0, diff = 0;
    int len1 = num1.size(), len2 = num2.size();
    int i = len1 - 1, j = len2 - 1;
    while (i >= 0 || j >= 0) {
        diff = (i >= 0 ? num1[i--] - '0' : 0) - (j >= 0 ? num2[j--] - '0' : 0) - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(diff + '0');
    }
    while (result.size() > 1 && result.back() == '0') result.pop_back();
    reverse(result.begin(), result.end());
    return result;
}
// Function to multiply a number by 10^n
string multiplyByPowerOf10(const string &num, int n) {
    return num + string(n, '0');
}
// Karatsuba multiplication function
string karatsuba(const string &x, const string &y) {
    int len1 = x.size(), len2 = y.size();
    if (len1 == 1 && len2 == 1) {
        return to_string((x[0] - '0') * (y[0] - '0'));
    }
    int maxLength = max(len1, len2);
    if (maxLength % 2 != 0) maxLength++;
    string xPadded = string(maxLength - len1, '0') + x;
    string yPadded = string(maxLength - len2, '0') + y;
    int mid = maxLength / 2;
    string high1 = xPadded.substr(0, mid);
    string low1 = xPadded.substr(mid);
    string high2 = yPadded.substr(0, mid);
    string low2 = yPadded.substr(mid);
    string z0 = karatsuba(low1, low2);
    string z1 = karatsuba(addStrings(low1, high1), addStrings(low2, high2));
    string z2 = karatsuba(high1, high2);
    string result = addStrings(
        addStrings(multiplyByPowerOf10(z2, 2 * (maxLength - mid)), 
                   multiplyByPowerOf10(subtractStrings(subtractStrings(z1, z2), z0), maxLength - mid)), 
        z0
    );
    return result;
}
int main() {
    srand(time(0));
    int size1, size2;
    cout << "Enter size of first number: ";
    cin >> size1;
    cout << "Enter size of second number: ";
    cin >> size2;
    string num1 = generateRandomNumber(size1);
    string num2 = generateRandomNumber(size2);
    cout << "Generated Number 1: " << num1 << endl;
    cout << "Generated Number 2: " << num2 << endl;
    auto start = high_resolution_clock::now();
    string result = karatsuba(num1, num2);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Product: " << result << endl;
    cout << "Time taken: " << duration.count() << " ms" << endl;
    return 0;
}