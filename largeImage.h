//
//  largeImage_h
//  assignment
//
//  Created by Andrew Smith on 14/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#ifndef largeImage_h
#define largeImage_h

#include <stdio.h>
#include "matrix.h"

class LargeImage : public Matrix{
    public:
        //Constructor
        LargeImage(int cols, int rows):Matrix(cols, rows){}
        LargeImage();
    
        //Member function to actually draw on the images
        double* draw(int finalRow, int finalCol, double* cluttered_scene_input_data, int wallyRows, int wallyCols, int clutteredCols);
};


#endif /* largeImage_h */
