//
//  matrixArea.cpp
//  assignment
//
//  Created by Andrew Smith on 21/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#include "matrixArea.h"

//
//  matchImage.cpp
//  assignment
//
//  Created by Andrew Smith on 14/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#include "matrixArea.h"
#include <iostream>

using namespace std;

//Method to get the NNS of the Matrix
float MatrixArea::getSSD(){
    return SSD;
}

//Method to set the NNS score of the matrix
void MatrixArea::setSSD(int NNSscore){
    SSD = NNSscore;
}


int MatrixArea::getRow(){
    return startingRow;
}

int MatrixArea::getCol(){
    return startingCol;
}

void MatrixArea::setStartingRow(int row){
    startingRow = row;
}

void MatrixArea::setStartingCol(int col){
    startingCol = col;
}

