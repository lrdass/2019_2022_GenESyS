/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CollectorDatafileDefaultImpl11.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 1 de Agosto de 2018, 20:58
 */

#include "CollectorDatafileDefaultImpl1.h"

CollectorDatafileDefaultImpl1::CollectorDatafileDefaultImpl1() {
    wasFileSorted = false;
    auto timestamp = std::time(0);
    _filename = "default_" + std::to_string(timestamp) + ".dat";
}

void CollectorDatafileDefaultImpl1::clear() {
}

void CollectorDatafileDefaultImpl1::addValue(double value) { 
    std::ofstream inputFile;
    
    inputFile.open(_filename, std::ios::binary);
    // dizer que o arquivo é de registros de doubles
    // arquivo binario <double>
    inputFile.write(reinterpret_cast<char *>(&value), sizeof(value));
    inputFile.close();
    
    this->_lastValue = value;
    this->_numElements +=1;
    
    if(_addValueHandler != nullptr){
        _addValueHandler(value);
    }
}

// ultimo valor inserido
double CollectorDatafileDefaultImpl1::getLastValue() {
    return this->_lastValue;
}

unsigned long CollectorDatafileDefaultImpl1::numElements() {
	return this->_numElements;
}

double CollectorDatafileDefaultImpl1::getValue(unsigned int num) {
    if(!wasFileSorted){
        sortFileInplace();
        
        std::ifstream file(_filename);
        std::string line;
        
        if(file.is_open()){
            file.seekg(-1, std::ios_base::end);
            std::getline(file, line);
            return std::stod(line);
        }
        return 0;
        
        
        
    } else{
        
        return 0.0;
    }
	
}

double CollectorDatafileDefaultImpl1::getNextValue() {
	return 0.0; // \todo:
}

void CollectorDatafileDefaultImpl1::seekFirstValue() {
}

std::string CollectorDatafileDefaultImpl1::getDataFilename() {
	return _filename;
}

void CollectorDatafileDefaultImpl1::setDataFilename(std::string filename) {
	_filename = filename;
}

void CollectorDatafileDefaultImpl1::setAddValueHandler(CollectorAddValueHandler addValueHandler) {
    
}

void CollectorDatafileDefaultImpl1::setClearHandler(CollectorClearHandler clearHandler) {

}

int CollectorDatafileDefaultImpl1::sortFileInplace(){
    // https://codereview.stackexchange.com/questions/135241/sort-a-binary-file-without-loading-it-into-memory-or-using-a-temporary-file
    std::string command = "sort "+_filename + " -o " + _filename;
    
    if(system(command.c_str()) == 0){
        wasFileSorted = true;
        return 0;
    }
    return -1;
}

