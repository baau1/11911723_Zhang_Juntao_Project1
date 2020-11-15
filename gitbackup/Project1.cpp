#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <omp.h>

using   namespace std;
struct Matrices
{
    int column;
    int row;
    float** data;
};
void multiplication(Matrices a, Matrices b, Matrices c);
int main()

{
    auto now = std::chrono::system_clock::now();
    Matrices a;  Matrices b; Matrices c;
    int I = 3000; int M = 3000; int N = 3000;
    a.column = M; a.row = I; b.column = N;
    b.row = M; c.column = N; c.row = I;
    a.data = new float* [a.row]; b.data = new float* [b.row];
    c.data = new float* [c.row];
    for (int i = 0; i < a.row; i++)
    {
        a.data[i] = new float[a.column];
        c.data[i] = new float[c.column];
    }
    for (int i = 0; i < b.row; i++)
    {
        b.data[i] = new float[b.column];
    }
    a.data[1][2] = 2.3f; a.data[2][1] = 3.0f; b.data[1][2] = 2.0f; b.data[2][1] = 1.1f;
    auto start = std::chrono::steady_clock::now();
    multiplication(a, b, c);
    auto end = std::chrono::steady_clock::now();
    std::cout << endl << "multiplication took "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "µs ≈ "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms ≈ "
        << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s.\n";
    return 0;

}

void multiplication(Matrices a, Matrices b, Matrices c)
{
    if (b.column % 8 == 0)
    {
        #pragma omp parallel for
        for (int i = 0; i < a.row; ++i)
        {
            for (int k = 0; k < a.column; k++)
            {
                float s = a.data[i][k];
                for (int j = 0; j < b.column; j += 8)
                {
                    c.data[i][j] += s * b.data[k][j];
                    c.data[i][j + 1] += s * b.data[k][j + 1];
                    c.data[i][j + 2] += s * b.data[k][j + 2];
                    c.data[i][j + 3] += s * b.data[k][j + 3];
                    c.data[i][j + 4] += s * b.data[k][j + 4];
                    c.data[i][j + 5] += s * b.data[k][j + 5];
                    c.data[i][j + 6] += s * b.data[k][j + 6];
                    c.data[i][j + 7] += s * b.data[k][j + 7];
                }
            }
        }
    }
}
