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

    _collectorDatafile = new Traits<Statistics_if>::CollectorDatafileImplementation();

    _collectorDatafile->setAddValueHandler(setCollectorAddValueHandler(&StatisticsDataFileDummyImpl::collectorAddHandler, this));
    _collectorDatafile->setClearHandler(setCollectorClearHandler(&StatisticsDataFileDummyImpl::collectorClearHandler, this));

    
    
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

    // TODO sorted? if no then sort
    
    long n_mode = 1; // qtd de repeticoes da moda
    double elem_mode = _collectorDatafile->getValueOrdered(0); // o elemento que eh a moda
    
    long n = 1; // qtd de repeticoes do valor sendo analisado
    double elem = elem_mode; // o elemento sendo analisado, comeca sendo o mode
    
    double elem_aux;
    
    // for comeca em 1 pois o primeiro item ja foi "lido"
    for(int i = 1; i < _elems; i++) {
        elem_aux = _collectorDatafile->getValue(i);
    
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
    // TODO sorted? if no then sort
    
    if (_elems % 2 == 0) { // Even? Then take n/2 and (n/2)-1 and average them (-1 because index starts from zero)
        double aux = (_elems/2); //  n/2
        double auxTwo = aux - 1; // (n/2)-1
        
        return (aux + auxTwo)/2;
    } else { // Odd? Then take (n/2)-0.5
        return (_elems/2) - 0.5; // dummy
    }
}

double StatisticsDataFileDummyImpl::variance() {

    // (_variance * (_elems - 1) + pow(lastValue - _average, 2)) / _elems
    
    if(!_wasAltered_variance) {
        return _variance;
    }
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia
    
    mpf_t f_variance, f_elems, f_lastValue, f_average, f_aux, f_auxTwo;
    mpf_inits(f_variance, f_elems, f_lastValue, f_average, f_aux, f_auxTwo, NULL);
    
    mpf_set_ui(f_elems, _elems);
    mpf_set_d(f_lastValue, _collectorDatafile->getLastValue());
    mpf_set_d(f_average, _average);
    mpf_set_d(f_variance, _variance);
    
    mpf_sub_ui(f_aux, f_elems, 1);             //               _elems - 1
    mpf_mul(f_aux, f_variance, f_aux);         //  _variance * (_elems - 1)
    mpf_sub(f_auxTwo, f_lastValue, f_average); //                                 lastValue - _average
    mpf_pow_ui(f_auxTwo, f_auxTwo, 2);         //                             pow(lastValue - _average, 2)
    mpf_add(f_aux, f_aux, f_auxTwo);           //  _variance * (_elems - 1) + pow(lastValue - _average, 2)
    mpf_div(f_aux, f_aux, f_elems);            // (_variance * (_elems - 1) + pow(lastValue - _average, 2)) / _elems
    
    _variance = mpf_get_d(f_aux);              // _variance <- float _variance (possivel truncamento)
    
    _wasAltered_variance = false;
    
    return _variance; // dummy
}

double StatisticsDataFileDummyImpl::stddeviation() {
    if(!_wasAltered_stddeviation) {
        return _stddeviation;
    }
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia
    
    mpf_t f_variance, f_stddeviation;
    mpf_inits(f_variance, f_stddeviation, NULL);
    
    mpf_set_d(f_variance, variance());
    mpf_sqrt(f_stddeviation, f_variance);
    
    _stddeviation = mpf_get_d(f_stddeviation);
    
    _wasAltered_stddeviation = false;
    
    return _stddeviation; // dummy
}

double StatisticsDataFileDummyImpl::variationCoef() {
    if(!_wasAltered_variationCoef) {
        return _variationCoef;
    }
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia
    
    mpf_t f_variationCoef, f_stddeviation, f_average, f_aux;
    mpf_inits(f_variationCoef, f_stddeviation, f_average, f_aux, NULL);
    
    mpf_set_d(f_stddeviation, stddeviation());
    mpf_set_d(f_average, _average);
    
    if (_average != 0) {
        mpf_div(f_variationCoef, f_stddeviation, f_average); // _variationCoef = _stddeviation/_average;
        _variationCoef = mpf_get_d(f_variationCoef);
    } else {
        _variationCoef = 0.0;
    }
    
    _wasAltered_variationCoef = false;
    
    return _variationCoef; // dummy
}

double StatisticsDataFileDummyImpl::halfWidthConfidenceInterval() {
    // _halfWidth = _criticalTn_1 * (_stddeviation / std::sqrt(_elems))
    if(!_wasAltered_halfWidth) {
        return _halfWidth;
    }    
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia
    
    mpf_t f_elems, f_aux, f_auxTwo;
    mpf_inits(f_elems, f_aux, f_auxTwo, NULL);
    
    mpf_set_ui(f_elems, _elems);
    mpf_set_d(f_aux, stddeviation());
    
    mpf_sqrt(f_auxTwo, f_elems);     //                                 std::sqrt(_elems)
    mpf_div(f_aux, f_aux, f_auxTwo); //                (_stddeviation / std::sqrt(_elems))
    mpf_set_d(f_auxTwo, _criticalTn_1); // Necessario pois nao existe mpf_mul_d()
    mpf_mul(f_aux, f_auxTwo, f_aux); //_criticalTn_1 * (_stddeviation / std::sqrt(_elems))
    
    _halfWidth = mpf_get_d(f_aux);
    
    _wasAltered_halfWidth = false;
    
    return _halfWidth; // dummy

}

unsigned int StatisticsDataFileDummyImpl::newSampleSize(double halfWidth) {
    // sampleSize = (_criticalTn_1 * _criticalTn_1 * _stddeviation * (1 - _stddeviation))/(halfWidth * 2)
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia
    
    mpf_t f_sampleSize, f_criticalTn_1, f_stddeviation, f_halfWidth, f_aux, f_auxTwo;
    mpf_inits(f_sampleSize, f_criticalTn_1, f_stddeviation, f_halfWidth, f_aux, f_auxTwo, NULL);
    
    mpf_set_d(f_criticalTn_1, _criticalTn_1);
    mpf_set_d(f_stddeviation, stddeviation());
    mpf_set_d(f_halfWidth, halfWidth);
    
    mpf_mul(f_aux, f_criticalTn_1, f_criticalTn_1);     //  _criticalTn_1 * _criticalTn_1
    mpf_mul(f_aux, f_aux, f_stddeviation);              //  _criticalTn_1 * _criticalTn_1 * _stddeviation
    mpf_ui_sub(f_auxTwo, 1, f_stddeviation);            //                                                   1 - _stddeviation
    mpf_mul(f_aux, f_aux, f_auxTwo);                    //  _criticalTn_1 * _criticalTn_1 * _stddeviation * (1 - _stddeviation)
    mpf_mul(f_auxTwo, f_halfWidth, f_halfWidth);        //                                                                        halfWidth * halfWidth
    mpf_div(f_sampleSize, f_aux, f_auxTwo);             // (_criticalTn_1 * _criticalTn_1 * _stddeviation * (1 - _stddeviation))/(halfWidth * halfWidth)
    
    // _stddeviation
    // _criticalTn_1 eh o Z-score
    return (unsigned int) mpf_get_d(f_sampleSize); // dummy
}

double StatisticsDataFileDummyImpl::quartil(unsigned short num) {
    if (num < 1 || num > 3) {
        throw std::invalid_argument("Value is not within bounds of this operation");
    }
    
    // TODO sorted? if no then sort
    
    double aux = (num * (_elems + 1))/4;            //            ( n * (elems + 1) ) / 4
    int k = (int) aux;
    aux = aux - k;                                  //          ( ( n * (elems + 1) ) / 4 ) - k
    double xk = _collectorDatafile->getValue(k-1);     // k-1 pois array comeca em zero
    double xk_next = _collectorDatafile->getValue(k);
    double auxTwo = xk_next - xk;                   //                                              ( x(k+1) - x(k) )
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia
    
    mpf_t f_aux, f_auxTwo;
    mpf_inits(f_aux, f_auxTwo, NULL);
    
    mpf_set_d(f_aux, aux);
    mpf_set_d(f_auxTwo, auxTwo);
    
    mpf_mul(f_aux, f_aux, f_auxTwo);
    
    double quartil = xk + (mpf_get_d(f_aux));       // x(k) + ( ( ( n * (elems + 1) ) / 4 ) - k ) * ( x(k+1) - x(k) )
    
    return quartil; // dummy
}

double StatisticsDataFileDummyImpl::decil(unsigned short num) {
    if (num < 1 || num > 9) {
        throw std::invalid_argument("Value is not within bounds of this operation");
    }
    
    // TODO sorted? if no then sort
    
    double aux = (num * (_elems + 1))/10;           //             ( n * (elems + 1) ) / 10
    int k = (int) aux;
    aux = aux - k;                                  //           ( ( n * (elems + 1) ) / 10 ) - k
    double xk = _collectorDatafile->getValue(k-1);     // k-1 pois array comeca em zero
    double xk_next = _collectorDatafile->getValue(k);
    double auxTwo = xk_next - xk;                   //                                               ( x(k+1) - x(k) )
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia
    
    mpf_t f_aux, f_auxTwo;
    mpf_inits(f_aux, f_auxTwo, NULL);
    
    mpf_set_d(f_aux, aux);
    mpf_set_d(f_auxTwo, auxTwo);
    
    mpf_mul(f_aux, f_aux, f_auxTwo);
    
    double decil = xk + (mpf_get_d(f_aux));       // x(k) + ( ( ( n * (elems + 1) ) / 10 ) - k ) * ( x(k+1) - x(k) )
    
    return decil; // dummy
}

double StatisticsDataFileDummyImpl::centil(unsigned short num) {
    if (num < 1 || num > 99) {
        throw std::invalid_argument("Value is not within bounds of this operation");
    }
    
    // TODO sorted? if no then sort
    
    double aux = (num * (_elems + 1))/100;           //         ( n * (elems + 1) ) / 100
    int k = (int) aux;
    aux = aux - k;                                  //        ( ( n * (elems + 1) ) / 100 ) - k
    double xk = _collectorDatafile->getValue(k-1);     // k-1 pois array comeca em zero
    double xk_next = _collectorDatafile->getValue(k);
    double auxTwo = xk_next - xk;                   //                                              ( x(k+1) - x(k) )
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia
    
    mpf_t f_aux, f_auxTwo;
    mpf_inits(f_aux, f_auxTwo, NULL);
    
    mpf_set_d(f_aux, aux);
    mpf_set_d(f_auxTwo, auxTwo);
    
    mpf_mul(f_aux, f_aux, f_auxTwo);
    
    double centil = xk + (mpf_get_d(f_aux));       // x(k) + ( ( ( n * (elems + 1) ) / 100 ) - k ) * ( x(k+1) - x(k) )
    
    return centil; // dummy
}

void StatisticsDataFileDummyImpl::setHistogramNumClasses(unsigned short num) {
    _histogramclassNum = num;
}

unsigned short StatisticsDataFileDummyImpl::histogramNumClasses() {
    //short num_classes = sqrt(_collector->numElements()) + 1;
    //return num_classes;
    return _histogramclassNum; // dummy
}

double StatisticsDataFileDummyImpl::histogramClassLowerLimit(unsigned short classNum) {
    double range = _max - _min;
    double amplitude = range/_histogramclassNum;
    return (_min + (classNum - 1) * amplitude);
    //return 0.0; // dummy
}

unsigned int StatisticsDataFileDummyImpl::histogramClassFrequency(unsigned short classNum) {
    unsigned int count = 0;
    double value;
    for(int i = 0; i < _elems; i++){
        value = _collectorDatafile->getValue(i);
        if(value < histogramClassLowerLimit(classNum)){/*pass*/}
        else if(value >= histogramClassLowerLimit(classNum + 1)) {
            break;
        } else {
            count++;
        }
    }
    return count; // dummy
}

Collector_if* StatisticsDataFileDummyImpl::getCollector() {
    return this->_collectorDatafile;
}

void StatisticsDataFileDummyImpl::setCollector(Collector_if* collector) {
    _collectorDatafile = collector;
}

double StatisticsDataFileDummyImpl::getConfidenceLevel(){
    return 0.0;
};

void StatisticsDataFileDummyImpl::setConfidenceLevel(double confidencelevel){
    
}

void StatisticsDataFileDummyImpl::collectorAddHandler(double newValue){
    // do stuff with new value
    _elems = _collectorDatafile->numElements();
    if(newValue < _min){
        _min = newValue;
    }
    if(newValue> _max){
        _max = newValue;
    }
    
    _sum += newValue;
    _average = _sum / _elems;

    
    _wasAltered_variance = true;
    _wasAltered_stddeviation = true;
    _wasAltered_variationCoef = true;
    _wasAltered_halfWidth = true;

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
