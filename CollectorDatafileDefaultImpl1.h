/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CollectorDatafileDefaultImpl1.h
 * Author: rafael.luiz.cancian
 *
 * Created on 1 de Agosto de 2018, 20:58
 */

#ifndef COLLECTORDATAFILEDEFAULTIMPL1_H
#define COLLECTORDATAFILEDEFAULTIMPL1_H

#include <string>
#include <fstream>
#include <ctime>
#include "CollectorDatafile_if.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <limits>
#include "DataFileArray.h"

class CollectorDatafileDefaultImpl1 : public CollectorDatafile_if {
public:
	CollectorDatafileDefaultImpl1();
	virtual ~CollectorDatafileDefaultImpl1() = default;
public: // inherited from Collector_if
	void clear();
	void addValue(double value);
	double getLastValue();
	unsigned long numElements();
public:
	double getValue(unsigned int num);
	double getNextValue();
	void seekFirstValue();
	std::string getDataFilename();
	void setDataFilename(std::string filename);
        void sortFile();
        void getValueOrdered(unsigned int num);
public:
	void setAddValueHandler(CollectorAddValueHandler addValueHandler);
	void setClearHandler(CollectorClearHandler clearHandler);
private:
        bool wasFileSorted = false;
	std::string _filename;
        double _lastValue;
        int _numElements = 0;
        DataFileArray* _datafile;
        DataFileArray* _sortedFile;
private:
    
        CollectorAddValueHandler _addValueHandler = nullptr;
        CollectorClearHandler _clearHandler = nullptr;
};

#endif /* COLLECTORDATAFILEDEFAULTIMPL1_H */

