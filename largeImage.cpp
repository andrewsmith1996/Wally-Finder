//
//  matchImage.cpp
//  assignment
//
//  Created by Andrew Smith on 14/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#include "largeImage.h"
#include <iostream>

using namespace std;

LargeImage::LargeImage(){
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


//Draws a line 4 pixels around Wally
double* LargeImage::draw(int finalRow, int finalCol, double* cluttered_scene_input_data, int wallyRows, int wallyCols, int clutteredCols){
    
    //Loop to run through each row and column to edit the data to draw on it
    for(int rowCount = finalRow; rowCount <= finalRow + wallyRows; rowCount++){
        for(int colCount = finalCol; colCount <= finalCol + wallyCols; colCount++){
            
            if(rowCount == finalRow || rowCount == finalRow + wallyRows){
                for(int x = 0; x < 4; x++){
                    cluttered_scene_input_data[clutteredCols * (rowCount - x) + (colCount - x)] = 0;
                }
            }
            
            if(colCount == finalCol || colCount == finalCol + wallyCols){
                for(int x = 0; x < 4; x++){
                    cluttered_scene_input_data[clutteredCols * (rowCount - x) + (colCount - x)] = 0;
                }
            }
        }
    }
    
    return cluttered_scene_input_data;
    
}
