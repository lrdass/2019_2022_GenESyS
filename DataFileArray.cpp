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

#include "DataFileArray.h"
//
//DataFileArray::DataFileArray(const char* filename):filename{filename}
//{
//   
//}

double DataFileArray::read(unsigned int line){
    std::ifstream file;
    file.open(filename,  std::ios::in | std::ios::binary);
    
    auto size = sizeof(double);
    auto pos = sizeof(double) * line;
    
    file.seekg(pos, std::ios::beg);
    char buffer[size];
    
    file.read(buffer, size);
    
    return *((double*) buffer);
    
}

void DataFileArray::write(double data){
    std::ofstream file(filename,  std::ios::binary | std::ios::in | std::ios::out | std::ios::ate); 
    file.write((const char*) &data, sizeof(data));
    file.close();
}

DataFileArray::~DataFileArray() {
    
}

