/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StatisticsDataFileDefaultImpl.h
 * Author: rafael.luiz.cancian
 *
 * Created on 22 de Novembro de 2018, 01:24
 */

#ifndef STATISTICSDATAFILEDEFAULTIMPL_H
#define STATISTICSDATAFILEDEFAULTIMPL_H

#include "StatisticsDataFile_if.h"
#include "CollectorDatafile_if.h"
#include "CollectorDatafileDefaultImpl1.h" // necessario para declarar _collector_test

class StatisticsDataFileDummyImpl : public StatisticsDatafile_if {
public:
	StatisticsDataFileDummyImpl();
	~StatisticsDataFileDummyImpl() = default;
public:
	Collector_if* getCollector();
	void setCollector(Collector_if* collector);
public:
	unsigned int numElements();
	double min();
	double max();
	double average();
	double variance();
	double stddeviation();
	double variationCoef();
	double halfWidthConfidenceInterval();

	double mode();
	double mediane();
	double quartil(unsigned short num);
	double decil(unsigned short num);
	double centil(unsigned short num);
	void setHistogramNumClasses(unsigned short num);
	unsigned short histogramNumClasses();
	double histogramClassLowerLimit(unsigned short classNum);
	unsigned int histogramClassFrequency(unsigned short classNum);
        
	unsigned int newSampleSize(double halfWidth);
        
	double getConfidenceLevel();
	void setConfidenceLevel(double confidencelevel);
        
private:
        void collectorAddHandler(double newValue);
        void collectorClearHandler();
private:
	Collector_if* _collector;
        CollectorDatafileDefaultImpl1* _collectorDatafile;
        		
        unsigned long _elems = 0;
        double _sum = 0.0;
        double _sumSquare = 0.0;
        double _min = +1e+99;
        double _max = -1e+99;
        double _average = 0.0;
        double _variance = 0.0;
        double _stddeviation = 0.0;
        double _variationCoef = 0.0;
        double _confidenceLevel = 0.95;
        double _criticalTn_1 = 1.96;
        double _halfWidth = 0.0;
        
        unsigned short _histogramclassNum = 0;
        
        bool _wasAltered_stddeviation = false;
        bool _wasAltered_variationCoef = false;
        bool _wasAltered_halfWidth = false;
};

#endif /* STATISTICSDATAFILEDEFAULTIMPL_H */

