#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>

#define PHYSICAL_CORES 4
#define LOGICAL_CORES 8

using namespace std;
using namespace std::chrono;


// void printMatrix(vector<vector<int>> matrix, int matrixDimensions) {

//         for (int i = 0; i < matrixDimensions; i++) {
//             for (int j = 0; j < matrixDimensions; j++) {
//                 cout << matrix[i][j] << "\t";
//             }
//             cout << endl;
//         }
// }

void simpleAlgorithm(int matrixDimensions) {

    int minElemIndex;

    vector<vector<int>> matrix(matrixDimensions, vector<int>(matrixDimensions));

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 99);

    for (int i = 0; i < matrixDimensions; i++) {
        for (int j = 0; j < matrixDimensions; j++) {
            matrix[i][j] = distrib(gen);
        }
    }

    for (int i = 0; i < matrixDimensions; i++) {
        minElemIndex = 0;
        for (int j = 1; j < matrixDimensions; j++) {
            if (matrix[i][j] < matrix[i][minElemIndex]) {
                minElemIndex = j;
            }
        }
        swap(matrix[i][minElemIndex], matrix[i][matrixDimensions - i - 1]);
    }
}

void fillRandomValue(vector<vector<int>>& matrix, int startRow, int endRow) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 99);

    int matrixDimensions = matrix.size();
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < matrixDimensions; ++j) {
            matrix[i][j] = distrib(gen);
        }
    }
}

void processRows(vector<vector<int>>& matrix, int startRow, int endRow) {
    int matrixDimensions = matrix.size();
    for (int i = startRow; i < endRow; ++i) {
        int minElemIndex = 0;
        for (int j = 1; j < matrixDimensions; ++j) {
            if (matrix[i][j] < matrix[i][minElemIndex]) {
                minElemIndex = j;
            }
        }
        swap(matrix[i][minElemIndex], matrix[i][matrixDimensions - i - 1]);
    }
}

void parallelizationAlgorithm (int matrixDimensions, int threadsCount) {

    vector<vector<int>> matrix(matrixDimensions, vector<int>(matrixDimensions));

    vector<thread> threads;

    int rowsPerThread = matrixDimensions / threadsCount;
    int remainingRows = matrixDimensions % threadsCount;

    int startRow = 0;

    for (int i = 0; i < threadsCount; ++i) {
        int endRow = startRow + rowsPerThread;
        if (i < remainingRows) {
            endRow++;
        }

        threads.emplace_back(fillRandomValue, ref(matrix), startRow, endRow);

        startRow = endRow;
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    threads.clear();

    startRow = 0;
    for (int i = 0; i < threadsCount; ++i) {
        int endRow = startRow + rowsPerThread;
        if (i < remainingRows) {
            endRow++;
        }


        threads.emplace_back(processRows, ref(matrix), startRow, endRow);

        startRow = endRow;
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}

void results() {

    vector<int> matrixDimensions= {1000, 5000, 10000, 20000};

    vector<int> threadsCount = {
        PHYSICAL_CORES / 2, 
        PHYSICAL_CORES, 
        LOGICAL_CORES, 
        LOGICAL_CORES * 2, 
        LOGICAL_CORES * 4, 
        LOGICAL_CORES * 8, 
        LOGICAL_CORES * 16
    };

    cout << "Size\t|Threads | Time" << endl;
    cout << "------------------------" << endl;

    for (int i = 0; i < matrixDimensions.size(); i++) {

        auto start = high_resolution_clock::now();

        simpleAlgorithm(matrixDimensions[i]);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        cout << matrixDimensions[i] << "\t| " << 1 << "\t |" << duration.count() << " ms" << endl;

        for (int j = 0; j < threadsCount.size(); j++) {

            auto start = high_resolution_clock::now();

            parallelizationAlgorithm(matrixDimensions[i], threadsCount[j]);

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);

            cout << matrixDimensions[i] << "\t| " << threadsCount[j] << "\t |" << duration.count() << " ms" << endl;
        }

        cout << "- - - - - - - - - - - - " << endl;
    }
}

int main() {

    results();

    return 0;
}
