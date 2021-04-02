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

    collector->addValue(1.0);
    collector->addValue(3.0);
    collector->addValue(1.5);
    
    std::cout<< "last value " << collector->getLastValue() << std::endl;
    
    std::cout<< "pos 0 " << collector->getValueOrdered(0) << std::endl;
    

   
  
    
//    
//    
//    statImp->average();

    
    

}

