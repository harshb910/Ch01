// Ch01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <immintrin.h>
#include "Ch01_01.h"
#include <chrono>

using namespace std;

void CalcZ_Cpp(float* z, const float* x, const float* y, size_t n)
{
    for (size_t i = 0; i < n; i++)
        z[i] = x[i] + y[i];
}
void CalcZ_Iavx(float* z, const float* x, const float* y, size_t n)
{
    size_t i = 0;
    const size_t num_simd_elements = 8;
    for (; n - i >= num_simd_elements; i += num_simd_elements)
    {
        // Calculate z[i:i+7] = x[i:i+7] + y[i:i+7]
        __m256 x_vals = _mm256_loadu_ps(&x[i]);
        __m256 y_vals = _mm256_loadu_ps(&y[i]);
        __m256 z_vals = _mm256_add_ps(x_vals, y_vals);
        _mm256_storeu_ps(&z[i], z_vals);
    }
    // Calculate z[i] = x[i] + y[i] for any remaining elements
    for (; i < n; i += 1)
        z[i] = x[i] + y[i];
}

int main()
{
    //std::cout << "Hello World!\n";
    float* x = new float[10000];
    float* y = new float[10000];
    float* z = new float[10000];
    for (int i = 0; i < 10000; i++) {
        x[i] = i+1;
        y[i] = i*2;
    }
    size_t size = 10000;
    //cout << size << endl;
    auto start = chrono::high_resolution_clock::now();
    CalcZ_Iavx(z, x, y, size);
    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    cout << "Time taken by Iavx program is : " << fixed << setprecision(9) << time_taken << " sec" << endl;

    start = chrono::high_resolution_clock::now();
    CalcZ_Cpp(z, x, y, size);
    end = chrono::high_resolution_clock::now();
    time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    cout << "Time taken by Cpp program is  : " << fixed << setprecision(9) << time_taken << " sec" << endl;
    
    start = chrono::high_resolution_clock::now();
    CalcZ_Aavx(z, x, y, size);
    end = chrono::high_resolution_clock::now();
    time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    cout << "Time taken by Aavx program is : " << fixed << setprecision(9) << time_taken << " sec" << endl;
    


    //for (int i = 0; i < 100; i++) {
    //    cout << x[i]<<" "<<y[i]<<" "<<z[i] << " \n";
    //}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
