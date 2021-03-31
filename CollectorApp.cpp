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

#include <gmp.h>

CollectorApp::CollectorApp() {
}

int CollectorApp::main(int argc, char** argv) {
//    std::cout << "Teste" << std::endl;
    
//    StatisticsDataFileDummyImpl* imp = new StatisticsDataFileDummyImpl();
//    Collector_if* collector = imp->getCollector();
//    
//    collector->addValue(1.0);
//    collector->addValue(3.0);
//    collector->addValue(1.5);
//    
//    std::cout<< collector->getLastValue() << std::endl;
    /*
    
    mp_bitcnt_t z = mpf_get_default_prec(); // Variavel z contem a precisao default de float do GMP
    mpf_set_default_prec(z); // set a precisao com o valor default para esta instancia

    mpf_t result, base; // Crio duas variaveis float do GMP

    mpf_inits(result, base, NULL);	// Inicializo as variaveis float do GMP
                                    // Necessario o NULL para evitar Segmentation Fault

    mpf_set_d(base, 2.4); // Set o valor de BASE para o double 2.2

    mpf_pow_ui(result, base, 20000); // Set RESULT para BASE elevado ao long int 2

    mpf_out_str(stdout, 10, 120, result);	// Print em stdout, na base 10, com 120 digitos, a variavel RESULT
                                            // ARGS: Output, Base Numerica, Qtd digitos, Var

    // Abaixo eu verifico a conversao de GMP float para double
    std::cout << std::endl;

    double a = mpf_get_d(result); // Metodo GMP para obter um double truncado do float

    std::cout << a << std::endl;
    
    */
    
    StatisticsDefaultImpl1 * statImp = new StatisticsDefaultImpl1();
    Collector_if* coll = statImp->getCollector();
    coll->addValue(3);
    coll->addValue(5);
    
    
    statImp->average();
    
    

}

