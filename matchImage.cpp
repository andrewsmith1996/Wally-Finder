//
//  matchImage.cpp
//  assignment
//
//  Created by Andrew Smith on 14/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#include "matchImage.h"
#include "largeImage.h"
#include <iostream>

//Default constructor when not using the specified constructor
MatchImage::MatchImage(){
    rows = 49;
    cols = 36;
    
    matrixArray = new double*[rows];
    
    for(int i = 0; i < rows; ++i){
        matrixArray[i] = new double[cols];
        for(int j = 0; j < cols; ++j){
            matrixArray[i][j] = 0.0;
        }
    }
}

//Copy Constructor for creating identical Matrices
MatchImage::MatchImage(const MatchImage& temp){
    rows = 49;
    cols = 36;
    
    matrixArray = new double*[rows];
    
    for(int i = 0; i < rows; ++i){
        matrixArray[i] = new double[cols];
        for(int j = 0; j < cols; ++j){
            matrixArray[i][j] = temp.getMatrixArray()[i][j];
        }
    }
    
}

//Assignment operator for creating identical Matrices
MatchImage* MatchImage::operator=(const MatchImage& previousMatrix){
   
    matrixArray = new double*[rows];
    
    for(int i = 0; i < rows; ++i){
        matrixArray[i] = new double[cols];
        for(int j = 0; j < cols; ++j){
            matrixArray[i][j] = previousMatrix.getMatrixArray()[i][j];
        }
    }
    
    return this;
}


int MatchImage::getStartingRow(){
    return startingRow;
}

int MatchImage::getStartingCol(){
    return startingCol;
}

void MatchImage::setStartingRow(int row){
    startingRow = row;
}

void MatchImage::setStartingCol(int col){
    startingCol = col;
}

void MatchImage::populate(int numberOfRows, int numberOfCols, double** scene, int code){
    for(int rowCount = startingRow; rowCount < startingRow + numberOfRows; rowCount++){
        for(int colCount = startingCol; colCount < startingCol + numberOfCols; colCount++){
            matrixArray[rowCount][colCount] = code;
        }
    }
}



