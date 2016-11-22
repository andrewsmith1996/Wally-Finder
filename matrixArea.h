//
//  matrixArea.hpp
//  assignment
//
//  Created by Andrew Smith on 21/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#ifndef matrixArea_h
#define matrixArea_h

#include <stdio.h>
#include "matrix.h"

class MatrixArea : public Matrix{
public:
    
    MatrixArea(int cols, int rows):Matrix(cols, rows){}
    
    void setSSD(int NNSscore);
    float getSSD();
    int getRow();
    int getCol();
    void setStartingRow(int row);
    void setStartingCol(int col);
    
protected:
    float SSD;
    int startingRow;
    int startingCol;
};




#endif /* matrixArea_h */
