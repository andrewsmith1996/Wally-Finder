//
//  matrix.hpp
//  assignment
//
//  Created by Andrew Smith on 13/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#ifndef matrix_h
#define matrix_h

#include <stdio.h>
#include <array>

using namespace std;

//Main Matrix Class

class Matrix{
    public:
    
        //Constructor for Matrix
        Matrix(int numRows, int numCols);
  
        //Deconstructor for Matrix
        ~Matrix();
    
        //Print whole Matrix
        void printMatrix();
 
        //Get Individual Elements
        void getIndividualElement(int row, int col);
    
        void setPixel(int row, int col, int code);
    
        float* getMatrixArea(int row, int col, int rows, int cols);
    
        float workoutSSD(float wallyMatrix[], float sceneMatrix[], int wallyRows, int wallyCols);
        float workoutNC(float wallyMatrix[], float sceneMatrix[], int wallyRows, int wallyCols);
    
        
    
        void setSSD(double NNSscore);
        float getSSD();
    
        void setNC(double NCscore);
        float getNC();
    
        double ** getMatrixArray();
    


    protected:
    
        int rows;
        int cols;
    
        float SSD;
        float NC;
    
        //Definition of array rows
        double** matrixArray;
    };


#endif /* matrix_h */
