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
#include <gmp.h>
#include <math.h>

StatisticsDataFileDummyImpl::StatisticsDataFileDummyImpl() {
    _collector = new Traits<Statistics_if>::CollectorDatafileImplementation();
    // avisar statistics datafile
    _collector->setAddValueHandler(setCollectorAddValueHandler(&StatisticsDataFileDummyImpl::collectorAddHandler, this));
    _collector->setClearHandler(setCollectorClearHandler(&StatisticsDataFileDummyImpl::collectorClearHandler, this));
    
    
}

unsigned int StatisticsDataFileDummyImpl::numElements() {
    return _collector->numElements(); // dummy
}

double StatisticsDataFileDummyImpl::min() {
    return _min; // dummy
}

double StatisticsDataFileDummyImpl::max() {
    return _max; // dummy
}

double StatisticsDataFileDummyImpl::average() {
    return this->_average;
}

double StatisticsDataFileDummyImpl::mode() {
    // TODO no momento, nao eh possivel acessar o get value, parece que o _collector nao eh a versao certa
    // getValue() ja verifica o sort do arquivo
    
    long n_mode = 1; // qtd de repeticoes da moda
    double elem_mode = _collector->getValue(0); // o elemento que eh a moda
    
    long n = 1; // qtd de repeticoes do valor sendo analisado
    double elem = elem_mode; // o elemento sendo analisado, comeca sendo o mode
    
    double elem_aux;
    
    // for comeca em 1 pois o primeiro item ja foi "lido"
    for(int i = 1; i < _elems; i++) {
        elem_aux = _collector->getValue(i);
    
        if(elem == elem_aux) {
            n += 1;
        } else {
            if (n > n_mode) {
                n_mode = n;
                elem_mode = elem;
            }
    
            elem = elem_aux;
            n = 1;
        }
    }
    
    
    if (n > n_mode) {
        n_mode = n;
        elem_mode = elem;
    }
    
    return elem_mode; // dummy
}

double StatisticsDataFileDummyImpl::mediane() {
    // getValue() ja verifica o sort do arquivo
    
    // else take (n/2)-0,5
    
    if (_elems % 2 == 0) { // Even? Then take n/2 and (n/2)-1 and average them (-1 because index starts from zero)
        double aux = (_elems/2); //  n/2
        double auxTwo = aux - 1; // (n/2)-1
        
        return (aux + auxTwo)/2;
    } else { // Odd? Then take (n/2)-0.5
        return (_elems/2) - 0.5; // dummy
    }
}

double StatisticsDataFileDummyImpl::variance() {
    // (_variance * (_elems - 1) + pow(newValue - _average, 2)) / _elems
    // Using GMP
    // TODO substituir newValue por algo existente e que faca sentido
    mpf_t f_variance, f_elems, f_newValue, f_average, f_aux, f_auxTwo;
    mpf_inits(f_variance, f_elems, f_newValue, f_average, f_aux, f_auxTwo, NULL);
    
    mpf_set_ui(f_elems, _elems);
    mpf_set_d(f_newValue, newValue);
    mpf_set_d(f_average, _average);
    
    //mpf_sub_ui(f_aux, f_elems, 1);     // aux <-              _elems - 1
    //mpf_mul(f_aux, f_average, f_aux);  // aux <-  _average * (_elems - 1)
    //mpf_add(f_aux, f_aux, f_newValue); // aux <-  _average * (_elems - 1) + newValue
    //mpf_div(f_aux, f_aux, f_elems);    // aux <- (_average * (_elems - 1) + newValue) / _elems
    
    //mpf_set(f_average, f_aux); // Update f_average with calculated average
    
    mpf_sub_ui(f_aux, f_elems, 1);            //               _elems - 1
    mpf_mul(f_aux, f_variance, f_aux);        //  _variance * (_elems - 1)
    mpf_sub(f_auxTwo, f_newValue, f_average); //                                 newValue - _average
    mpf_pow_ui(f_auxTwo, f_auxTwo, 2);        //                             pow(newValue - _average, 2)
    mpf_add(f_aux, f_aux, f_auxTwo);          //  _variance * (_elems - 1) + pow(newValue - _average, 2)
    mpf_div(f_aux, f_aux, f_elems);           // (_variance * (_elems - 1) + pow(newValue - _average, 2)) / _elems
    
    _variance = mpf_get_d(f_aux);             // _variance <- float _variance (possivel truncamento)
    
    return _variance; // dummy
}

double StatisticsDataFileDummyImpl::stddeviation() {
    return sqrt(_average); // dummy
}

double StatisticsDataFileDummyImpl::variationCoef() {
    // _variationCoef = (_average != 0 ? _stddeviation / _average : 0.0)
    
    if (_average != 0) {
        _variationCoef = _stddeviation/_average;
    } else {
        _variationCoef = 0.0;
    }
    
    return _variationCoef; // dummy
}

double StatisticsDataFileDummyImpl::halfWidthConfidenceInterval() {
    // _halfWidth = _criticalTn_1 * (_stddeviation / std::sqrt(_elems))
    mpf_t f_elems, f_aux, f_auxTwo;
    mpf_inits(f_elems, f_aux, f_auxTwo, NULL);
    
    mpf_set_ui(f_elems, _elems);
    mpf_set_d(f_aux, _stddeviation);
    
    mpf_sqrt(f_auxTwo, f_elems);     //                                 std::sqrt(_elems)
    mpf_div(f_aux, f_aux, f_auxTwo); //                (_stddeviation / std::sqrt(_elems))
    mpf_set_d(f_auxTwo, _criticalTn_1); // Necessario pois nao existe mpf_mul_d()
    mpf_mul(f_aux, f_auxTwo, f_aux); //_criticalTn_1 * (_stddeviation / std::sqrt(_elems))
    
    _halfWidth = mpf_get_d(f_aux);
    
    return _halfWidth; // dummy
}

unsigned int StatisticsDataFileDummyImpl::newSampleSize(double halfWidth) {
    // TODO o que eh pra fazer aqui?
    return 0; // dummy
}

double StatisticsDataFileDummyImpl::quartil(unsigned short num) {
    if (num < 0 || num > 4) {
        // TODO throw exception?
    }
    
    // getValue() ja verifica o sort do arquivo
    
    double aux = (num * (_elems + 1))/4;
    int k = (int) aux;
    aux = aux - k;
    double xk = _collector->getValue(k-1); // k-1 pois array comeca em zero
    double xk_next = _collector->getValue(k);
    double auxTwo = xk_next - xk;
    
    double quartil = xk + (aux * auxTwo);
    
    return quartil; // dummy
}

double StatisticsDataFileDummyImpl::decil(unsigned short num) {
    if (num < 0 || num > 10) {
        // TODO throw exception?
    }
    
    // getValue() ja verifica o sort do arquivo
    
    double aux = (num * (_elems + 1))/10;
    int k = (int) aux;
    aux = aux - k;
    double xk = _collector->getValue(k-1); // k-1 pois array comeca em zero
    double xk_next = _collector->getValue(k);
    double auxTwo = xk_next - xk;
    
    double decil = xk + (aux * auxTwo);
    
    return decil; // dummy
}

double StatisticsDataFileDummyImpl::centil(unsigned short num) {
    if (num < 0 || num > 100) {
        // TODO throw exception?
    }
    
    // getValue() ja verifica o sort do arquivo
    
    double aux = (num * (_elems + 1))/100;
    int k = (int) aux;
    aux = aux - k;
    double xk = _collector->getValue(k-1); // k-1 pois array comeca em zero
    double xk_next = _collector->getValue(k);
    double auxTwo = xk_next - xk;
    
    double centil = xk + (aux * auxTwo);
    
    return centil; // dummy
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
}

void StatisticsDataFileDummyImpl::collectorClearHandler(){
    // do stuff to reset 
}
