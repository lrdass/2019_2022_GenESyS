/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StatisticsDefaultImpl1.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 1 de Agosto de 2018, 21:03
 */

#include <complex>

#include "StatisticsDefaultImpl1.h"
#include "Traits.h"
#include <gmp.h>
//#include "Integrator_if.h"
//#include "ProbDistribDefaultImpl1.h"

//using namespace GenesysKernel;

StatisticsDefaultImpl1::StatisticsDefaultImpl1() {
	//_collector = new Traits<Statistics_if>::CollectorImplementation();
 	_collector = new Traits<ModelComponent>::StatisticsCollector_CollectorImplementation();
	_collector->setAddValueHandler(setCollectorAddValueHandler(&StatisticsDefaultImpl1::collectorAddHandler, this));
	_collector->setClearHandler(setCollectorClearHandler(&StatisticsDefaultImpl1::collectorClearHandler, this));
	// nosso collector DataFile "escutar" este collector
        
        //_collector->setAddValueHandler(std::bind(&StatisticsDefaultImpl1::collectorAddHandler, this, std::placeholders::_1));
	this->initStatistics();
}

StatisticsDefaultImpl1::StatisticsDefaultImpl1(Collector_if* collector) {
	_collector = collector;
	_collector->setAddValueHandler(setCollectorAddValueHandler(&StatisticsDefaultImpl1::collectorAddHandler, this));
	_collector->setClearHandler(setCollectorClearHandler(&StatisticsDefaultImpl1::collectorClearHandler, this));
	//_collector->setAddValueHandler(std::bind(&StatisticsDefaultImpl1::collectorAddHandler, this, std::placeholders::_1));
	this->initStatistics();
}

void StatisticsDefaultImpl1::collectorAddHandler(double newValue) {
	_elems = _collector->numElements();
	if (newValue < _min) {
		_min = newValue;
	}
	if (newValue > _max) {
		_max = newValue;
	}
	// alternative 1
	//_sum += newValue;
	//_sumSquare += newValue*newValue;
	//_average = _sum / _elems;
	//_variance = _sumSquare / _elems - _average*_average;
	// alternative 2
	mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
        mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia

        mpf_t f_average, f_variance, f_newValue, f_elems, f_aux, f_auxTwo;
        mpf_inits(f_average, f_variance, f_newValue, f_elems, f_aux, f_auxTwo, NULL);
        
        mpf_set_ui(f_elems, _elems);
        mpf_set_d(f_newValue, newValue);
        mpf_set_d(f_average, _average);
        
        mpf_sub_ui(f_aux, f_elems, 1);     // aux <-              _elems - 1
        mpf_mul(f_aux, f_average, f_aux);  // aux <-  _average * (_elems - 1)
        mpf_add(f_aux, f_aux, f_newValue); // aux <-  _average * (_elems - 1) + newValue
        mpf_div(f_aux, f_aux, f_elems);    // aux <- (_average * (_elems - 1) + newValue) / _elems
        
        _average = mpf_get_d(f_aux);       // _average <- float _average (possivel truncamento)
        
        //_average = (_average * (_elems - 1) + newValue) / _elems;  // this approach propagates the numeric error
        mpf_set(f_average, f_aux); // Update f_average with new average
	
        mpf_sub_ui(f_aux, f_elems, 1);            // aux <-                  _elems - 1
        mpf_mul(f_aux, f_variance, f_aux);        // aux <-     _variance * (_elems - 1)
        mpf_sub(f_auxTwo, f_newValue, f_average); // auxTwo <-                                 newValue - _average
        mpf_pow_ui(f_auxTwo, f_auxTwo, 2);        // auxTwo <-                             pow(newValue - _average, 2)
        mpf_add(f_aux, f_aux, f_auxTwo);          // aux <-     _variance * (_elems - 1) + pow(newValue - _average, 2)
        mpf_div(f_aux, f_aux, f_elems);           // aux <-    (_variance * (_elems - 1) + pow(newValue - _average, 2)) / _elems
        
        _variance = mpf_get_d(f_aux);             // _variance <- float _variance (possivel truncamento)
        
        //_variance = (_variance * (_elems - 1) + pow(newValue - _average, 2)) / _elems;  // this approach propagates the numeric error
        
        mpf_sqrt(f_aux, f_aux);
        _stddeviation = mpf_get_d(f_aux); //_stddeviation = std::sqrt(_variance)
	//_stddeviation = std::sqrt(_variance);
        
        
        if (_average != 0) {
             mpf_div(f_aux, f_aux, f_average); //_stddeviation / _average
            _variationCoef = mpf_get_d(f_aux);
        } else {
            _variationCoef = 0.0;
        }
        //_variationCoef = (_average != 0 ? _stddeviation / _average : 0.0);
	
        mpf_set_d(f_aux, _stddeviation); // Evita criar mais um float
        mpf_sqrt(f_auxTwo, f_elems);     //                                 std::sqrt(_elems)
        mpf_div(f_aux, f_aux, f_auxTwo); //                (_stddeviation / std::sqrt(_elems))
        mpf_set_d(f_auxTwo, _criticalTn_1); // Necessario pois nao existe mpf_mul_d()
        mpf_mul(f_aux, f_auxTwo, f_aux); //_criticalTn_1 * (_stddeviation / std::sqrt(_elems))
        
        _halfWidth = mpf_get_d(f_aux);
        //_halfWidth = _criticalTn_1 * (_stddeviation / std::sqrt(_elems));
}

void StatisticsDefaultImpl1::collectorClearHandler() {
	this->initStatistics();
}

void StatisticsDefaultImpl1::initStatistics() {
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

unsigned int StatisticsDefaultImpl1::numElements() {
	return this->getCollector()->numElements();
}

double StatisticsDefaultImpl1::min() {
	if (_elems > 0)
		return _min;
	else
		return 0.0;
}

double StatisticsDefaultImpl1::max() {
	if (_elems > 0)
		return _max;
	else
		return 0.0;
}

double StatisticsDefaultImpl1::average() {
	return _average;
}

double StatisticsDefaultImpl1::variance() {
	return _variance;
}

double StatisticsDefaultImpl1::stddeviation() {
	return _stddeviation;
}

double StatisticsDefaultImpl1::variationCoef() {
	return _variationCoef;
}

double StatisticsDefaultImpl1::halfWidthConfidenceInterval() {
	return _halfWidth;
}

void StatisticsDefaultImpl1::setConfidenceLevel(double confidencelevel) {
	_confidenceLevel = confidencelevel;
	//Integrator_if* integrator = new Traits<Integrator_if>::Implementation();
	_criticalTn_1 = 1.96; //integrator->integrate()

}

double StatisticsDefaultImpl1::getConfidenceLevel() {
	return _confidenceLevel;
}

unsigned int StatisticsDefaultImpl1::newSampleSize(double halfWidth) {
	return 0;
}

Collector_if* StatisticsDefaultImpl1::getCollector() {
	return this->_collector;
}

void StatisticsDefaultImpl1::setCollector(Collector_if* collector) {
	this->_collector = collector;
}