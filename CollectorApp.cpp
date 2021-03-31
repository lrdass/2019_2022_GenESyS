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
#include <gmp.h>

CollectorApp::CollectorApp() {
}

int CollectorApp::main(int argc, char** argv) {
//    std::cout << "Teste" << std::endl;
    
    StatisticsDataFileDummyImpl* imp = new StatisticsDataFileDummyImpl();
    
    Collector_if* collector = imp->getCollector();
    for (double i = 0.0; i < 200; i++){
        collector->addValue(i);
       
    }
    
//    collector->addValue(1.0);
//    collector->addValue(3.0);
//    collector->addValue(1.5);
//    
//    std::cout<< collector->getLastValue() << std::endl;
//     
//    
//    
//    StatisticsDefaultImpl1 * statImp = new StatisticsDefaultImpl1();
//    Collector_if* coll = statImp->getCollector();

    DataFileArray* data = new DataFileArray("datafile.dat");

    
    data->write(40);
    
//    std::cout << data->read(40) << std::endl;
//    std::cout << data->read(41) << std::endl;
    
    for (double i =0; i < 201; i++){
        std::cout << data->read(i) << std::endl;
    }
    
//    
//    
//    statImp->average();
    
    

}

