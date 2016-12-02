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
#include "math.h"

//Default constructor creates a 1x1 Matrix
Matrix::Matrix(){
    rows = 1;
    cols = 1;
    
    matrixArray = new double*[rows];
    
    for(int i = 0; i < rows; ++i){
        matrixArray[i] = new double[cols];
        for(int j = 0; j < cols; ++j){
            matrixArray[i][j] = 0.0;
        }
    }
}

//Overloaded constructor for specifying the Matrix dimensions
Matrix::Matrix(int numRows, int numCols){
    rows = numRows;
    cols = numCols; 
    
    matrixArray = new double*[rows];
    
    for(int i = 0; i < rows; ++i){
        matrixArray[i] = new double[cols];
        for(int j = 0; j < cols; ++j){
            matrixArray[i][j] = 0.0;
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
        
}


double* Matrix::getMatrixArea(int row, int col, int numberRows, int numberCols){
  
    double* matrixArea = new double[numberRows * numberCols];
    
    int count = 0;
    
    
    for(int rowcount = row; rowcount < row + numberRows; rowcount++){
        for(int colcount = col; colcount < col + numberCols; colcount++){
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


double Matrix::workoutSSD(double* wallyMatrix, double* sceneMatrix, int wallyRows, int wallyCols){
    
    double SSD = 0.0, diff = 0.0;
  
    for(int i = 0; i < wallyRows * wallyCols; i++){
            if(wallyMatrix[i] != 255){
                diff = wallyMatrix[i] - sceneMatrix[i];
                SSD += diff * diff;
            }
    }
    
    return SSD;
}


double Matrix::workoutNC(double* wallyMatrix, double* sceneMatrix, int wallyRows, int wallyCols){
 
    //Deep copy
    double* tempWallyMatrix = new double[wallyRows * wallyCols];
    
    for(int ii = 0; ii < wallyRows * wallyCols; ii++){
            tempWallyMatrix[ii] = wallyMatrix[ii];
        
    }
    
    double pixelTotalScene = 0.0, pixelTotalWally = 0.0, NC = 0.0, meanOfWally = 0.0, meanOfScene = 0.0, topLine = 0.0, leftSide = 0.0, rightSide = 0.0;
    
    //Calculate sum of scene and wally matrices
    for(int y = 0; y < wallyRows * wallyCols; y++){
            pixelTotalWally += tempWallyMatrix[y];
            pixelTotalScene += sceneMatrix[y];
    }
    
    meanOfWally = (1 / (wallyRows * wallyCols)) * pixelTotalWally;
    meanOfScene = (1 / (wallyRows * wallyCols)) * pixelTotalScene;

    //Take away mean from each pixel
    for(int y = 0; y < wallyRows * wallyCols; y++){
            tempWallyMatrix[y] -= meanOfWally;
            sceneMatrix[y] -= meanOfScene;
        
    }

    for(int y = 0; y < wallyRows * wallyCols; y++){
            topLine += sceneMatrix[y] * tempWallyMatrix[y];
            
    }
    
    delete[] tempWallyMatrix;


    NC = topLine / sqrt((pixelTotalWally * pixelTotalWally) * (pixelTotalScene * pixelTotalScene));
    

    
    return NC;
}



//Method to print the whole Matrix out
void Matrix::printMatrix(){
   
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            cout << matrixArray[row][col] << " ";
        }
        cout << "\n";
       
    }
}


double Matrix::getSSD(){
    return SSD;
}

//Method to set the NNS score of the matrix
void Matrix::setSSD(double NNSscore){
    SSD = NNSscore;
}


double Matrix::getNC(){
    return NC;
}

//Method to set the NNS score of the matrix
void Matrix::setNC(double NCscore){
    NC = NCscore;
}



