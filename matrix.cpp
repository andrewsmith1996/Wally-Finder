//
//  matrix.cpp
//  assignment
//
//  Created by Andrew Smith on 13/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//
using namespace std;

#include <iostream>
#include "matrix.h"

//Constructor
Matrix::Matrix(int rows, int cols){
    rows = rows;
    cols = cols;
    
    matrixArray = new double*[rows];
    
    for(int i = 0; i < rows; ++i){
        matrixArray[i] = new double[cols];
        for(int j = 0; j < cols; ++j){
            matrixArray[i][j] = 0;
        }
    }
}

Matrix::~Matrix(){
    
    //Delete elements in array
    for(int i = 0; i < rows; ++i){
        delete matrixArray[i];
    }
    
    //Delete whole Matrix array
    delete[] matrixArray;
    
    cout << "Matrix was deallocated memory" << endl;
    
}


double* Matrix::getMatrixArea(int row, int col, int numberRows, int numberCols){
  
    double* matrixArea = new double[numberRows * numberCols];
    
    int count = 0;

    for(int rowcount = row; rowcount < row + (numberRows - 1); rowcount++){
        for(int colcount = col; colcount < col + (numberCols - 1); colcount++){
            matrixArea[count] = matrixArray[rowcount][colcount];
            count++;
        }
    }
    
    return matrixArea;
    
}

double** Matrix::getMatrixArray(){
    return matrixArray;
}


void Matrix::setPixel(int row, int col, int code){
    matrixArray[row][col] = code;
}


double Matrix::workoutSSD(double wallyMatrix[], double sceneMatrix[], int wallyRows, int wallyCols){
   
    double SSD = 0.0;
    
    for(int x = 0; x < wallyRows * wallyCols; x++){
        SSD += (wallyMatrix[x] - sceneMatrix[x]) * (wallyMatrix[x] - sceneMatrix[x]);
    }
    

    return SSD;
}




//Method to print the whole Matrix out
void Matrix::printMatrix(){
   
    for(int col = 0; col < cols; col++){
        for(int row = 0; row < rows; row++){
            cout << matrixArray[col][row] << " ";
        }
        cout << "\n";
       
    }
}


float Matrix::getSSD(){
    return SSD;
}

//Method to set the NNS score of the matrix
void Matrix::setSSD(int NNSscore){
    SSD = NNSscore;
}


