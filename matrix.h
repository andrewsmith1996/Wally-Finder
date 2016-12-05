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
    
        //Constructors for Matrix, including an overloaded constructor
         Matrix();
         Matrix(int numRows, int numCols);
    
        //Deconstructor for Matrix
        ~Matrix();
    
        //Assignment Operator
        Matrix* operator=(const Matrix& other);
    
        //Print whole Matrix
        void printMatrix();
 
        //Get Individual Elements
        void getIndividualElement(int row, int col);
    
        //Member function to set individual pixels in the program
        void setPixel(int row, int col, int code);
    
        //Member function to return the Matrix
        double* getMatrixArea(int row, int col, int rows, int cols);
    
        //Member functions to workout the scores
        double workoutSSD(double* wallyMatrix, double* sceneMatrix, int wallyRows, int wallyCols);
        double workoutNC(double* wallyMatrix, double* sceneMatrix, int wallyRows, int wallyCols);
    
        //Get and Set methods for the SSD
        void setSSD(double NNSscore);
        double getSSD();
    
        //Get and Set methods for the NC
        void setNC(double NCscore);
        double getNC();
    
        double ** getMatrixArray() const;

    protected:
    
        //Structure of Matrix
        int rows;
        int cols;
    
        //Scores
        double SSD;
        double NC;
    
        //Definition of array rows
        double** matrixArray;
    };


#endif /* matrix_h */
