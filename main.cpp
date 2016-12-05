//
//  main.cpp
//  tasks
//
//  Created by Andrew Smith on 06/11/2016.
//  Copyright © 2016 Andrew Smith. All rights reserved.
//

#include <iostream>
#include <sstream> // stringstream
#include <fstream> // ifstream
#include <istream>
#include <algorithm>
#include <string>

//Include header files 
#include "matrix.h"
#include "matchImage.h"
#include "largeImage.h"

//Prototypes

//Reads .txt file and converts it to a 1D array of doubles of size R*C
double* readTXT(std::string fileName, int sizeR, int sizeC);

//Converts 1D array of doubles to .pgm image. Use Q = 255 for greyscale images and Q=1 for binary images.
void WritePGM(std::string filename, double *data, int sizeR, int sizeC, int Q);

//Gets the search algorithm choice from the user
int getAlgorithm();

//Gets how the user wants to search the image
int getSearchArea();

//Function for converting the pixels from the file images into the objects
void convertPixels(int rows, int cols, double* data, MatchImage*& image);

//Overloaded function for the convertPixels function
void convertPixels(int rows, int cols, double* data, LargeImage*& image);

int main() {
    
    //M and N represent the number of rows and columns in the image
    const int clutteredRows = 768, clutteredCols = 1024, wallyRows = 49, wallyCols = 36;
    int comparisons = 0;
    double SSD, NC;
    
    //Initiate the Matrices from the classes
    MatchImage* wallyImage = new MatchImage();
    LargeImage* sceneImage = new LargeImage(clutteredRows, clutteredCols);
    
    //File names for reading in the images from the text files
    const std::string inputFileName = "Cluttered_scene.txt", wallyInputFileName = "Wally_Grey.txt";
    
    //Creates pointers to 1D arrays of doubles read in from the text files
    double* cluttered_scene_input_data_SSD = 0;
    double* cluttered_scene_input_data_NC = 0;
    double* wally_input_data = 0;
    double* matrixAtArea;
   
    //input_data arrays to hold the colour codes of the text files being imported
    cluttered_scene_input_data_SSD = readTXT(inputFileName, clutteredRows, clutteredCols);
    cluttered_scene_input_data_NC = readTXT(inputFileName, clutteredRows, clutteredCols);
    wally_input_data = readTXT(wallyInputFileName, wallyRows, wallyCols);
    
    //Sets the colour codes to the Matrix for the read in text files, now the class objects contain the colour codes in 2D array format
    convertPixels(wallyRows, wallyCols, wally_input_data, wallyImage);
    convertPixels(clutteredRows, clutteredCols, cluttered_scene_input_data_SSD, sceneImage);
    convertPixels(clutteredRows, clutteredCols, cluttered_scene_input_data_NC, sceneImage);
    
    //Delete the date
    delete [] wally_input_data;
    
    //Retrieve the Wally image values into a 1D Array
    double* wallyMatrixArea = wallyImage->getMatrixArea(0, 0, wallyRows, wallyCols);
    
    //Delete Wally object now the Wally data is in the array
    delete wallyImage;

    //Create the temporary objects for the comparisons, using a copy constructor
    MatchImage* tempMatrixObjectSSD = new MatchImage();
    MatchImage tempMatrixObjectNC = *tempMatrixObjectSSD;
    
    //*  *  *  *  *  * NOTE  *  *  *  *  *  *  *
    //^This process can also be done with this assignment operator overload by uncommenting the next line & changing the pointer functions from . to ->
    //MatchImage* tempMatrixObjectNC = tempMatrixObjectSSD;
    //*  *  *  *  *  * NOTE  *  *  *  *  *  *  *

    //Prepares the basic variables for the main loop
    int algorithmChoice = getAlgorithm(), area = getSearchArea(), thresholdCols, thresholdRows;
    
    //Assign the threshold variables for searching the image, based on the user's choice
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
            std::cout << "ERROR setting the threshold rows and columns - Please Restart the Program" << std::endl;
    }
    
    std::cout << "Searching for Wally..." << std::endl << std::endl;
    
    //Main loop for running through the scene and calculating the similarity values
    for(int rowCount = 0; rowCount < clutteredRows - wallyRows; rowCount = rowCount + thresholdRows){
        for(int colCount = 0; colCount < clutteredCols - wallyCols; colCount = colCount + thresholdCols){

                //Get 1D array of the scene area vcolour values
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
            
                //Set initial objects variables if the main loop is at the first row and col
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
            
                //Increase the number of comparisons
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
    
        //Delete the objects
        delete sceneImage;
        delete tempMatrixObjectSSD;
    
        //Write out the new scene showing where wally is. Q = 255 for greyscale images and 1 for binary images.
        const int Q = 255;
    
        //Declare output filenames
        const std::string outputFileName_SSD = "SSD_result.pgm", outputFileName_NC = "NC_result.pgm";
    
        //Actually write data to the files
        if(algorithmChoice == 1){
            WritePGM(outputFileName_SSD, cluttered_scene_input_data_SSD, clutteredRows, clutteredCols, Q);
        } else{
            WritePGM(outputFileName_NC, cluttered_scene_input_data_NC, clutteredRows, clutteredCols, Q);
        }
    
        //Delete objects
        delete [] cluttered_scene_input_data_SSD;
        delete [] cluttered_scene_input_data_NC;
    
        //Stringstream output to the user
        std::ostringstream output;
        output << "Search completed." << std::endl << comparisons << " subimages have been compared." << " I've drawn a black box around where I think Wally is. Your result is stored in the image file";
    
        //Append correct filename to the stringstream
        if(algorithmChoice == 1){
            output << " 'SSD_result.pgm'.";
        } else{
            output << " 'NC_result.pgm'.";
        }
    
        //Output stringstream message
        std::cout << output.str() << std::endl << std::endl;
    
        return 0;
}


// Read .txt file with image of size RxC, and convert to an array of doubles
double* readTXT(std::string fileName, int sizeR, int sizeC){
    
    //Data Array - 1D Array of doubles
    double* data = new double[sizeR*sizeC];
   
    int i = 0;
   
    std::ifstream myfile;
    myfile.open(fileName, std::ios::in);
   
    if (myfile.is_open()){
        
        while (myfile.good()){
            //If I reaches the end of the Matrix
            if (i>sizeR*sizeC-1) break;
            myfile >> *(data+i);
            //std::std::cout << *(data+i) << ' '; // This line display the converted data on the screen, you may comment it out.
            i++;
        }
        
        myfile.close();
    }
    
    else{
        std::cout << "Unable to open file" << std::endl;
    }
    
    return data;
}

// convert data from double to .pgm stored in filename
void WritePGM(std::string filename, double *data, int sizeR, int sizeC, int Q){
    
    int i;
    unsigned char *image;
    
    std::ofstream myfile;
    
    image = (unsigned char *) new unsigned char [sizeR*sizeC];
    
    // convert the integer values to unsigned char
    
    for(i=0; i<sizeR*sizeC; i++)
        image[i]=(unsigned char)data[i];
    
    myfile.open(filename, std::ios::out|std::ios::binary|std::ios::trunc);
    
    if (!myfile) {
        std::cout << "Can't open file: " << filename << std::endl;
        exit(1);
    }
    
    myfile << "P5" << std::endl;
    myfile << sizeC << " " << sizeR << std::endl;
    myfile << Q << std::endl;
    
    myfile.write( reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));
    
    if (myfile.fail()) {
        std::cout << "Can't write image " << filename << std::endl;
        exit(0);
    }
    
    myfile.close();
    
    delete [] image;
    
}

int getAlgorithm(){
    int algorithmChoice;
    bool valid = false;
    
    //Error Handling loop to validate input
    while(valid != true){
        try{
                std::cout << "-----------------------------------------------------------------------------" << std::endl;
                std::cout << "Please select which Image Search Algorithm to use" << std::endl;
                std::cout << "1 - Sum of Squared Differences" << std::endl;
                std::cout << "2 - Normalised Correlation" << std::endl;
                std::cout << "-----------------------------------------------------------------------------" << std::endl;
            
                //Take user input
                std::cin >> algorithmChoice;
            
                //Condition to test whether the input is out of bounds
                if((algorithmChoice != 1) && (algorithmChoice != 2)){
                    throw 1;
                } else{
                    throw 2;
                }
            
        //Catch error message and process it, break the loop if it's valid
        } catch(int errorNumber){
            if(errorNumber == 1){
                std::cout << "Invalid Input! Please try again" << std::endl;
            } else{
                valid = true;
            }
            
        }
        
    }

    
    return algorithmChoice;
}

int getSearchArea(){
    int area;
    
    bool valid = false;
    
    //Loop to validate user input
    while(valid != true){
        try{
            std::cout << std::endl << "-----------------------------------------------------------------------------" << std::endl;
            std::cout << "Please select which how to search the image" << std::endl;
            std::cout << "1 - Small Sub-Image - Rows / 3 & Cols / 2) (max 2 second)" << std::endl;
            std::cout << "2 - Medium Sub-Image - Rows / 2 & Cols / 2) (max 3 seconds)" << std::endl;
            std::cout << "3 - Whole Image - Pixel by Pixel (max 1 minute)" << std::endl;
            std::cout << "-----------------------------------------------------------------------------" << std::endl;
            
            //Take user input
            std::cin >> area;
            
            //Test condition and throw error message based upon this
            if((area != 1) && (area != 2) && (area != 3)){
                throw 1;
            } else{
                throw 2;
            }
        } catch(int errorNumber){
            if(errorNumber == 1){
                std::cout << "Invalid Input! Please try again" << std::endl;
            } else{
                valid = true;
            }
            
        }

    }
    
    return area;
}

void convertPixels(int rows, int cols, double* data, MatchImage*& image){
    int count = 0;
    
    //Sets the colour codes to the Matrix for the cluttered scene matrix by using a member function of the Matrix class
    for(int rowcount = 0; rowcount < rows; rowcount++){
        for(int colcount = 0; colcount < cols; colcount++){
            image->setPixel(rowcount, colcount, data[count]);
            count++;
        }
    }
    
}

void convertPixels(int rows, int cols, double* data, LargeImage*& image){
    int count = 0;
    
    //Sets the colour codes to the Matrix for the Wally matrix by using a member function of the Matrix class
    for(int rowcount = 0; rowcount < rows; rowcount++){
        for(int colcount = 0; colcount < cols; colcount++){
            image->setPixel(rowcount, colcount, data[count]);
            count++;
        }
    }
    
}



