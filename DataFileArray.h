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

class DataFileArray {
public:
    DataFileArray(const char* filename): filename{filename}{};
    void write(double data);
    double read(unsigned int line);
    virtual ~DataFileArray();
private:
    std::fstream file;
    std::size_t size;
    std::string filename;
};

#endif /* DATAFILEARRAY_H */

