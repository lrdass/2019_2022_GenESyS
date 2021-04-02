/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataFileArray.cpp
 * Author: lrdass
 * 
 * Created on 30 de Março de 2021, 16:23
 */

#include <iomanip>
#include <istream>
#include <iostream>

#include "DataFileArray.h"
//
//DataFileArray::DataFileArray(const char* filename):filename{filename}
//{
//   
//}

double DataFileArray::read(unsigned int line){
    std::fstream file;
    file.open(filename,  std::ios::in | std::ios::out | std::ios::binary);
    
    auto size = sizeof(double);
    auto pos = sizeof(double) * line;
    
    file.seekg(pos, std::ios::beg);
    char buffer[size];
    
    file.read(buffer, size);
    
    file.seekg(0, std::ios::end);
    this->size = file.tellg() / sizeof(double);
    
    return *((double*) buffer);
    
}

void DataFileArray::write(double data){
    std::fstream file;
    
    file.open(filename,  std::ios::out | std::ios::app | std::ios::binary);
    // dizer que o arquivo é de registros de doubles
    // arquivo binario <double>
    file.write((const char*) &data, sizeof(data));
    
    size +=1;
    file.close();
}

DataFileArray::~DataFileArray() {
    
}

void DataFileArray::sortFile() {
    int currentChunk = 1;
    int totalMemoryBuffer = 10 * sizeof(double);
    int numberOfChunks = totalMemoryBuffer / sizeof(double);
    
    double chunkBuffer[numberOfChunks];
    double currentValue = 0;
    int currentCount = 0;
    bool unprocessedData = true;
    

    std::fstream file(filename, std::ios::binary| std::ios::in);
    while(file.read((char*)&currentValue, sizeof(currentValue))){

        chunkBuffer[currentCount] = currentValue;
        currentCount+=1;
        
        if(currentCount == numberOfChunks){
            unprocessedData = true;
            
            std::sort(chunkBuffer, chunkBuffer + currentCount);
            std::ofstream tempFile(
                filename+"_temp"+ std::to_string(currentChunk),
                std::ios::binary | std::ios::in | std::ios::out | std::ios::app
            );
            
            for (auto value: chunkBuffer){
                tempFile.write( (const char *) &value, sizeof(double));
            }
            tempFile.close();
            
            currentCount = 0;
            currentChunk +=1;
            unprocessedData = false;
        }
    }
    
    if(unprocessedData){
        std::sort(chunkBuffer, chunkBuffer + currentCount);
        std::ofstream tempFile(
            filename+"_temp"+ std::to_string(currentChunk),
            std::ios::binary | std::ios::in | std::ios::out | std::ios::app
        );

        for (auto value: chunkBuffer){
            tempFile.write( (const char *) &value, sizeof(value));
        }
        tempFile.close();
    } else{
        // it could be just one file
        numberOfChunks -=1;
    }

    file.close();
        
    if(currentChunk != 0){
        this->_mergeTempFiles(currentChunk);
    }
        
}

void DataFileArray::clear(){
    std::ofstream ofs;
    ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

struct compare
{
    //Ascending order sort
    bool operator() (std::pair<double, int> pair1, std::pair<double, int> pair2)
    {
        return pair1.first > pair2.first;
    }
};

void writeOnFile(std::string filename, double value){
    std::ofstream tempFile(
            filename+"_sorted",
            std::ios::binary | std::ios::in | std::ios::out | std::ios::app
    );
    tempFile.write( (const char *) &value, sizeof(double));
    tempFile.close();
}

void DataFileArray::_mergeTempFiles(int totalChunks){
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, compare> minHeap;
    std::fstream* chunkFiles = new std::fstream[totalChunks];
  
    
    
    for (int i = 1; i <= totalChunks; i++) {
        std::string sortedChunkFile = filename+"_temp"+ std::to_string(i);
        chunkFiles[i - 1].open(sortedChunkFile, std::ios::binary| std::ios::in);

        double valueReaded;
        if(chunkFiles[i- 1].read((const char*)&valueReaded, sizeof(valueReaded))){
            minHeap.push(std::pair<double, int>(valueReaded, i-1));
        } else {
            continue;
        } //first value in the file (minimum in the file)
        //second value in pair keeps track of the file from which the number was drawn

    }
        
    while(!minHeap.empty()){
        std::pair<double, int> value = minHeap.top();
        std::cout << std::to_string(value.first)<< std::endl;
        minHeap.pop();

        // write on final sorted file
        // read one more line for each file
        writeOnFile(this->filename, value.first);
        
        double nextValue = 0;
        if (chunkFiles[value.second].read((char*)&nextValue, sizeof(nextValue)))
            minHeap.push(std::pair<double, int>(nextValue, value.second));

    }
        
    for (int i = 1; i <= totalChunks; i++) {
        chunkFiles[i - 1].close();
        std::string sortedChunkFile = filename+"_temp"+ std::to_string(i);
        std::remove(sortedChunkFile);
        
    }
     delete[] chunkFiles;
}



