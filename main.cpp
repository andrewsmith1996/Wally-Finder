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
double* readTXT(char *fileName, int sizeR, int sizeC);

//Converts 1D array of doubles to .pgm image. Use Q = 255 for greyscale images and Q=1 for binary images.
void WritePGM(string filename, double *data, int sizeR, int sizeC, int Q);

//Prints the Progress of image
void printProgress(int rowCount, int colCount, int clutteredCols, int clutteredRows);



void quickSort(vector<MatchImage*>&,int,int);

int partition(vector<MatchImage*>&, int,int);







int main() {
    
    //M and N represent the number of rows and columns in the image. Cluttered_scene, M = 768, N = 1024, Wally_grey, M = 49, N =  36
    int clutteredRows = 768, clutteredCols = 1024, wallyRows = 49, wallyCols = 36;
    
    //Initiate the Matrixes
    MatchImage* wallyImage = new MatchImage(wallyRows, wallyCols);
    LargeImage* sceneImage = new LargeImage(clutteredRows, clutteredCols);
    
    //File names for reading in the images from the text files
    char* inputFileName = "Cluttered_scene.txt";
    char* wallyInputFileName = "Wally_Grey.txt";
    
    //Creates pointers to 1D arrays of doubles read in from the text files
    double* cluttered_scene_input_data = 0;
    double* wally_input_data = 0;
   
    //input_data to hold the colour codes of the text files
    cluttered_scene_input_data = readTXT(inputFileName, clutteredRows, clutteredCols);
    wally_input_data = readTXT(wallyInputFileName, wallyRows, wallyCols);

    //Converts wally_input_datas into the a wally matrix
    int count = 0;
    for(int rowcount = 0; rowcount < wallyRows; rowcount++){
        for(int colcount = 0; colcount < wallyCols; colcount++){
            wallyImage->setPixel(rowcount, colcount, wally_input_data[count]);
            count++;
        }
    }
    
    
    //Converts cluttered scene data into the cluttered scene matrix
    count = 0;
    for(int rowcount = 0; rowcount < clutteredRows; rowcount++){
        for(int colcount = 0; colcount < clutteredCols; colcount++){
            sceneImage->setPixel(rowcount, colcount, cluttered_scene_input_data[count]);
            count++;
        }
    }
    
    //Processing starts here
    cout << "Data from text file\n----------------------------" << endl;
   
    int finalRow, finalCol, comparisons = 0;
    
    cout << "Searching for Wally..." << endl;
   
    //Arrays that now contain the function section matrices
    double* wallyMatrixArea = wallyImage->getMatrixArea(0, 0, wallyRows, wallyCols);
   
    MatchImage* tempMatrixObject = new MatchImage(wallyRows, wallyCols);
    
   
    
    double* firstArea = sceneImage->getMatrixArea(0, 0, wallyRows, wallyCols);
    
    double firstSSD = sceneImage->workoutSSD(wallyMatrixArea, firstArea, wallyRows, wallyCols);

    delete[] firstArea;
    
    vector <MatchImage*> objects;
   
    
    tempMatrixObject->setSSD(firstSSD);
    
    for(int rowCount = 0; rowCount < clutteredRows - wallyRows; rowCount++){
        for(int colCount = 0; colCount < clutteredCols - wallyCols; colCount++){
            
         
            printProgress(rowCount, colCount, clutteredCols, clutteredRows);
        
            //Create a new Matrix the size of the searched section
            MatchImage* compareMatrix = new MatchImage(wallyRows, wallyCols);
          
            double* matrixAtArea = sceneImage->getMatrixArea(rowCount, colCount, wallyRows, wallyCols);
           
            double SSD = sceneImage->workoutSSD(wallyMatrixArea, matrixAtArea, wallyRows, wallyCols);
            
            delete[] matrixAtArea;
            
            compareMatrix->setStartingRow(rowCount);
            compareMatrix->setStartingCol(colCount);
            compareMatrix->setSSD(SSD);
            
            objects.push_back(compareMatrix);
            
            delete compareMatrix;
          
           /* if(SSD < tempMatrixObject->getSSD()){
                tempMatrixObject->setSSD(SSD);
                tempMatrixObject->setStartingRow(rowCount);
            }*/
        
            
            comparisons++;
            
        }
        
    }
    
    

    int size = objects.size() - 1;
    
    quickSort(objects, 0, size);

   

    cout << "Comparisons: " << comparisons << endl;

    for(int x = 0; x < 10; x++){
        cluttered_scene_input_data = sceneImage->draw(objects[x]->getStartingRow(), objects[x]->getStartingCol(), cluttered_scene_input_data, wallyRows, wallyCols, clutteredCols);
        
        //delete tempMatrixObject;
        
        //Write out the new scene showing where wally is. Q = 255 for greyscale images and 1 for binary images.
        int Q = 255;
        
        
        string outputFileName = "NNS_result_";
        
        ostringstream oss;
        oss << outputFileName << x << ".pgm";
        outputFileName = oss.str();
        
        WritePGM(outputFileName, cluttered_scene_input_data, clutteredRows, clutteredCols, Q);
    }
        
    
    

   
    
//    sceneImage->draw(tempMatrixObject->getStartingRow(), tempMatrixObject->getStartingCol(), cluttered_scene_input_data, wallyRows, wallyCols, clutteredCols);
    
    //Write out the new scene showing where wally is. Q = 255 for greyscale images and 1 for binary images.
   // int Q = 255;
    

    //char* outputFileName = "new_cluttered_scene.pgm";
    
    //WritePGM(outputFileName, cluttered_scene_input_data, clutteredRows, clutteredCols, Q);
    
    //cout << "Wally has been located!" << endl;
    
    //Delete objects
    delete[] cluttered_scene_input_data;
    delete wallyImage;
    delete sceneImage;
    
    
    return 0;
}


// Read .txt file with image of size RxC, and convert to an array of doubles
double* readTXT(char* fileName, int sizeR, int sizeC){
    
    //Data Array - 1D Array of Doubles
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



void quickSort(vector<MatchImage*>& A, int p,int q)
{
    int r;
    if(p<q)
    {
        r=partition(A, p,q);
        quickSort(A,p,r);
        quickSort(A,r+1,q);
    }
}


int partition(vector<MatchImage*>& A, int p,int q)
{
    int x= A[p]->getSSD();
    int i=p;
    int j;
    
    for(j=p+1; j<q; j++)
    {
        if(A[j]->getSSD()<=x)
        {
            i=i+1;
            swap(A[i],A[j]);
        }
        
    }
    
    swap(A[i],A[p]);
    return i;
}


