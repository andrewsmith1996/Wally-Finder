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

//Constructor
Matrix::Matrix(int numRows, int numCols){
    rows = numRows;
    cols = numCols;
    
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
    
        
}


float* Matrix::getMatrixArea(int row, int col, int numberRows, int numberCols){
  
    float* matrixArea = new float[numberRows * numberCols];
    
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


float Matrix::workoutSSD(float wallyMatrix[], float sceneMatrix[], int wallyRows, int wallyCols){
   
    float SSD = 0.0, diff = 0.0;
    /*for(int x = 0; x < wallyRows * wallyCols; x++){
        SSD += (wallyMatrix[x] - sceneMatrix[x]) * (wallyMatrix[x] - sceneMatrix[x]);
    }*/
    

    for(int i = 0; i < wallyRows - 1; i++){
        for(int j = 0; j < wallyCols - 1; j++){
            diff = wallyMatrix[wallyCols * i + j] - sceneMatrix[wallyCols * i + j];
            SSD += diff * diff;
        }
    }
        
        
    
    return SSD;
}




float Matrix::workoutNC(float wallyMatrix[], float sceneMatrix[], int wallyRows, int wallyCols){
 
    
    
    float pixelTotalScene = 0.0, pixelTotalWally = 0.0, NC = 0.0, meanOfWally = 0.0, meanOfScene = 0.0, topLine = 0.0, leftSide = 0.0, rightSide = 0.0;
    
    //Calculate sum of scene and wally matrices
    for(int y = 0; y < wallyRows * wallyCols; y++){
        pixelTotalScene += sceneMatrix[y];
        pixelTotalWally += wallyMatrix[y];
    }
    
//    cout << fixed <<"pixelTotalScene : " << pixelTotalScene << endl;
//    cout << fixed <<"pixelTotalWally : " << pixelTotalWally << endl;
    
    meanOfWally = pixelTotalWally / (wallyRows * wallyCols);
    meanOfScene = pixelTotalScene / (wallyRows * wallyCols);
    
//    cout << fixed << "meanOfWally : " << meanOfWally << endl;
//    cout << fixed << "meanOfScene : " << meanOfScene << endl;

    
    //Take away mean from each pixel
    for(int y = 0; y < wallyRows * wallyCols; y++){
        sceneMatrix[y] -= meanOfScene;
        wallyMatrix[y] -= meanOfWally;
    }
    
    
    
    
    for(int y = 0; y < wallyRows * wallyCols; y++){
        topLine += wallyMatrix[y] * sceneMatrix[y];
        leftSide += wallyMatrix[y] * wallyMatrix[y];
        rightSide += sceneMatrix[y] * sceneMatrix[y];
    }
    
//    cout << fixed << "topLine : " << topLine << endl;
//    cout << fixed <<"leftSide : " << leftSide << endl;
//    cout << fixed <<"rightSide : " << rightSide << endl;
    
    NC = topLine / sqrt(leftSide * rightSide);

    cout << fixed << "NC : " << NC << endl;
    
    

    
    return NC;
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
void Matrix::setSSD(double NNSscore){
    SSD = NNSscore;
}


float Matrix::getNC(){
    return NC;
}

//Method to set the NNS score of the matrix
void Matrix::setNC(double NCscore){
    NC = NCscore;
}



