//
//  matchImage.h
//  assignment
//
//  Created by Andrew Smith on 14/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#ifndef matchImage_h
#define matchImage_h

#include <stdio.h>
#include "matrix.h"
#include "largeImage.h"

class MatchImage : public Matrix{
    public:
    
        //Constructor for the MatchImage, basically from the original matrix
        MatchImage(int cols, int rows):Matrix(cols, rows){}
    
//    bool operator<(MatchImage &compareMatrix);
    
    int getStartingRow();
    int getStartingCol();
    void setStartingRow(int row);
    void setStartingCol(int col);
    void populate(int numberOfRows, int numberOfCols, double** scene);
protected:
    
    int startingRow;
    int startingCol;
  



};





#endif /* matchImage_h */
