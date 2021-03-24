/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CollectorApp.h
 * Author: lrdass
 *
 * Created on 24 de Março de 2021, 16:03
 */

#ifndef COLLECTORAPP_H
#define COLLECTORAPP_H
#include "BaseConsoleGenesysApplication.h"
#include "iostream"

class CollectorApp : public BaseConsoleGenesysApplication {
public:
    CollectorApp();
public:
    virtual int main(int argc, char** argv);
};


#endif /* COLLECTORAPP_H */

