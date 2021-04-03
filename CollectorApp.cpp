/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CollectorApp.cpp
 * Author: lrdass
 * 
 * Created on 24 de Março de 2021, 16:03
 */

#include "CollectorApp.h"
#include "DataFileArray.h"
#include "CollectorDatafile_if.h"
#include <gmp.h>
#include <stdlib.h>
#include <iostream>
#include "CollectorDatafileDefaultImpl1.h"

CollectorApp::CollectorApp() {
}

int CollectorApp::main(int argc, char** argv) {
//    std::cout << "Teste" << std::endl;
    

    StatisticsDataFileDummyImpl* imp = new StatisticsDataFileDummyImpl();
    
    CollectorDatafileDefaultImpl1* collector = imp->getCollector();
    
    collector->addValue(8.5);
    collector->addValue(3.4);
    collector->addValue(0.7);
    collector->addValue(7.1);
    collector->addValue(0.6);
    collector->addValue(5.2);
    collector->addValue(3.9);
    collector->addValue(10.0);
    collector->addValue(2.2);
    collector->addValue(2.9);
    collector->addValue(0.8);
    collector->addValue(1.6);
    collector->addValue(4.1);
    collector->addValue(7.3);
    collector->addValue(2.1);
    collector->addValue(9.6);
    collector->addValue(9.1);
    collector->addValue(4.6);
    collector->addValue(1.6);
    collector->addValue(4.7);
    
    std::cout<< "last value " << collector->getLastValue() << std::endl;
    
    for (int i = 0; i < 20;i++){
        printf("pos %d -> %f\n",i,collector->getValueOrdered(i));
    }
    
    printf("average = %f\n", imp->average());
    printf("mode = %f\n", imp->mode());
    printf("mediane = %f\n", imp->mediane());
    printf("quartil = %f\n", imp->quartil(1));
    printf("decil = %f\n", imp->decil(1));
    printf("centil = %f\n", imp->centil(1));
    printf("halfWidthConfidenceInterval = %f\n", imp->halfWidthConfidenceInterval());
    printf("min = %f\n", imp->min());
    printf("max = %f\n", imp->max());
    printf("newSampleSize = %d\n", imp->newSampleSize(imp->halfWidthConfidenceInterval()));
    printf("numElements = %d\n", imp->numElements());
    printf("variance = %f\n", imp->variance());
    printf("stddeviation = %f\n", imp->stddeviation());
    printf("variationCoef = %f\n", imp->variationCoef());
    
    imp->setHistogramNumClasses(4);
    printf("Frequency of Histogram Class 1 = %d\n", imp->histogramClassFrequency(1));
    printf("Frequency of Histogram Class 2 = %d\n", imp->histogramClassFrequency(2));
    printf("Frequency of Histogram Class 3 = %d\n", imp->histogramClassFrequency(3));
    printf("Frequency of Histogram Class 4 = %d\n", imp->histogramClassFrequency(4));
    printf("histogramNumClasses = %d\n", imp->histogramNumClasses());
}

