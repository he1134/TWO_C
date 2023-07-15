/* Copyright (c) 2001-2005 Kathleen M. Carley - Center for Computational Analysis of Social and Organizational Systems (CASOS), 
Institute for Software Research International (ISRI), School of Computer Science (SCS), Carnegie Mellon University (CMU), 
5000 Forbes Avenue - Pittsburgh, PA 15213-3890 - All Rights Reserved. */
/***************************************************************************
                          periodOut.h  -  description
                             -------------------
    begin                : March 2005
    email                : ojuarez@cs.cmu.edu
 ***************************************************************************/



#ifndef _periodout_h_
#define _periodout_h_
#pragma warning (disable:4786)
#pragma warning (disable:4503)

#include "actor.h"
#include "scenario.h"
#include "resource.h"
#include "action.h"
#include "enhanced_dense_matrix.hpp"
#include "actor_goal.h"
#include "beliefAction.h"
#include "beliefActor.h"
#include "influence.h"
#include "hostility.h"
#include "tendency.h"
#include "goal_effect.h"
#include "action_effect.h"
#include "actor_resource.h"
#include "simulation_events.h"
#include "action_resource_required.h"
#include "Agent.h"
#include "actor_resource_proportions.h"
#include "actor_action_proportions.h"
#include "influenceOut.h"
#include "hostilityOut.h"
#include "tendencyOut.h"
#include "resourceOut.h"
#include "action_set.h"
#include "agent_measures.h"
#include "resource.h"
#include "goal.h"
#include "math.h"
#include <sstream>
#include <stdlib.h>
#include "Connection.h"
#include <map>
//#include "simulation_events.h"
	typedef std::map<std::string,EnhancedDenseMatrixT<float>*> matrixContainer;
	typedef std::map<int,hostility*> hostilityContainer;
	typedef std::map<int,float> volatilityContainer;
	typedef std::map<int,float> tensionContainer;
	typedef std::map<std::string, double>   intellContainer;
	typedef std::map<std::string, float>   simContainer;

class periodOut {

private: std::string		config_filename ;
private: int				scenario_id;  
private: std::string		aggregation;
private: std::string		dsn; // DB Parameters
private: std::string		dbuser;
private: std::string		dbpwd;
private: int				sid;  //scenario ID
private: Connection			con;
private: int                nRun;
private: int                nTimePeriod;
private: std::vector<influenceOut*> influencesOut;
private: std::vector<hostilityOut*> hostilitiesOut;
private: std::vector<tendencyOut*>  tendenciesOut;
private: std::vector<resourceOut*>  resourcesOut;
private: std::map<std::string, agent_measures*>* actor_measures;
private: double averageActionsS;
private: double averageActionsR;
private: double averageActionsHS;
private: double averageActionsFS;
private: double averageActionsNS;
private: double averageActionsHR;
private: double averageActionsFR;
private: double averageActionsNR;
private: double hostilityAvg;
private: double influenceAvg;
private: double resourceSAvg;
private: double resourceRAvg;


public: periodOut(int scenario, int runR, int periodP);
public: void addInfluence(influenceOut* a);
public: void addHostility(hostilityOut* a);
public: void addTendnecy(tendencyOut* a);
public: void addMeasures(std::map<std::string, agent_measures*>* &am);
public: std::map<std::string, agent_measures*>* getMeasures();
public: std::vector<influenceOut*> getInfluences();
public: std::vector<hostilityOut*> getHostilities();
public: std::vector<tendencyOut*> getTendencies();
public: std::vector<resourceOut*> getResources();
public: int getRun();
public: int getPeriod();
public:	void printInfluence();
public:	void printHostility();
public: void printTendencies();
public: void printResources();
public: void computeAverages();
public: void addTendency(tendencyOut* a);
public: void addResource(resourceOut* a);
public: double getAvgActionsS();

public: double getAvgActionsR();
public: double getAvgActionsHS();

public: double getAvgActionsFS();

public: double getAvgActionsNS();

public: double getAvgActionsHR();

public: double getAvgActionsFR();

public: double getAvgActionsNR(); 
public: double getInfluenceAvg();
public: double getHostilityAvg();
public: double getAvgResourcesS();
public: double getAvgResourcesR();
public: void computeAveragesRes();

  
};

#endif












