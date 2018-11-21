/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ModelPersistence.cpp
 * Author: cancian
 * 
 * Created on 24 de Agosto de 2018, 19:22
 */

#include <iostream>

#include "ModelPersistenceDummyImpl.h"
#include "ModelComponent.h"

ModelPersistenceDummyImpl::ModelPersistenceDummyImpl(Model* model) {
	_model = model;
}

ModelPersistenceDummyImpl::ModelPersistenceDummyImpl(const ModelPersistenceDummyImpl& orig) {
}

ModelPersistenceDummyImpl::~ModelPersistenceDummyImpl() {
}

bool ModelPersistenceDummyImpl::saveAsTXT(std::string filename) {
	bool res = true;
	std::list<std::string>* words;
	// save model own infos
	// ...
	
	// save components
	_model->getTracer()->trace(Util::TraceLevel::mostDetailed, "Writing components\":"); 
	List<ModelComponent*>* components = this->_model->getComponents();
	Util::IncIndent();
	for (std::list<ModelComponent*>::iterator it = components->getList()->begin(); it != components->getList()->end(); it++) {
		words = (*it)->SaveInstance((*it));
		_saveLine(words);
	}
	Util::DecIndent();
	
	// save infras
	std::list<std::string>* infraTypenames = _model->getInfraManager()->getInfrastructureTypenames();
	for (std::list<std::string>::iterator itTypenames=infraTypenames->begin(); itTypenames!=infraTypenames->end(); itTypenames++) {
		List<ModelInfrastructure*>* infras = _model->getInfraManager()->getInfrastructures((*itTypenames));
		_model->getTracer()->trace(Util::TraceLevel::mostDetailed, "Writing infrastructures of type \"" + (*itTypenames) + "\":"); 
		Util::IncIndent();
		for (std::list<ModelInfrastructure*>::iterator it=infras->getList()->begin(); it!=infras->getList()->end(); it++) {
			_model->getTracer()->trace(Util::TraceLevel::mostDetailed, 	"Writing "+(*itTypenames)+" \"" + (*it)->getName() + "\""); 
			words = (*it)->SaveInstance((*it));
			_saveLine(words);
		}
		Util::DecIndent();
	}
}

bool ModelPersistenceDummyImpl::loadAsTXT(std::string filename) {

}

bool ModelPersistenceDummyImpl::saveAsXML(std::string filename) {
}

bool ModelPersistenceDummyImpl::loadAsXML(std::string filename) {
}

void ModelPersistenceDummyImpl::_saveLine(std::list<std::string>* words) {
	std::string line = "";
	for (std::list<std::string>::iterator it = words->begin(); it != words->end(); it++) {
		line += (*it) + " ; ";
	}
	_model->getTracer()->trace(Util::TraceLevel::mostDetailed, line);
}

bool ModelPersistenceDummyImpl::save(std::string filename) {
	return this->saveAsTXT(filename);
}

bool ModelPersistenceDummyImpl::load(std::string filename) {
	return this->loadAsTXT(filename);
}

bool ModelPersistenceDummyImpl::isSaved() {
	return _isSaved;
}