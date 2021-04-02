/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataFileArray.h
 * Author: lrdass
 *
 * Created on 30 de Março de 2021, 16:23
 */

#ifndef DATAFILEARRAY_H
#define DATAFILEARRAY_H

#include <fstream>
#include <algorithm>
#include <queue>
#include <fstream>
#include<sstream>
#include <queue>
#include<algorithm>

class DataFileArray {
public:
    DataFileArray(const char* filename): filename{filename}, size{0} {};
    void write(double data);
    void clear();
    double read(unsigned int line);
    ~DataFileArray();
    double read();
    void sortFile();
private:
    void _mergeTempFiles(int totalChunks);
    
private:
    std::fstream file;
    std::size_t size;
    std::string filename;
    
};

#endif /* DATAFILEARRAY_H */

