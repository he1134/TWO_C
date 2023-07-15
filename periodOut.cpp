/* Copyright (c) 2001-2005 Kathleen M. Carley - Center for Computational Analysis of Social and Organizational Systems (CASOS), 
Institute for Software Research International (ISRI), School of Computer Science (SCS), Carnegie Mellon University (CMU), 
5000 Forbes Avenue - Pittsburgh, PA 15213-3890 - All Rights Reserved. */
/***************************************************************************
                          periodOut.cpp  -  description
                             -------------------
    begin                : March 2005
    email                : ojuarez@cs.cmu.edu
 ***************************************************************************/

#include "periodOut.h"
 #include <fstream.h>

#include <sstream>

periodOut::periodOut(int sc, int run, int period){

	int sid = sc;
	nRun = run;
	nTimePeriod = period;
	averageActionsS =0;
    averageActionsR =0;
    averageActionsHS =0;
    averageActionsFS =0;
    averageActionsNS =0;
    averageActionsHR =0;
    averageActionsFR =0;
    averageActionsNR =0;

	hostilityAvg =0;
	influenceAvg = 0;
	resourceRAvg = 0;
	resourceSAvg = 0;

}

int periodOut::getRun(){
	return nRun;
}

int periodOut::getPeriod(){
	return nTimePeriod;
}

std::vector<influenceOut*> periodOut::getInfluences(){
	return influencesOut;
}

std::vector<tendencyOut*> periodOut::getTendencies(){
	return tendenciesOut;
}

std::vector<hostilityOut*> periodOut::getHostilities(){
	return hostilitiesOut;
}

void periodOut::addInfluence(influenceOut* a){
	influencesOut.push_back(a);
}

void periodOut::addHostility(hostilityOut* a){
	hostilitiesOut.push_back(a);
}

void periodOut::addTendency(tendencyOut* a){
	tendenciesOut.push_back(a);
}

void periodOut::addResource(resourceOut* a){
	resourcesOut.push_back(a);
}


void periodOut::addMeasures(std::map<std::string, agent_measures*>* &am){
    actor_measures = am;
}
std::map<std::string, agent_measures*>* periodOut::getMeasures(){
	return actor_measures;
}

void periodOut::printInfluence(){
	char num1[128];
	char num2[128];

	std::string fileName = "c:\\code\\casos\\giant\\Outputs\\Influence";
	itoa(nRun, num1,10);
	itoa(nTimePeriod, num2,10);
	fileName = fileName + num1+"-"+num2+".txt";

	ofstream file_op(fileName.c_str(),ios::out);
    for(std::vector<influenceOut*>::iterator itIn = influencesOut.begin(); itIn != influencesOut.end(); itIn++){
	//	std::string tempo =(*itIn)->getSender()<<","<<(*itIn)->getReceiver()<<","<<(*itIn)->getInfluenceVal()<<",";
	//	tempo = tempo +std::string(num1)+","+std::string(num2)+std::endl;
	//	file_op<<(*itIn)->getSender()<<","<<(*itIn)->getReceiver()<<std::endl;
	//	std::string tempo = std::string((*itIn)->getSender());
		char num3[20];
	//	int decimal, sign;
		gcvt((*itIn)->getInfluenceVal(), 6, num3);
		float theVal = (*itIn)->getInfluenceVal();
		std::string tempo =(*itIn)->getSender()+","+(*itIn)->getReceiver()+","+num3+","+std::string(num1)+","+std::string(num2)+"\n";
	//	tempo = tempo +std::string(num1)+","+std::string(num2)+std::endl;
		file_op<<tempo.c_str();
	}
	file_op.close();

}

void periodOut::printHostility(){
	char num1[128];
	char num2[128];

	std::string fileName = "c:\\code\\casos\\giant\\Outputs\\hostil";
	itoa(nRun, num1,10);
	itoa(nTimePeriod, num2,10);
	fileName = fileName + num1+"-"+num2+".txt";

	ofstream file_op(fileName.c_str(),ios::out);
    for(std::vector<hostilityOut*>::iterator hoIn = hostilitiesOut.begin(); hoIn != hostilitiesOut.end(); hoIn++){
	//	std::string tempo =(*itIn)->getSender()<<","<<(*itIn)->getReceiver()<<","<<(*itIn)->getInfluenceVal()<<",";
	//	tempo = tempo +std::string(num1)+","+std::string(num2)+std::endl;
	//	file_op<<(*itIn)->getSender()<<","<<(*itIn)->getReceiver()<<std::endl;
	//	std::string tempo = std::string((*itIn)->getSender());
		char num3[20];;
	//	int decimal, sign;
	//	num3 =  fcvt((*hoIn)->getHostilityVal(), 5, &decimal, &sign);
		gcvt((*hoIn)->getHostilityVal(), 6, num3);
		std::string tempo =(*hoIn)->getSender()+","+(*hoIn)->getReceiver()+","+std::string(num3)+","+std::string(num1)+","+std::string(num2)+"\n";
	//	tempo = tempo +std::string(num1)+","+std::string(num2)+std::endl;
		file_op<<tempo.c_str();
	}
	file_op.close();

}

void periodOut::printTendencies(){
	char num1[128];
	char num2[128];

	std::string fileName = "c:\\code\\casos\\giant\\Outputs\\tendency";
	itoa(nRun, num1,10);
	itoa(nTimePeriod, num2,10);
	fileName = fileName + num1+"-"+num2+".txt";

	ofstream file_op(fileName.c_str(),ios::out);
    for(std::vector<tendencyOut*>::iterator teIn = tendenciesOut.begin(); teIn != tendenciesOut.end(); teIn++){
	//	std::string tempo =(*itIn)->getSender()<<","<<(*itIn)->getReceiver()<<","<<(*itIn)->getInfluenceVal()<<",";
	//	tempo = tempo +std::string(num1)+","+std::string(num2)+std::endl;
	//	file_op<<(*itIn)->getSender()<<","<<(*itIn)->getReceiver()<<std::endl;
	//	std::string tempo = std::string((*itIn)->getSender());
		char num3[20];
	//	int decimal, sign;
	//	num3 =  fcvt((*teIn)->getTendencyVal(), 5, &decimal, &sign);
		gcvt((*teIn)->getTendencyVal(), 6, num3);
		std::string tempo =(*teIn)->getActor()+","+(*teIn)->getActionType()+","+std::string(num3)+","+std::string(num1)+","+std::string(num2)+"\n";
	//	tempo = tempo +std::string(num1)+","+std::string(num2)+std::endl;
		file_op<<tempo.c_str();
	}
	file_op.close();

}

double periodOut::getAvgResourcesR(){
	return resourceRAvg;
}

double periodOut::getAvgResourcesS(){
	return resourceSAvg;
}

double periodOut::getHostilityAvg(){
	return hostilityAvg;
}

double periodOut::getInfluenceAvg(){
	return influenceAvg;
}

double periodOut::getAvgActionsS(){
	return averageActionsS;
}

double periodOut::getAvgActionsR(){
	return averageActionsR;
}

double periodOut::getAvgActionsHS(){
	return averageActionsHS;
}

double periodOut::getAvgActionsFS(){
	return averageActionsFS;
}

double periodOut::getAvgActionsNS(){
	return averageActionsNS;
}

double periodOut::getAvgActionsHR(){
	return averageActionsHR;
}

double periodOut::getAvgActionsFR(){
	return averageActionsFR;
}

double periodOut::getAvgActionsNR(){
	return averageActionsNR;
}



void periodOut::computeAverages(){
	std::map<std::string, agent_measures*>* tempMap  = actor_measures;
	int dataCounter =0;
	for(std::map<std::string, agent_measures*>::iterator itAM=tempMap->begin(); itAM != tempMap->end(); itAM++){
		averageActionsS = (*itAM).second->getActionsTaken()+ averageActionsS;
		averageActionsR = (*itAM).second->getActionsReceived()+averageActionsR;
		averageActionsHS = (*itAM).second->getActionsHostileSent()+averageActionsHS;;
        averageActionsFS =(*itAM).second->getActionsFriendlySent()+averageActionsFS;
		averageActionsNS = (*itAM).second->getActionsNeutralSent()+averageActionsNS;
		averageActionsHR = (*itAM).second->getActionsHostileReceived()+averageActionsHR;
		averageActionsFR = (*itAM).second->getActionsFriendlyReceived()+averageActionsFR;
		averageActionsNR = (*itAM).second->getActionsNeutralReceived()+averageActionsNR;
		hostilityAvg = (*itAM).second->getHostilityAvg()+ hostilityAvg;
		influenceAvg = (*itAM).second->getInfluenceAvg()+influenceAvg;
		dataCounter++;

	}
		averageActionsS = averageActionsS /dataCounter;
		averageActionsR = averageActionsR /dataCounter;
	    averageActionsHS = averageActionsHS / dataCounter;
		averageActionsFS = averageActionsFS/dataCounter;
		averageActionsNS = averageActionsNS/dataCounter;
        averageActionsHR = averageActionsHR / dataCounter;
		averageActionsFR = averageActionsFR/dataCounter;
		averageActionsNR = averageActionsNR/dataCounter;
		hostilityAvg = hostilityAvg/dataCounter;
		influenceAvg = influenceAvg/dataCounter;

}

void periodOut::computeAveragesRes(){
	std::map<std::string, agent_measures*>* tempMap  = actor_measures;
	int dataCounter =0;
	std::vector<simulation_events*> tempEventsS;
	std::vector<simulation_events*> tempEventsR;
	resourceSAvg = 0;
	for(std::map<std::string, agent_measures*>::iterator itAM=tempMap->begin(); itAM != tempMap->end(); itAM++){
		//averageActionsS = (*itAM).second->getActionsTaken()+ averageActionsS;
		tempEventsS = (*itAM).second->getSendedEvents();
		for(std::vector<simulation_events*>::iterator itSE=tempEventsS.begin(); itSE != tempEventsS.end(); itSE++){
            std::map<std::string, float> tempResource=(*itSE)->getResources();
			for(std::map<std::string, float>::iterator itR= tempResource.begin(); itR!=tempResource.end(); itR++){
				resourceSAvg = resourceSAvg + (*itR).second;
			}
		}
		dataCounter++;

	}
  resourceSAvg = resourceSAvg /dataCounter;
  dataCounter = 0;
	resourceRAvg = 0;
   for(itAM=tempMap->begin(); itAM != tempMap->end(); itAM++){
		//averageActionsS = (*itAM).second->getActionsTaken()+ averageActionsS;
		tempEventsR = (*itAM).second->getReceivedEvents();
		for(std::vector<simulation_events*>::iterator itRE=tempEventsR.begin(); itRE != tempEventsR.end(); itRE++){
            std::map<std::string, float> tempResource=(*itRE)->getResources();
			for(std::map<std::string, float>::iterator itR= tempResource.begin(); itR!=tempResource.end(); itR++){
				resourceRAvg = resourceRAvg + (*itR).second;
			}
		}
		dataCounter++;

	}






		
		resourceRAvg = resourceRAvg /dataCounter;
	    

}

void periodOut::printResources(){
	char num1[128];
	char num2[128];

	std::string fileName = "c:\\code\\casos\\giant\\Outputs\\resource";
	itoa(nRun, num1,10);
	itoa(nTimePeriod, num2,10);
	fileName = fileName + num1+"-"+num2+".txt";

	ofstream file_op(fileName.c_str(),ios::out);
    for(std::vector<resourceOut*>::iterator reIn = resourcesOut.begin(); reIn != resourcesOut.end(); reIn++){
	//	std::string tempo =(*itIn)->getSender()<<","<<(*itIn)->getReceiver()<<","<<(*itIn)->getInfluenceVal()<<",";
	//	tempo = tempo +std::string(num1)+","+std::string(num2)+std::endl;
	//	file_op<<(*itIn)->getSender()<<","<<(*itIn)->getReceiver()<<std::endl;
	//	std::string tempo = std::string((*itIn)->getSender());
		char num3[20];
	//	int decimal, sign;
	//	num3 =  fcvt((*teIn)->getTendencyVal(), 5, &decimal, &sign);
		gcvt((*reIn)->getResourceVal(), 6, num3);
		std::string tempo =(*reIn)->getOwner()+","+(*reIn)->getResource()+","+std::string(num3)+","+std::string(num1)+","+std::string(num2)+"\n";
	//	tempo = tempo +std::string(num1)+","+std::string(num2)+std::endl;
		file_op<<tempo.c_str();
	}
	file_op.close();

}






















