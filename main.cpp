//
//  main.cpp
//  tasks
//
//  Created by Andrew Smith on 06/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

//Try and Catch
//Structs?
//Template
//Rules



#include <array>
#include <iostream>

#include <sstream> // stringstream
#include <fstream> // ifstream
#include <istream>
#include <vector>
#include <algorithm>
#include <string>

#include "matrix.h"
#include "matchImage.h"
#include "largeImage.h"


using namespace std;

//Prototypes

//Reads .txt file and converts it to a 1D array of doubles of size R*C
double* readTXT(string fileName, int sizeR, int sizeC);

//Converts 1D array of doubles to .pgm image. Use Q = 255 for greyscale images and Q=1 for binary images.
void WritePGM(string filename, double *data, int sizeR, int sizeC, int Q);

//Gets the search algorithm choice from the user
int getAlgorithm();

//Gets how the user wants to search the image
int getSearchArea();

//Function for converting the pixels from the file images into the objects
void convertPixels(int rows, int cols, double* data, MatchImage* image);

//Overloaded function for the convertPixels function
void convertPixels(int rows, int cols, double* data, LargeImage* image);

int main() {
    
    //M and N represent the number of rows and columns in the image
    const int clutteredRows = 768, clutteredCols = 1024, wallyRows = 49, wallyCols = 36;
    int comparisons = 0;
    double SSD, NC;
    
    //Initiate the Matrices from the classes
    MatchImage* wallyImage = new MatchImage();
    LargeImage* sceneImage = new LargeImage(clutteredRows, clutteredCols);
    
    //File names for reading in the images from the text files
    const string inputFileName = "Cluttered_scene.txt", wallyInputFileName = "Wally_Grey.txt";
    
    //Creates pointers to 1D arrays of doubles read in from the text files
    double* cluttered_scene_input_data_SSD = 0;
    double* cluttered_scene_input_data_NC = 0;
    double* wally_input_data = 0;
    double* matrixAtArea;
   
    //input_data to hold the colour codes of the text files
    cluttered_scene_input_data_SSD = readTXT(inputFileName, clutteredRows, clutteredCols);
    cluttered_scene_input_data_NC = readTXT(inputFileName, clutteredRows, clutteredCols);
    wally_input_data = readTXT(wallyInputFileName, wallyRows, wallyCols);
    
    //Sets the colour codes to the Matrix for the cluttered scene, also resets the value count
    convertPixels(wallyRows, wallyCols, wally_input_data, wallyImage);
    convertPixels(clutteredRows, clutteredCols, cluttered_scene_input_data_SSD, sceneImage);
    convertPixels(clutteredRows, clutteredCols, cluttered_scene_input_data_NC, sceneImage);
    
    delete [] wally_input_data;
    
    //1D Array that now contain the Matrix of the Wally image
    double* wallyMatrixArea = wallyImage->getMatrixArea(0, 0, wallyRows, wallyCols);
    
    delete wallyImage;

    //Create the temporary objects using a copy constructor
    MatchImage* tempMatrixObjectSSD = new MatchImage();
    MatchImage tempMatrixObjectNC = *tempMatrixObjectSSD;
    
    
    //*  *  *  *  *  * NOTE  *  *  *  *  *  *  *
    //Can also be done with this assignment operator overload by uncommenting the following line and changing the pointer functions from . to ->
    //MatchImage* tempMatrixObjectNC = tempMatrixObjectSSD;
    //*  *  *  *  *  * NOTE  *  *  *  *  *  *  *

    //Prepares the basic variales for the main loop
    int algorithmChoice = getAlgorithm(), area = getSearchArea(), thresholdCols, thresholdRows;
    
    //Assign the threshold variables for searching the image
    switch(area){
        case 1:
            thresholdRows = wallyRows / 3;
            thresholdCols = wallyCols / 2;
            break;
        case 2:
            thresholdRows = wallyRows / 2;
            thresholdCols = wallyCols / 2;
            break;
        case 3:
            thresholdCols = 1;
            thresholdRows = 1;
            break;
        default:
            cout << "ERROR - Please Restart the Program" << endl;
    }
    
    cout << "Searching for Wally..." << endl << endl;
    
    //Main loop for running through the scene
    for(int rowCount = 0; rowCount < clutteredRows - wallyRows; rowCount = rowCount + thresholdRows){
        for(int colCount = 0; colCount < clutteredCols - wallyCols; colCount = colCount + thresholdCols){

                //Get 1D array of the scene area
                matrixAtArea = sceneImage->getMatrixArea(rowCount, colCount, wallyRows, wallyCols);
            
                //Run the search algorithm on the retrieved Matrix area
                if(algorithmChoice == 1){
                    //Workout SSD of the scene area
                    SSD = sceneImage->workoutSSD(wallyMatrixArea, matrixAtArea, wallyRows, wallyCols);
                    
                } else{
                    //Workout NC of the scene area
                    NC = sceneImage->workoutNC(wallyMatrixArea, matrixAtArea, wallyRows, wallyCols);
                }
            
            
                //Delete Matrix Area ready for next use
                delete [] matrixAtArea;
            
                //Set initial objects if the main loop is at the first row and col
                if(rowCount == 0 && colCount == 0){
                    if(algorithmChoice == 1){
                        tempMatrixObjectSSD->setStartingCol(colCount);
                        tempMatrixObjectSSD->setStartingRow(rowCount);
                        tempMatrixObjectSSD->setSSD(SSD);
                    } else{
                        tempMatrixObjectNC.setStartingCol(colCount);
                        tempMatrixObjectNC.setStartingRow(rowCount);
                        tempMatrixObjectNC.setNC(NC);
                    }
                }
            
            
                //Check if new SSD is smaller than what's stored in the current object, if it is then override the object, else the loop continues
                if(algorithmChoice == 1){
                    if(SSD < tempMatrixObjectSSD->getSSD()){
                        tempMatrixObjectSSD->setSSD(SSD);
                        tempMatrixObjectSSD->setStartingRow(rowCount);
                        tempMatrixObjectSSD->setStartingCol(colCount);
                    }
                } else{
                    if(NC > tempMatrixObjectNC.getNC()){
                        tempMatrixObjectNC.setNC(NC);
                        tempMatrixObjectNC.setStartingRow(rowCount);
                        tempMatrixObjectNC.setStartingCol(colCount);
                    }
                }
            
                //Update the number of comparisons
                comparisons++;
            }
    }
    
        //Delete the Wally Matrix area upon completion of the program
        delete [] wallyMatrixArea;

        //Edit the cluttered scene inputs to where the program thinks Wally is at
        if(algorithmChoice == 1){
            cluttered_scene_input_data_SSD = sceneImage->draw(tempMatrixObjectSSD->getStartingRow(), tempMatrixObjectSSD->getStartingCol(), cluttered_scene_input_data_SSD, wallyRows, wallyCols, clutteredCols);
        } else{
            cluttered_scene_input_data_NC = sceneImage->draw(tempMatrixObjectNC.getStartingRow(), tempMatrixObjectNC.getStartingCol(), cluttered_scene_input_data_NC, wallyRows, wallyCols, clutteredCols);
        }
    
        delete sceneImage;
        delete tempMatrixObjectSSD;
    
        //Write out the new scene showing where wally is. Q = 255 for greyscale images and 1 for binary images.
        const int Q = 255;
    
        //Output filenames
        const string outputFileName_SSD = "SSD_result.pgm", outputFileName_NC = "NC_result.pgm";
    
        //Actually write data to the files
        if(algorithmChoice == 1){
            WritePGM(outputFileName_SSD, cluttered_scene_input_data_SSD, clutteredRows, clutteredCols, Q);
        } else{
            WritePGM(outputFileName_NC, cluttered_scene_input_data_NC, clutteredRows, clutteredCols, Q);
        }
    
        //Delete objects
        delete [] cluttered_scene_input_data_SSD;
        delete [] cluttered_scene_input_data_NC;
    
        //Stringstream output
        ostringstream output;
        output << "Search completed." << endl << comparisons << " subimages have been compared." << " I've drawn a black box around where I think Wally is. Your result is stored in the image file";
    
        if(algorithmChoice == 1){
            output << " 'SSD_result.pgm'.";
        } else{
            output << " 'NC_result.pgm'.";
        }
    
        cout << output.str() << endl << endl;
    
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

int getAlgorithm(){
    
    int algorithmChoice;
  
    do{
        cout << endl << "-----------------------------------------------------------------------------" << endl;
        cout << "Please select which Search Algorithm to use" << endl;
        cout << "1 - Sum of Squared Differences" << endl;
        cout << "2 - Normalised Correlation" << endl;
        cout << "-----------------------------------------------------------------------------" << endl;
        
        cin >> algorithmChoice;
    
        if((algorithmChoice != 1) && (algorithmChoice != 2)){
            cout << "Invalid Input! Please try again." << endl;
        }
        
    } while((algorithmChoice != 1) && (algorithmChoice != 2));
    
    return algorithmChoice;
}

int getSearchArea(){
    int area;

    do{
        cout << endl << "-----------------------------------------------------------------------------" << endl;
        cout << "Please select which how to search the image" << endl;
        cout << "1 - Small Area (Sub-image rows / 2 & sub-image rows / 3) (fastest)" << endl;
        cout << "2 - Medium Area of Sub-image (Sub-image rows / 2 & sub-image rows / 2) (medium speed)" << endl;
        cout << "3 - Whole Area (Pixel by Pixel) (slow)" << endl;
        cout << "-----------------------------------------------------------------------------" << endl;
        
        cin >> area;
        
        if((area != 1) && (area != 2) && (area != 3)){
            cout << "Invalid Input! Please try again" << endl;
        }

        
    } while((area != 1) && (area != 2) && (area != 3));
    
    return area;
}

void convertPixels(int rows, int cols, double* data, MatchImage* image){
    int count = 0;
    //Sets the colour codes to the Matrix for the Wally matrix
    for(int rowcount = 0; rowcount < rows; rowcount++){
        for(int colcount = 0; colcount < cols; colcount++){
            image->setPixel(rowcount, colcount, data[count]);
            count++;
        }
    }
    
}

void convertPixels(int rows, int cols, double* data, LargeImage* image){
    int count = 0;
    //Sets the colour codes to the Matrix for the Wally matrix
    for(int rowcount = 0; rowcount < rows; rowcount++){
        for(int colcount = 0; colcount < cols; colcount++){
            image->setPixel(rowcount, colcount, data[count]);
            count++;
        }
    }
    
}



