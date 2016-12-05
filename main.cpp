//
//  main.cpp
//  tasks
//
//  Created by Andrew Smith on 06/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#include <array>
#include <iostream>

#include <sstream> // stringstream
#include <fstream> // ifstream
#include <istream>
#include <vector>
#include <algorithm>

#include "matrix.h"
#include "matchImage.h"
#include "largeImage.h"


using namespace std;

//Prototypes

//Reads .txt file and converts it to a 1D array of doubles of size R*C
double* readTXT(string fileName, int sizeR, int sizeC);

//Converts 1D array of doubles to .pgm image. Use Q = 255 for greyscale images and Q=1 for binary images.
void WritePGM(string filename, double *data, int sizeR, int sizeC, int Q);

//Prints the Progress of image
void printProgress(int rowCount, int colCount, int clutteredCols, int clutteredRows);


int main() {
    
    //M and N represent the number of rows and columns in the image. Cluttered_scene, M = 768, N = 1024, Wally_grey, M = 49, N =  36
    int clutteredRows = 768, clutteredCols = 1024, wallyRows = 49, wallyCols = 36, comparisons = 0;
    double SSD, NC;
    
    //Initiate the Matrices
    MatchImage* wallyImage = new MatchImage();
    LargeImage* sceneImage = new LargeImage(clutteredRows, clutteredCols);
    
    //File names for reading in the images from the text files
    string inputFileName = "Cluttered_scene.txt", wallyInputFileName = "Wally_Grey.txt";
    
    //Creates pointers to 1D arrays of doubles read in from the text files
    double* cluttered_scene_input_data_NNS = 0;
    double* cluttered_scene_input_data_NC = 0;
    double* wally_input_data = 0;
    double* matrixAtArea;
   
    //input_data to hold the colour codes of the text files
    cluttered_scene_input_data_NNS = readTXT(inputFileName, clutteredRows, clutteredCols);
    cluttered_scene_input_data_NC = readTXT(inputFileName, clutteredRows, clutteredCols);
    wally_input_data = readTXT(wallyInputFileName, wallyRows, wallyCols);
    

    
    //Sets the colour codes to the Matrix for the Wally matrix
    int count = 0;
    for(int rowcount = 0; rowcount < wallyRows; rowcount++){
        for(int colcount = 0; colcount < wallyCols; colcount++){
            wallyImage->setPixel(rowcount, colcount, wally_input_data[count]);
            count++;
        }
    }
    
    
    //Sets the colour codes to the Matrix for the cluttered scene
    count = 0;
    for(int rowcount = 0; rowcount < clutteredRows; rowcount++){
        for(int colcount = 0; colcount < clutteredCols; colcount++){
            sceneImage->setPixel(rowcount, colcount, cluttered_scene_input_data_NNS[count]);
            sceneImage->setPixel(rowcount, colcount, cluttered_scene_input_data_NC[count]);
            count++;
        }
    }
    

    
    //Arrays that now contain the function section matrices
    double* wallyMatrixArea = wallyImage->getMatrixArea(0, 0, wallyRows, wallyCols);

    //Create the temporary objects using a copy constructor
    MatchImage* tempMatrixObjectNNS = new MatchImage();
    MatchImage tempMatrixObjectNC = *tempMatrixObjectNNS;
    
    //Can also be done with assignment operator
    //MatchImage* tempMatrixObjectNC = tempMatrixObjectNNS;

    cout << "Searching for Wally..." << endl << endl;

    //Main loop for running through the cluttered scene
    for(int rowCount = 0; rowCount < clutteredRows - wallyRows; rowCount++){
        for(int colCount = 0; colCount < clutteredCols - wallyCols; colCount++){
            
                //printProgress(rowCount, colCount, clutteredCols, clutteredRows);

                //Get 1D array of the scene area
                matrixAtArea = sceneImage->getMatrixArea(rowCount, colCount, wallyRows, wallyCols);
            
                //Workout SSD of the scene area
                SSD = sceneImage->workoutSSD(wallyMatrixArea, matrixAtArea, wallyRows, wallyCols);
            
                //Workout NC of the scene area
                NC = sceneImage->workoutNC(wallyMatrixArea, matrixAtArea, wallyRows, wallyCols);
            
                //Delete Matrix Area
                delete [] matrixAtArea;
            
                //Set initial objects if at the first row and col
                if(rowCount == 0 && colCount == 0){
                    tempMatrixObjectNNS->setStartingCol(colCount);
                    tempMatrixObjectNNS->setStartingRow(rowCount);
                    tempMatrixObjectNNS->setSSD(SSD);
                
                    tempMatrixObjectNC.setStartingCol(colCount);
                    tempMatrixObjectNC.setStartingRow(rowCount);
                    tempMatrixObjectNC.setNC(NC);
                }
            
            
                //Check if new SSD is smaller than what's stored in the current object, if it is then override the object, else the loop continues
                if(SSD < tempMatrixObjectNNS->getSSD()){
                    tempMatrixObjectNNS->setSSD(SSD);
                    tempMatrixObjectNNS->setStartingRow(rowCount);
                    tempMatrixObjectNNS->setStartingCol(colCount);
                }
            
                if(NC > tempMatrixObjectNC.getNC()){
                    tempMatrixObjectNC.setNC(NC);
                    tempMatrixObjectNC.setStartingRow(rowCount);
                    tempMatrixObjectNC.setStartingCol(colCount);
                }
            
            
            comparisons++;

        }
        
    }
    
    delete [] wallyMatrixArea;
    
    cout << "Comparisons: " << comparisons << endl << endl;

        //Edit the cluttered scene inputs to where the program thinks Wally is at
        cluttered_scene_input_data_NNS = sceneImage->draw(tempMatrixObjectNNS->getStartingRow(), tempMatrixObjectNNS->getStartingCol(), cluttered_scene_input_data_NNS, wallyRows, wallyCols, clutteredCols);
    
        cluttered_scene_input_data_NC = sceneImage->draw(tempMatrixObjectNC.getStartingRow(), tempMatrixObjectNC.getStartingCol(), cluttered_scene_input_data_NC, wallyRows, wallyCols, clutteredCols);
    
        //Write out the new scene showing where wally is. Q = 255 for greyscale images and 1 for binary images.
        int Q = 255;
    
        //Output filenames
        string outputFileName_NNS = "SSD_result.pgm", outputFileName_NC = "NC_result.pgm";
    
        //Actually write data to the files
        WritePGM(outputFileName_NNS, cluttered_scene_input_data_NNS, clutteredRows, clutteredCols, Q);
        WritePGM(outputFileName_NC, cluttered_scene_input_data_NC, clutteredRows, clutteredCols, Q);
    
        //Delete objects
        delete [] cluttered_scene_input_data_NNS;
   
        delete [] cluttered_scene_input_data_NC;

        delete [] wally_input_data;
  
        delete wallyImage;
  
        delete sceneImage;
    
        delete tempMatrixObjectNNS;
    
    
    cout << "Processing complete!" << endl << endl;
        
    return 0;
}


// Read .txt file with image of size RxC, and convert to an array of doubles
double* readTXT(string fileName, int sizeR, int sizeC){
    
    //Data Array - 1D Array of doubles
    double* data = new double[sizeR*sizeC];
   
    int i = 0;
   
    ifstream myfile;
    myfile.open(fileName, ios::in);
   
    if (myfile.is_open()){
        
        while (myfile.good()){
            //If I reaches the end of the Matrix
            if (i>sizeR*sizeC-1) break;
            myfile >> *(data+i);
            //cout << *(data+i) << ' '; // This line display the converted data on the screen, you may comment it out.
            i++;
        }
        
        myfile.close();
    }
    
    else{
        cout << "Unable to open file" << endl;
    }
    
    
    return data;
}

// convert data from double to .pgm stored in filename
void WritePGM(string filename, double *data, int sizeR, int sizeC, int Q){
    
    int i, j;
    unsigned char *image;
    
    ofstream myfile;
    
    image = (unsigned char *) new unsigned char [sizeR*sizeC];
    
    // convert the integer values to unsigned char
    
    for(i=0; i<sizeR*sizeC; i++)
        image[i]=(unsigned char)data[i];
    
    myfile.open(filename, ios::out|ios::binary|ios::trunc);
    
    if (!myfile) {
        cout << "Can't open file: " << filename << endl;
        exit(1);
    }
    
    myfile << "P5" << endl;
    myfile << sizeC << " " << sizeR << endl;
    myfile << Q << endl;
    
    myfile.write( reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));
    
    if (myfile.fail()) {
        cout << "Can't write image " << filename << endl;
        exit(0);
    }
    
    myfile.close();
    
    delete [] image;
    
}

void printProgress(int rowCount, int colCount, int clutteredCols, int clutteredRows){
    //Print % of being done
    double pos = (rowCount * clutteredCols  + colCount);
    printf("\rSearching: %.2f%%", (pos / (clutteredCols * clutteredRows) * 100));
    cout << endl;

}


