/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StatisticsDataFileDummyImpl.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 22 de Novembro de 2018, 01:24
 */

#include "StatisticsDataFileDefaultImpl.h"
#include "Traits.h"

StatisticsDataFileDummyImpl::StatisticsDataFileDummyImpl() {
    _collector = new Traits<Statistics_if>::CollectorDatafileImplementation();
    // avisar statistics datafile
    _collector->setAddValueHandler(setCollectorAddValueHandler(&StatisticsDataFileDummyImpl::collectorAddHandler, this));
    _collector->setClearHandler(setCollectorClearHandler(&StatisticsDataFileDummyImpl::collectorClearHandler, this));
    
    
}

unsigned int StatisticsDataFileDummyImpl::numElements() {
	return this->getCollector()->numElements();
        
}

double StatisticsDataFileDummyImpl::min() {
    if (_elems > 0)
            return _min;
    else
            return 0.0;
}

double StatisticsDataFileDummyImpl::max() {
    if (_elems > 0)
            return _max;
    else
            return 0.0;
}

double StatisticsDataFileDummyImpl::average() {
    return this->_average;
}

double StatisticsDataFileDummyImpl::mode() {
	return _variance; // dummy
}

double StatisticsDataFileDummyImpl::mediane() {
	return 0.0; // dummy
}

double StatisticsDataFileDummyImpl::variance() {
	return _variance; // dummy
}

double StatisticsDataFileDummyImpl::stddeviation() {
	return _stddeviation; // dummy
}

double StatisticsDataFileDummyImpl::variationCoef() {
	return _variationCoef; // dummy
}

double StatisticsDataFileDummyImpl::halfWidthConfidenceInterval() {
	return _halfWidth; // dummy
}

unsigned int StatisticsDataFileDummyImpl::newSampleSize(double halfWidth) {
	return 0; // dummy
}

double StatisticsDataFileDummyImpl::quartil(unsigned short num) {
	return 0.0; // dummy
}

double StatisticsDataFileDummyImpl::decil(unsigned short num) {
	return 0.0; // dummy
}

double StatisticsDataFileDummyImpl::centil(unsigned short num) {
	return 0.0; // dummy
}

void StatisticsDataFileDummyImpl::setHistogramNumClasses(unsigned short num) {
    std::cout << "teste" << std::endl;
}

unsigned short StatisticsDataFileDummyImpl::histogramNumClasses() {
	return 0; // dummy
}

double StatisticsDataFileDummyImpl::histogramClassLowerLimit(unsigned short classNum) {
	return 0.0; // dummy
}

unsigned int StatisticsDataFileDummyImpl::histogramClassFrequency(unsigned short classNum) {
	return 0; // dummy
}

Collector_if* StatisticsDataFileDummyImpl::getCollector() {
	return this->_collector;
}

void StatisticsDataFileDummyImpl::setCollector(Collector_if* collector) {
    _collector = collector;
}

double StatisticsDataFileDummyImpl::getConfidenceLevel(){
    return 0.0;
};
void StatisticsDataFileDummyImpl::setConfidenceLevel(double confidencelevel){
    
}

void StatisticsDataFileDummyImpl::collectorAddHandler(double newValue){
    // do stuff with new value
    _elems = _collector->numElements();
    if(newValue < _min){
        _min = newValue;
    }
    if(newValue> _max){
        _max = newValue;
    }
    
    _sum += newValue;
    _average = _sum / _elems;
    _stddeviation = std::sqrt(_variance);
    _variance = _sumSquare / _elems - _average*_average;
    _variationCoef = (_average != 0 ? _stddeviation / _average : 0.0);
    _halfWidth = _criticalTn_1 * (_stddeviation / std::sqrt(_elems));
}

void StatisticsDataFileDummyImpl::collectorClearHandler(){
    // do stuff to reset 
    _elems = 0;
    _min = +1e+99;
    _max = -1e+99;
    _sum = 0.0;
    _sumSquare = 0.0;
    _average = 0.0;
    _variance = 0.0;
    _stddeviation = 0.0;
    _variationCoef = 0.0;
    _halfWidth = 0.0;
}
