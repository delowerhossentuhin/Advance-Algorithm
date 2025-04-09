#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

using Complex = complex<double>;
const double PI = acos(-1);

// FFT Algorithm (Recursive)
void FFT(vector<Complex>& a, bool invert) {
    size_t n = a.size();
    if (n == 1) return;

    vector<Complex> a_even(n / 2), a_odd(n / 2);
    for (size_t i = 0; i < n / 2; i++) {
        a_even[i] = a[i * 2];
        a_odd[i] = a[i * 2 + 1];
    }

    FFT(a_even, invert);
    FFT(a_odd, invert);

    double angle = 2 * PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));
    for (size_t i = 0; i < n / 2; i++) {
        a[i] = a_even[i] + w * a_odd[i];
        a[i + n / 2] = a_even[i] - w * a_odd[i];
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}

// Generate random dataset (using static allocation to ensure consistency)
void generate_dataset(vector<Complex>& data, size_t size) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1000);

    data.resize(size);
    for (auto& x : data) {
        x = Complex(dis(gen), 0);
    }
}

// Perform FFT and measure time
void perform_fft(size_t n, bool invert) {
    size_t data_size = 1 << n; // Calculate 2^n
    static vector<Complex> data; // Static to reuse memory
    generate_dataset(data, data_size); // Fill dataset

    cout << "Starting FFT computation on dataset of size: " << data_size << "...\n";

    auto start = high_resolution_clock::now();
    FFT(data, invert); // Perform FFT on the dataset
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "FFT computation completed in " << duration.count() << " ms\n";
}

int main() {
    int n;
    char invert_choice;

    while (true) {
        cout << "Enter the value of n (dataset size will be 2^n, or -1 to exit): ";
        cin >> n;

        if (n < 0) {
            cout << "Exiting...\n";
            break;
        }

        cout << "Do you want to perform an inverse FFT? (y/n): ";
        cin >> invert_choice;

        bool invert = (invert_choice == 'y' || invert_choice == 'Y');

        perform_fft(n, invert);
    }

    return 0;
}
