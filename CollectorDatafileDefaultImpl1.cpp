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
    inputFile.open(_filename, std::ios::app);
    inputFile << value << std::endl;
    inputFile.close();
}

double CollectorDatafileDefaultImpl1::getLastValue() {
    if(!wasFileSorted){
        sortFileInplace();
    } else{
        return 0.0; // \todo:
    }
}

unsigned long CollectorDatafileDefaultImpl1::numElements() {
	return 0.0; // \todo:
}

double CollectorDatafileDefaultImpl1::getValue(unsigned int num) {
    if(!wasFileSorted){
        sortFileInplace();
    } else{
        return 0.0; // \todo:
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
    std::string command = "sort "+_filename + " -o " + _filename;

    if(system(command.c_str()) == 0){
        wasFileSorted = true;
        return 1;
    } 
    return -1;
}

