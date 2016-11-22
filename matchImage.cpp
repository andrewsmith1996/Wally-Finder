//
//  matchImage.cpp
//  assignment
//
//  Created by Andrew Smith on 14/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#include "matchImage.h"
#include "largeImage.h";
#include <iostream>

using namespace std;


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

void MatchImage::populate(int numberOfRows, int numberOfCols, double** scene){
    for(int rowCount = startingRow; rowCount < startingRow + numberOfRows; rowCount++){
        for(int colCount = startingCol; colCount < startingCol + numberOfCols; colCount++){
            matrixArray[rowCount][colCount] = matrixArray[rowCount][colCount];
        }
    }
}



