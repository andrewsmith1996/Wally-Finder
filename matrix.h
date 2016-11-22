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
        Matrix(int r, int c);
  
        //Deconstructor for Matrix
        ~Matrix();
    
        //Print whole Matrix
        void printMatrix();
 
        //Get Individual Elements
        void getIndividualElement(int row, int col);
    
        void setPixel(int row, int col, int code);
    
        double* getMatrixArea(int row, int col, int rows, int cols);
    
        double workoutSSD(double wallyMatrix[], double sceneMatrix[], int wallyRows, int wallyCols);
    
        void setSSD(int NNSscore);
        float getSSD();
    
    double ** getMatrixArray();
    


    protected:
    
        int rows;
        int cols;
    
        float SSD;
           
        //Definition of array rows
        double** matrixArray;
    };


#endif /* matrix_h */
