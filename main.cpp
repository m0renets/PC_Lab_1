#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

void printMatrix(int **matrix, int matrixDimensions) {

        for (int i = 0; i < matrixDimensions; i++) {
            for (int j = 0; j < matrixDimensions; j++) {
                cout << matrix[i][j] << "\t";
            }
            cout << endl;
        }
}

void simpleAlgorithm() {

    const int matrixDimensions = 5;
    int ** matrix = new int*[matrixDimensions];
    for (int i = 0; i < matrixDimensions; i++) {
        matrix[i] = new int[matrixDimensions];
    }

    // int matrix[matrixDimensions][matrixDimensions];

    int minElemIndex;

    for (int i = 0; i < matrixDimensions; i++) {
        for (int j = 0; j < matrixDimensions; j++) {
            matrix[i][j] = rand() % 100;
        }
    }

    printMatrix(matrix, matrixDimensions);
    cout <<  endl;

    for (int i = 0; i < matrixDimensions; i++) {
        minElemIndex = 0;
        for (int j = 1; j < matrixDimensions; j++) {
            if (matrix[i][j] < matrix[i][minElemIndex]) {
                minElemIndex = j;
            }
        }
        swap(matrix[i][minElemIndex], matrix[i][matrixDimensions - i - 1]);
    }

    printMatrix(matrix, matrixDimensions);
    cout <<  endl;

}

int main() {
    
    srand(time(0));
    simpleAlgorithm();

    return 0;
}
