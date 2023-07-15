/* Copyright (c) 2001-2005 Kathleen M. Carley - Center for Computational Analysis of Social and Organizational Systems (CASOS), 
Institute for Software Research International (ISRI), School of Computer Science (SCS), Carnegie Mellon University (CMU), 
5000 Forbes Avenue - Pittsburgh, PA 15213-3890 - All Rights Reserved. */
/***************************************************************************
                          simulation_engine.cpp  -  description
                             -------------------
    begin                : March 2005
    email                : ojuarez@cs.cmu.edu
 ***************************************************************************/

#include "simulation_engine.h"
#include <sstream>

simulation_engine::simulation_engine(scenarioOBJ* sc, outputOBJ* oo){

		currentScenario = sc;
		//Connection con;
		dsn = "giantdb";
		dbuser = "ojuarez";
		dbpwd = "zorrote";
		con = currentScenario->getConnection();
	//	con.close();
		
		if(!con.isConnected()) {
			con.getConnection(dsn.c_str(), dbuser.c_str(), dbpwd.c_str());
//			std::cout<<" There is not connection  "<<std::endl;
		}

		sid = currentScenario->getScenarioID();
		scenario_id = sid;
		actors = currentScenario->getActors();
		actions = currentScenario->getActions();
		action_resource_requireds = currentScenario->getActionResourceRequired();
		influences = currentScenario->getInfluence();
		action_types = currentScenario->getActionTypes();
		resource_types = currentScenario->getResourceTypes();
		actor_resources = currentScenario->getActorResources();
        goals = currentScenario->getGoals();
		hostilities = currentScenario->getHostilities();
		resource_proportions = currentScenario->getResourceProportions();
		action_proportions = currentScenario->getActionProportions();
        resources = currentScenario->getResources();
        tensions = currentScenario->getTensions();
		volatilities = currentScenario->getVolatilities();
		intelligences = currentScenario->getIntelligences();
		tendencies = currentScenario->getTendencies();
        nRuns =  currentScenario->getRuns();
		actor_goals = currentScenario->getActorGoals();
		goal_effects = currentScenario->getGoalEffects();
		action_effects = currentScenario->getActionEffects();
		nTimePeriods = currentScenario->getTimes();
		currentOutput = oo;
		
		
        
}

simulation_engine::~simulation_engine(){
	actors.clear();
}

void simulation_engine::print_actor_measures(std::map<std::string, agent_measures*>* &actor_measures){
	for(std::map<std::string, agent_measures*>::iterator amIt= actor_measures->begin(); amIt!= actor_measures->end(); amIt++){
		agent_measures* amVal = (*amIt).second;
		std::string   actor = (*amIt).first;
//        std::cout<<"El Actor  selecciones "<<actor<<std::endl;
		std::vector<simulation_events*> ag_events = amVal->getSendedEvents();
		for(std::vector<simulation_events*>::iterator seIt=ag_events.begin(); seIt!= ag_events.end(); seIt++){
		//	std::cout<<(*seIt)->getActName();
			std::cout<<(*seIt)->getActName()<<" "<<(*seIt)->getReceiver()<<" "<<(*seIt)->getDirection()<<std::endl;
		}

	}

}

void simulation_engine::print_action_rankings(std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >  &spec_actions_Rank, std::vector<actor*> &actors, std::vector<std::string> &action_types){
	std::multimap<float,std::string, std::greater<float> >* tempMap;
	for(std::vector<actor*>::iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
		std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >::iterator it_spec_actions=spec_actions_Rank.find((*act_iter20)->name());
	    tempMap = it_spec_actions->second;
        std::multimap<float,std::string, std::greater<float> >::iterator mientras= tempMap->begin();
		std::cout<<"Actor      ::"<<(*act_iter20)->name()<<std::endl;
		while (mientras != tempMap->end()){
			std::cout<<mientras->first<<" ::: "<<mientras->second<<std::endl;
			mientras++;
		}
	}

}



void simulation_engine::print_actions_actor(std::map<std::string, matrixContainer> actorActionProbabilities, std::vector<actor*> &actors, std::vector<std::string> &action_types){
    int row=0;
	for(std::vector<actor*>::iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
		std::cout<<"actor Name "<<(*act_iter20)->name()<<std::endl;
       std::map<std::string, matrixContainer>::iterator tempMatrixMap= actorActionProbabilities.find((*act_iter20)->name());
	   for(std::vector<std::string>::iterator at_iter = action_types.begin(); at_iter != action_types.end(); at_iter++){
//		   std::cout<<"===========================Action Type==="<<(*at_iter)<<std::endl;
		 //  matrixContainer::iterator tempActorActor = tempMatrixMap->find((*at_iter));
		   std::map<std::string, EnhancedDenseMatrixT<float>*> tempActorActor = (*tempMatrixMap).second;
           std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator tempMatrix = tempActorActor.find((*at_iter));
           EnhancedDenseMatrixT<float>* temp_ActorActorAction = tempMatrix->second;
           //temp_ActorActorAction->dump();
		   int row=0;
           for(std::vector<actor*>::iterator act_iterS = actors.begin(); act_iterS != actors.end(); ++act_iterS){

                int col=0;
				for(std::vector<actor*>::iterator act_iterT = actors.begin(); act_iterT != actors.end(); ++act_iterT){
					if ((*temp_ActorActorAction)(row,col)!=0)
						std::cout<<(*act_iterS)->name()<<" +++++++++++++++++++++++++"<<(*act_iterT)->name()<<" "<<(*temp_ActorActorAction)(row,col)<<std::endl;
					col++;
				}
				row++;
		   }
	   }
	}
}


void simulation_engine::compute_actor_resource_type( EnhancedDenseMatrixT<float>* &actor_x_goal, std::vector<std::string> &resource_types, std::map<std::string, EnhancedDenseMatrixT<float>*> &goal_x_resource_type_x_actor, std::vector<actor*> &actors, std::map<std::string, EnhancedDenseMatrixT<float>*>  &results_actor_resources){

  EnhancedDenseMatrixT<float>* temp_results_matrix;
  for(std::map<std::string, EnhancedDenseMatrixT<float>*>::const_iterator m_mat = goal_x_resource_type_x_actor.begin();m_mat != goal_x_resource_type_x_actor.end(); m_mat++){
	  temp_results_matrix= new EnhancedDenseMatrixT<float>(actors.size()+1, resource_types.size()+1);
	  addMatrixActorNames(temp_results_matrix, actors, 1);
	  addMatrixResourceNames(temp_results_matrix, resource_types, 0);
	  matrix::multiply(*actor_x_goal, *(m_mat->second), *temp_results_matrix);
    //std::cout<<(m_mat)->first<<std::endl;
    // temp_results_matrix->dump();
		  results_actor_resources.insert(std::make_pair(std::string((m_mat)->first),temp_results_matrix));
  }
}

bool simulation_engine::find_goal_effects(std::vector<goal_effect*> &goal_effects, int gid, int aid, std::string rt, double* geVal ){
	for(std::vector<goal_effect*>::iterator geIt = goal_effects.begin(); geIt!=goal_effects.end(); geIt++){
		if((gid == (*geIt)->goal_id()) && (aid == (*geIt)->resource_owner()) && (rt == (*geIt)->resource_name()) ){
		  *geVal = (*geIt)->value();
	       return true;
		}
	}
    return false;
}

//void simulation_engine::compute_goal_resource_type_actor(Connection &con, std::vector<goal*> &goals, std::vector<actor*> &actors, std::vector<std::string> &resource_types,  std::map<std::string, EnhancedDenseMatrixT<float>*> &goal_x_resource_type_x_actor){
void simulation_engine::compute_goal_resource_type_actor(std::vector<goal_effect*> goal_effects, std::vector<goal*> &goals, std::vector<actor*> &actors, std::vector<std::string> &resource_types,  std::map<std::string, EnhancedDenseMatrixT<float>*> &goal_x_resource_type_x_actor){

  for(std::vector<actor*>::const_iterator aiter = actors.begin(); aiter!= actors.end(); ++ aiter){

    EnhancedDenseMatrixT<float>* goal_resource_type_matrix = new EnhancedDenseMatrixT<float>(goals.size()+1, resource_types.size()+1);
    goal_resource_type_matrix->zeros(goals.size()+1, resource_types.size()+1);

    int gidx = 0;
    for(std::vector<goal*>::const_iterator giter = goals.begin(); giter != goals.end(); ++giter){
      
      goal_resource_type_matrix->rowname(gidx, (*giter)->name());
	  //std::cout<<(*giter)->name()<<std::endl;
      
      int rt_idx = 0;
      for(std::vector<std::string>::const_iterator rt_iter = resource_types.begin(); rt_iter != resource_types.end(); ++rt_iter){
        goal_resource_type_matrix->colname(rt_idx, (*rt_iter));
       /* goal_effect g(&con);

        if(g.open((*giter)->id(), (*aiter)->id(), (*rt_iter))){
            
		//	std::cout<<"========"<<(*rt_iter)<<std::endl;
            goal_resource_type_matrix->set(gidx, rt_idx, g.value());
            ++rt_idx;
        } */
		  double geVal;
		  if (find_goal_effects(goal_effects,(*giter)->id(),(*aiter)->id(),(*rt_iter), &geVal )){
				goal_resource_type_matrix->set(gidx, rt_idx, geVal);
		  }
		++rt_idx;
      }
	   ++gidx;
      
    }
 //   goal_resource_type_matrix->dump();
	// std::cout<<(*aiter)->name()<<std::endl;
   //  goal_resource_type_matrix->dump();
      goal_x_resource_type_x_actor.insert(std::make_pair(std::string((*aiter)->name()), goal_resource_type_matrix));
  }
 }

void simulation_engine::addMatrixResourceNames(EnhancedDenseMatrixT<float>* theMatrix, std::vector<std::string> &resource_types, int row){
	int counter = 0;
	
	for (std::vector<std::string>::iterator it_rt = resource_types.begin();it_rt != resource_types.end(); it_rt++){
		if (row == 1){
			theMatrix->rowname(counter, (*it_rt));
		} else {
			theMatrix->colname(counter, (*it_rt));
		}
		counter++;
	}
}

bool simulation_engine::find_hostility(std::vector<hostility*> &hostilities, int act1, int act2, double* hosVal){
		int actorS ;
		int actorT ;
	if (act1 == act2) return false;
    std::vector<hostility*>::iterator hosIt = hostilities.begin();
	while (hosIt != hostilities.end()){
//	for(std::vector<hostility*>::iterator hosIt = hostilities.begin(); hosIt != hostilities.end(); hosIt++){
		actorS = (*hosIt)->source_actor();
		actorT = (*hosIt)->target_actor();

		if ( ( actorS == act1) && ( actorT == act2)){
			*hosVal = (*hosIt)->value();
			return true;
		}
		hosIt++;
	}
	return false;
}

void simulation_engine::get_hostility_matrix(std::vector<hostility*> &hostilities, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &hostilityMat){
	hostilityMat->zeros(actors.size()+1, actors.size()+1);
	int rows = 0;
	for(std::vector<actor*>::const_iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
		hostilityMat->rowname(rows, std::string((*act_iter2)->name()));
		hostilityMat->colname(rows, std::string((*act_iter2)->name()));
		rows++;
	}
	rows =0;
	for(act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
        //influence_matrix->colname(infexc, (*act_iter2)->name());
		//hostilityMat->rowname(rows, std::string((*act_iter2)->name()));
	    int cols = 0;
        for(std::vector<actor*>::const_iterator act_iter6 = actors.begin(); act_iter6 != actors.end(); ++act_iter6){
		
			double hosVal =0;
		//	std::cout<<"the Value is "<<(*act_iter2)->id()<<" "<<(*act_iter6)->id()<<std::endl;
			if (find_hostility(hostilities, (*act_iter2)->id(),(*act_iter6)->id(), &hosVal)){

		 		hostilityMat->set(rows, cols, hosVal);
			}
			cols++;
		}
		rows++;
      }
//  hostilityMat->dump();
  
	std::cerr<<"--------end hostility--------------"<<std::endl;  

}

std::string simulation_engine::getDirection(std::vector<action*> &actions, int actId){
	std::string strReturn="none";
	for (std::vector<action*>::iterator itAct=actions.begin(); itAct != actions.end(); itAct++){
		if ((*itAct)->id() == actId)
			return (*itAct)->direction();
	}
	return strReturn;
}

std::string simulation_engine::find_actor_name(std::vector<actor*> &actors, int actor_id){
	for(std::vector<actor*>::const_iterator act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
		if ((*act_iter)->id() == actor_id){
			return ((*act_iter)->name());
		}
	}
	return "";
}

void simulation_engine::Tokenize(const std::string& str,
			  std::vector<std::string>& tokens,
			  const std::string& delimiters = " ")
{
    // Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

action* simulation_engine::find_action(std::vector<action*> &actions, int aNumber){
	action* tempAction = NULL;
	for (std::vector<action*>::iterator iAction=actions.begin(); iAction != actions.end(); iAction++){
		tempAction = *iAction;
		if (tempAction->id() == aNumber)
			return tempAction;
	}
	return tempAction;
}

void simulation_engine::init_actions_taken(std::vector<actor*> &actors, std::map<std::string, std::vector<std::string> >  &actionsTaken){
	std::vector<std::string> *temporalActionTaken; 
	for(std::vector<actor*>::iterator ac_iter=actors.begin(); ac_iter != actors.end(); ac_iter++){
		temporalActionTaken = new std::vector<std::string>;
		actionsTaken.insert(std::make_pair((*ac_iter)->name(),*temporalActionTaken));
	}
}


void simulation_engine::addMatrixActorNames(EnhancedDenseMatrixT<float>* theMatrix, std::vector<actor*> &actors, int row){
	int counter = 0;
	
	for (std::vector<actor*>::iterator it_act = actors.begin();it_act != actors.end(); it_act++){
		if (row == 1){
			theMatrix->rowname(counter, std::string((*it_act)->name()));
		} else {
			theMatrix->colname(counter, std::string((*it_act)->name()));
		}
		counter++;
	}
}

void simulation_engine::compute_actor_resource_receiver(std::vector<std::string> &resource_types, std::vector<actor*> &actors, std::map<std::string, EnhancedDenseMatrixT<float>*>  &results_actor_resources, std::map<std::string, EnhancedDenseMatrixT<float>*>  &actor_resources_receiver){

  EnhancedDenseMatrixT<float>* temp_results_matrix;
  EnhancedDenseMatrixT<float>* matrix_sender_rt;
  for (std::vector<actor*>::iterator it_act=actors.begin(); it_act != actors.end(); it_act++){
	 matrix_sender_rt = new EnhancedDenseMatrixT<float>(actors.size()+1, resource_types.size()+1);
	 addMatrixActorNames(matrix_sender_rt, actors, 1);
	 addMatrixResourceNames(matrix_sender_rt, resource_types, 0);
		
	for (std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator oldIt=results_actor_resources.begin();oldIt != results_actor_resources.end(); oldIt++){
		temp_results_matrix = (*oldIt).second;
		//temp_results_matrix->dump();
		std::string sender = (*oldIt).first;
		int rowActor = temp_results_matrix->rowIndex((*it_act)->name());
		int rowActor2 = temp_results_matrix->rowIndex(sender);
		for(std::vector<std::string>::iterator rIter=resource_types.begin(); rIter != resource_types.end(); rIter++){
			int colResource = temp_results_matrix->colIndex((*rIter));
			matrix_sender_rt->set(rowActor2,colResource,(*temp_results_matrix)(rowActor, colResource));
		}

	}

//    matrix_sender_rt->dump();
    actor_resources_receiver.insert(std::make_pair(std::string((*it_act)->name()),matrix_sender_rt ));
  }
}

bool simulation_engine::find_influence(std::vector<influence*> &influences, int aid1, int aid2, double* infVal){
	for (std::vector<influence*>::iterator infIt= influences.begin(); infIt != influences.end(); infIt++){
		if (((*infIt)->source_actor() == aid1) && ((*infIt)->target_actor() == aid2) ){
			*infVal = (*infIt)->strength();
			return true;
		}
	}
	return false;
}

void simulation_engine::get_initial_influence_matrix(std::vector<influence*> &influences, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &influence_matrix){
	
  influence_matrix->zeros(actors.size()+1, actors.size()+1);
  int infex = 0;
   // for(std::vector<action*>::const_iterator giter = goals.begin(); giter != goals.end(); ++giter){
      for(std::vector<actor*>::const_iterator act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
		  influence_matrix->rowname(infex, std::string((*act_iter)->name()));
		  influence_matrix->colname(infex, std::string((*act_iter)->name()));
      ++infex;
	  };
	  infex = 0;
	  for(act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
		int infexc = 0;
		for(std::vector<actor*>::const_iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
        //influence_matrix->colname(infexc, (*act_iter2)->name());
			/*influence infdata(&con);
			if(infdata.open((*act_iter)->id(), (*act_iter2)->id()))
				influence_matrix->set(infex, infexc, infdata.strength());
			*/
			double infVal = 0;
			if(find_influence(influences, (*act_iter)->id(), (*act_iter2)->id(), &infVal)){
				influence_matrix->set(infex, infexc, infVal);
			}
			++infexc;
        
		}
		++infex;
	  }

//     influence_matrix->dump();
//	 std::cout<<"hola"<<std::endl;

}

//std::map<std::string, actor_measures*> actor_measures;
void simulation_engine::populate_initial_measures(std::vector<actor*> &actors, std::map<std::string, agent_measures*>* &actor_measures){
	agent_measures* tmp_actor_measures;
	for(std::vector<actor*>::const_iterator act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
		tmp_actor_measures = new agent_measures();
		actor_measures->insert(std::make_pair((*act_iter)->name(), tmp_actor_measures));

	}

}

void simulation_engine::getSimilarities(std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &hostilityMat, EnhancedDenseMatrixT<float>* &influence_matrix, std::map<std::string, simContainer* > &similarities){
	std::cerr<<"--------begin similarity--------------"<<std::endl;  
  //  simContainer* temp_Actor_Sims;
	double simValue;
	std::map<std::string, float>* temp_Actor_Sims;
//	influence_matrix->dump();
//	hostilityMat->dump();
	std::map<std::string, double> hola;
	int rc=0;
	for(std::vector<actor*>::iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
		int row1 = influence_matrix->rowIndex((*act_iter2)->name());
		int cc=0;
		temp_Actor_Sims = new std::map<std::string, float>;
	//	temp_Actor_Sims = new std::map(const hola);
		for(std::vector<actor*>::iterator  act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
	//	int row2 = hostilityMat->rowIndex((*act_iter)->name());
		
	//	EnhancedDenseMatrixT<float>*
		//double simValue= computeSimilarity(*hostilityMat, row1,row2,actors.size());
		// Note Similarity is defined using Influencia Matrix Until Further Notice
			simValue = (*influence_matrix)(rc,cc);
		//	simValue = (*hostilityMat)(rc, cc);
			if(rc == cc)
            (temp_Actor_Sims)->insert(std::make_pair(std::string((*act_iter)->name()),1));
			if (simValue == 0)
            (temp_Actor_Sims)->insert(std::make_pair(std::string((*act_iter)->name()),0.1));
			(temp_Actor_Sims)->insert(std::make_pair(std::string((*act_iter)->name()),simValue+0.1));
			cc++;

	}
	rc++;
//	std::cout<<temp_Actor_Sims->size()<<"=======+++++"<<(*act_iter2)->name()<<std::endl;
	similarities.insert(std::make_pair(std::string((*act_iter2)->name()), (temp_Actor_Sims)));
	}

	std::cerr<<"--------end similarity--------------"<<std::endl;  

}


void simulation_engine::get_actor_resources_matrix(std::vector<actor_resource*> &actor_resources, std::vector<actor*> &actors, std::vector<std::string> &resource_types, EnhancedDenseMatrixT<float>* actor_resource_Mat){
	actor_resource_Mat->zeros(actors.size()+1, resource_types.size()+1);
	int rows = 0;
	for(std::vector<actor*>::const_iterator act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
		actor_resource_Mat->rowname(rows, std::string((*act_iter)->name()));
		rows++;
	}
	int cols =0;
	for(std::vector<std::string>::const_iterator rt_iter = resource_types.begin(); rt_iter != resource_types.end(); rt_iter++){
		actor_resource_Mat->colname(cols, (*rt_iter));
		cols++;
	}

	for(std::vector<actor_resource*>::iterator ar_iter = actor_resources.begin(); ar_iter != actor_resources.end(); ar_iter++){
		std::string actor_name = find_actor_name(actors, (*ar_iter)->actor_id());
		std::string resource_name = (*ar_iter)->resource_type();
        int row_index = actor_resource_Mat->rowIndex(actor_name);
		int col_index = actor_resource_Mat->colIndex(resource_name);
		if ((row_index != -1) && (col_index != -1))
        actor_resource_Mat->set(row_index, col_index, (*ar_iter)->level());
	}
   // actor_resource_Mat->dump();
	std::cout<<"End Actor Resources Mat"<<std::endl;
	
}


void simulation_engine::select_actions(int period, EnhancedDenseMatrixT<float>* &hostilityMat, std::vector<action*> actions, std::vector<std::string> &resource_types, std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &attractiveness_actions_Rank ,std::vector<actor*> &actors,EnhancedDenseMatrixT<float>* action_resource_required_matrix, std::map<std::string, std::vector<std::string> > &performedActions, EnhancedDenseMatrixT<float>* actor_resources_matrix,std::map<std::string, agent_measures*>* &actor_measures, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_type_x_resource_type_x_actor){
    std::vector<std::string>* tempAction;
	std::vector<std::string> theTokens;
	std::string tempActionText;
	int hostilityFlag=0;
	for(std::vector<actor*>::iterator ac_iter=actors.begin(); ac_iter != actors.end(); ac_iter++){
		// Finds Ranks for Actor
		std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >::iterator actionTaken = attractiveness_actions_Rank.find((*ac_iter)->name());
		std::map<std::string,std::vector<std::string> >::iterator performedIter=performedActions.find((*ac_iter)->name());
		std::multimap<float,std::string, std::greater<float> >::iterator actionIter = actionTaken->second->begin();
		int medida = actionTaken->second->size();
        int action_found = 0;
		//std::vector<int>  actionCandidates;
		float number;
		int c = 0;
		//for (int c =0; c < 3; c++){
		while ( (c < 3) && (!action_found)){
			actionIter = actionTaken->second->begin();
		  number = randGenerator->getUniformRand(0.0f,1.0f);
		  int number2 =(int)( number * 10);
			// actionCandidates.push_back(number2);
			//}
			//	std::sort(actionCandidates.begin(), actionCandidates.end());
			int k = 0;
			while (k < number2){
				actionIter++;
				k++;
			}

			hostilityFlag=0;
		

	//	while ((action_found==0) && (actionIter != actionTaken->second->end())){
	//	c = 0;
	
			tempActionText=(actionIter->second);
			tempAction = &(performedIter->second);
			tempAction->push_back(tempActionText);
			int pos = tempActionText.find(";");
			std::string aType = tempActionText.substr(0,pos);
			//aType ACtion Type
			theTokens.clear();
			Tokenize(tempActionText, theTokens,";");
//			std::cout<<aType<<std::endl;
            int actionNumber = atoi(theTokens[1].c_str());
			std::string theSender = (*ac_iter)->name();
			std::string theReceiver = theTokens[2];
			std::string theDirection = theTokens[3];

		//	std::vector<actor*>::iterator rec_iter=actors.find(theReceiver);
			std::string actionDirection="";
			int rowActorS  = hostilityMat->rowIndex((*ac_iter)->name());
			int colActorR  = hostilityMat->colIndex(theReceiver);
		//	hostilityMat->dump();
			double hostilityVal = (*hostilityMat)(rowActorS, colActorR);
			for(std::vector<action*>::iterator itAction = actions.begin(); itAction != actions.end(); itAction++){
				if( ((*itAction)->id() == actionNumber)){
					actionDirection = (*itAction)->direction();
					break;
				}
			}

			if ((actionDirection == "hostile") && (hostilityVal <0 )){
                   hostilityFlag = 1;
			}
			if ((actionDirection == "friendly") && (hostilityVal >0 )){
                   hostilityFlag = 1;
			}

			if(( theSender != theReceiver) && (hostilityFlag == 1)){

			//	action_resource_required_matrix->dump();
				int rowAction = action_resource_required_matrix->rowIndex(aType);
				int rowActor  = actor_resources_matrix->rowIndex((*ac_iter)->name());
			//std::map<std::string, double> resourcesAction = obtain_resources_action(aType, action_resource_requireds );
			//std::map<std::string, double> resourcesActor = obtain_resources_actor((*ac_iter)->name(), actors, actor_resources );
				int resources_available = 0;
				std::map<std::string, float> *actionResources = new std::map<std::string, float>;
				for (std::vector<std::string>::iterator riter = resource_types.begin(); riter != resource_types.end(); riter++){
					int colAction= action_resource_required_matrix->colIndex((*riter));
					int colActor = actor_resources_matrix->colIndex((*riter));
				//	actionResources->insert(std::make_pair((*riter),(*action_resource_required_matrix)(rowAction, colAction) ));
					if ((*actor_resources_matrix)(rowActor, colActor) < (*action_resource_required_matrix)(rowAction, colAction))
						resources_available = 1;
				}
		
				if (resources_available ==0){
				//	std::cout<<"Recursos Suficientes "<<std::endl;
			
					action_found = 1;
					std::map<std::string, float> *actionResources = new std::map<std::string, float>;
					for (std::vector<std::string>::iterator riter = resource_types.begin(); riter != resource_types.end(); riter++){
						int colAction= action_resource_required_matrix->colIndex((*riter));
						int colActor = actor_resources_matrix->colIndex((*riter));
						float tempAVal = (*action_resource_required_matrix)(rowAction, colAction);
						if (tempAVal > 0)
						actionResources->insert(std::make_pair((*riter), tempAVal ));
					}
					std::map<std::string, float> *receiverResources = new std::map<std::string, float>;
					std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator itResActor =action_type_x_resource_type_x_actor.find(theReceiver);
					EnhancedDenseMatrixT<float>* resMatrix = (itResActor)->second;
					int rowAction = resMatrix->rowIndex(aType);
					for (riter = resource_types.begin(); riter != resource_types.end(); riter++){
						int xAction= resMatrix->colIndex((*riter));
					
						receiverResources->insert(std::make_pair((*riter),(*resMatrix)(rowAction, xAction) ));
					}
					int time_duration = (*itAction)->duration_peak();
					double theLevel = (*itAction)->level();
					simulation_events *sendEvent;
					simulation_events *recEvent;
					sendEvent = new simulation_events(period,period+time_duration-1,(*ac_iter)->name(), theReceiver, theDirection, *actionResources, (*itAction)->name(), aType, theLevel);
					recEvent = new simulation_events(period,period+time_duration-1,(*ac_iter)->name(), theReceiver, theDirection, *receiverResources,(*itAction)->name(), aType, theLevel);
					std::map<std::string, agent_measures*>::iterator itSender =actor_measures->find((*ac_iter)->name());
					std::map<std::string, agent_measures*>::iterator itReceiver =actor_measures->find(theReceiver);

					(*itReceiver).second->add_received_event(recEvent);
					(*itSender).second->add_sended_event(sendEvent);

               

               // itSender->second->setSended(*tempSended);
			//	itReceiver->second->setReceived(*tempReceived);
			//	agentMeasures
					performedActions.insert(std::make_pair((*ac_iter)->name(), *tempAction));
				}

			}
		/*	if (!action_found){
				actionIter++;
				
			} */

			c++;
		//}
		} /* End C */
		if (!action_found){
			tempActionText = "none";
			performedActions.insert(std::make_pair((*ac_iter)->name(), *tempAction));
		}
		

	} //End actors Loop
	
}

void simulation_engine::compute_initial_actor_resources_matrix(std::vector<actor_resource*> &actor_resources, std::vector<actor*> &actors, std::vector<std::string> &resource_types, EnhancedDenseMatrixT<float>* initial_actor_x_resources){
	initial_actor_x_resources->zeros(actors.size()+1, resource_types.size()+1);
	int rows=0;
	for(std::vector<actor*>::iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
		initial_actor_x_resources->rowname(rows, std::string((*act_iter2)->name()));
		 rows++;
	}
	int cols = 0;
    for(std::vector<std::string>::const_iterator r_iter = resource_types.begin(); r_iter != resource_types.end(); ++r_iter){
		initial_actor_x_resources->colname(cols, (*r_iter));
		cols++;
	}
	for(std::vector<actor_resource*>::iterator ar_iter=actor_resources.begin(); ar_iter != actor_resources.end(); ar_iter++){
		int the_act_id = (*ar_iter)->actor_id();
		std::string act_txt = find_actor_name(actors, the_act_id);
		int row = initial_actor_x_resources->rowIndex(act_txt);
		int col = initial_actor_x_resources->colIndex((*ar_iter)->resource_type());
        initial_actor_x_resources->set(row,col,(*ar_iter)->level());
        

	}
//	initial_actor_x_resources->dump();
}


void simulation_engine::compute_growth_actor_resources_matrix(std::vector<actor_resource*> &actor_resources, std::vector<actor*> &actors, std::vector<std::string> &resource_types, EnhancedDenseMatrixT<float>* growth_actor_x_resources){
	growth_actor_x_resources->zeros(actors.size()+1, resource_types.size()+1);
	int rows=0;
	for(std::vector<actor*>::iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
		growth_actor_x_resources->rowname(rows, std::string((*act_iter2)->name()));
		 rows++;
	}
	int cols = 0;
    for(std::vector<std::string>::const_iterator r_iter = resource_types.begin(); r_iter != resource_types.end(); ++r_iter){
		growth_actor_x_resources->colname(cols, (*r_iter));
		cols++;
	}
	for(std::vector<actor_resource*>::iterator ar_iter=actor_resources.begin(); ar_iter != actor_resources.end(); ar_iter++){
		int the_act_id = (*ar_iter)->actor_id();
		std::string act_txt = find_actor_name(actors, the_act_id);
		int row = growth_actor_x_resources->rowIndex(act_txt);
		int col = growth_actor_x_resources->colIndex((*ar_iter)->resource_type());
        growth_actor_x_resources->set(row,col,(*ar_iter)->resource_growth());
        

	}
//	growth_actor_x_resources->dump();
}

void simulation_engine::compute_actionType_tendencies_cube(std::vector<std::string> &action_types, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &tendencyMat, EnhancedDenseMatrixT<float>* &hostilityMat, std::map<std::string, EnhancedDenseMatrixT<float>*> &tendenciesCube){
	EnhancedDenseMatrixT<float>* tempActorActorMatrix;

	int at_count_t = 0;
      for(std::vector<std::string>::const_iterator at_iter1 = action_types.begin(); at_iter1 != action_types.end(); ++at_iter1){
		  int act_count_t = 0;
		  tempActorActorMatrix = new EnhancedDenseMatrixT<float>(actors.size()+1,actors.size()+1);
		  for(std::vector<actor*>::const_iterator act_iter5 = actors.begin(); act_iter5 != actors.end(); ++act_iter5){
		    float value_actor_at2 = (*tendencyMat)(act_count_t,at_count_t);
			int act_count2_t = 0;
			int thecount=0;
			for(std::vector<actor*>::const_iterator act_iter44 = actors.begin(); act_iter44 != actors.end(); ++act_iter44){
				tempActorActorMatrix->rowname(thecount,(*act_iter44)->name());
                tempActorActorMatrix->colname(thecount,(*act_iter44)->name());
				thecount++;
			}
			for(std::vector<actor*>::const_iterator act_iter4 = actors.begin(); act_iter4 != actors.end(); ++act_iter4){
			   //	tempActorActorMatrix->set(act_count_t,act_count2_t, value_actor_at2 * (*hostilityMat)(act_count_t, act_count2_t));
				tempActorActorMatrix->set(act_count_t,act_count2_t, value_actor_at2 * (*hostilityMat)(act_count_t, act_count2_t));
			    ++act_count2_t;
			} 
            
			//tempActorActorMatrix->dump();
			++act_count_t;
		  }
         // std::cout<<(*at_iter1)<<std::endl;
        // tempActorActorMatrix->dump();
			(tendenciesCube).insert(std::make_pair(std::string((*at_iter1)),tempActorActorMatrix));
		  ++at_count_t;
	  }
	std::cerr<<"--------end tendency--------------"<<std::endl; 

}

void simulation_engine::get_weights_matrices_influence_weights(std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &influence_matrix, EnhancedDenseMatrixT<float>* &weights_matrix, EnhancedDenseMatrixT<float>* &I_weights_matrix, EnhancedDenseMatrixT<float>* &InfluenceWeigthtsMat){
	weights_matrix->zeros(actors.size()+1, actors.size()+1);
	I_weights_matrix->zeros(actors.size()+1, actors.size()+1);
    InfluenceWeigthtsMat->zeros(actors.size()+1, actors.size()+1);
	int infex = 0;
   // for(std::vector<action*>::const_iterator giter = goals.begin(); giter != goals.end(); ++giter){
      for(std::vector<actor*>::const_iterator act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
		weights_matrix->rowname(infex, (*act_iter)->name());
		weights_matrix->colname(infex, (*act_iter)->name());
        I_weights_matrix->rowname(infex, (*act_iter)->name());
		I_weights_matrix->colname(infex, (*act_iter)->name());
		InfluenceWeigthtsMat->rowname(infex, (*act_iter)->name());
		InfluenceWeigthtsMat->colname(infex, (*act_iter)->name());
      ++infex;
	  };

	  infex = 0;
	  for(act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
		int infexc = 0;
		for(std::vector<actor*>::const_iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){     
			if (infex == infexc){
				weights_matrix->set(infex, infexc, 1-(*influence_matrix)(infex,infexc));
                I_weights_matrix->set(infex, infexc, (*influence_matrix)(infex,infexc));
			}
            ++infexc;
        }
		++infex;
      }
	  
  //    influence_matrix->dump();
//	  weights_matrix->dump();
	// I_weights_matrix->dump();
	  matrix::multiply(*(weights_matrix),*(influence_matrix),*InfluenceWeigthtsMat);
 //     InfluenceWeigthtsMat->dump();
	std::cerr<<"--------end influences--------------"<<std::endl; 

  }


void simulation_engine::compute_beliefs(std::map<std::string, simContainer* > &similarities, std::vector<actor*> &actors, std::vector<std::string> &action_types, intellContainer &intelligences, std::map<std::string, EnhancedDenseMatrixT<float>*> &tendenciesCube, std::map<std::string, matrixContainer > &bel_Cube){

	std::map<std::string, EnhancedDenseMatrixT<float>*> *temp_Matrix_Map;
    EnhancedDenseMatrixT<float>* temp_Matrix;
	double intel=0;
	double similarity=0;
	std::map<std::string, std::map<std::string,float> * >::iterator sim_curr;
	std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator it_curr_tend;
	for(std::vector<actor*>::const_iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){

		temp_Matrix_Map= new std::map<std::string, EnhancedDenseMatrixT<float>*>;

		intellContainer::iterator intell_cur= intelligences.find((*act_iter20)->name());
		
        intel = intell_cur->second;
		for(std::vector<std::string>::iterator at_iter1 = action_types.begin(); at_iter1 != action_types.end(); ++at_iter1){
            temp_Matrix = new EnhancedDenseMatrixT<float>(actors.size()+1,actors.size()+1);
			it_curr_tend = tendenciesCube.find((*at_iter1));
		   // EnhancedDenseMatrixT<float>* matrix_act_act = tendenciesCube.find((*at_iter1));
			int row_sim =0;
			for(std::vector<actor*>::iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
				
		//	std::map<std::string, std::map<std::string, double>* >::iterator sim_curr=similarities.find((*act_iter2)->name());
               // intelligences.insert(std::make_pair((*aiter)->name(),(*aiter)->intell()));				
              // double similarity = sim_curr->second;				
			//	std::cout<<"intell "<<intel<<std::endl;
				sim_curr=similarities.find((*act_iter20)->name());
				std::map<std::string, float>* tempMap = (sim_curr->second);
                std::map<std::string, float>:: iterator curr_actor = tempMap->find((*act_iter2)->name());
				similarity = curr_actor->second;
			//	similarity=0;
			 //   std::map<std::string, double> tempMap = (std::map<std::string, double>)*(sim_curr->second);
		//	for(std::map<std::string, double>::iterator curr_actor=sim_curr->secondbegin(); curr_actor != tempMap.end();++curr_actor){
				int col_sim = 0;
				for(std::vector<actor*>::iterator a_itera=actors.begin(); a_itera!=actors.end(); a_itera++){
                    
				//	similarity = fabs((double)similarity);
					std::string test= curr_actor->first;
				//	std::cout<<"similarity "<<similarity<<std::endl;
			//		if (similarity != 0)
			//	std::cout<<"intell "<<intel<<std::endl;
				//	int valory = it_curr_tend->second->colIndex((curr_actor->first));
				//	int valorx = it_curr_tend->second->rowIndex((*act_iter2)->name());
				int	valorx = row_sim;
				int	valory = col_sim;
				//	std::cout<<similarity<<" x "<<valorx<<" y"<<valory<<std::endl;
                  // temp_Matrix->set(valorx,valory,(*it_curr_tend->second)(valorx,valory) * (float)intel * (float)similarity);
				   double borra1 = ((*it_curr_tend->second)(valorx,valory));
				    double borra = borra1 * (float)intel;
					if (borra != 0){
						borra = borra * similarity;
//                      std::cout<<borra<<" x "<<valorx<<" y"<<valory<<std::endl;
					  
					}
					temp_Matrix->set(valorx,valory,borra);
				//	if (((*it_curr_tend->second)(valorx,valory) * intel * similarity) > 0)
                  //  std::cout<<(*it_curr_tend->second)(valorx,valory)* intel * similarity<<std::endl;
					col_sim++;
				}
		//	(*temp_Actor_Sims).insert(std::make_pair(std::string((*act_iter)->name()),simValue));
				row_sim++;
            }
			temp_Matrix_Map->insert(std::make_pair((*at_iter1), temp_Matrix));
	//		std::cout<<(*at_iter1)<<std::endl;
//            std::cout<<intel<<std::endl;
	//		temp_Matrix->dump();
     //      it_curr_tend->second->dump();
		//	std::cout<<intel<<std::endl;
           // std::cout<<similarity<<std::endl;
//			std::cout<<(*at_iter1)<<std::endl;
        }
        
		bel_Cube.insert(std::make_pair((*act_iter20)->name(),*temp_Matrix_Map));
        temp_Matrix_Map= NULL;
	//	bel_Cube_time1.insert(std::make_pair((*act_iter20)->name(),*temp_Matrix_Map));
	}
}

void simulation_engine::compute_sum_beliefs_weights_Iweights(EnhancedDenseMatrixT<float>* &weights_matrix, EnhancedDenseMatrixT<float>* &I_weights_matrix, EnhancedDenseMatrixT<float>* &influence_matrix, std::vector<actor*> actors, std::vector<std::string> action_types, std::map<std::string, matrixContainer > &bel_Cube, std::map<std::string, matrixContainer > &bel_Cube_time1,std::map<std::string, matrixContainer >   &bel_Cube_Aux){
std::map<std::string, EnhancedDenseMatrixT<float>*> temp_Matrix_Map_B;
   std::map<std::string, EnhancedDenseMatrixT<float>*> temp_Matrix_Map_B2;
    EnhancedDenseMatrixT<float>* temp_Matrix_B;
    EnhancedDenseMatrixT<float>* temp_Matrix_B2;
    EnhancedDenseMatrixT<float>* sus_influence;
  
    EnhancedDenseMatrixT<float>* sum_Matrix_B;
	for(std::vector<actor*>::iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
	//	temp_Matrix_Map_B= new std::map<std::string, EnhancedDenseMatrixT<float>*>;
        std::map<std::string, matrixContainer >::iterator it_beliefs= bel_Cube.find((*act_iter20)->name());
        std::map<std::string, matrixContainer >::iterator it_beliefs2= bel_Cube_time1.find((*act_iter20)->name());
        temp_Matrix_Map_B = (it_beliefs->second);
        temp_Matrix_Map_B2 = (it_beliefs2->second);
		


	//	temp_Matrix_Map_B2
		for(std::vector<std::string>::iterator at_iter1 = action_types.begin(); at_iter1 != action_types.end(); ++at_iter1){

            temp_Matrix_B = new EnhancedDenseMatrixT<float>(actors.size()+1,actors.size()+1);
            sus_influence = new EnhancedDenseMatrixT<float>(actors.size()+1,actors.size()+1);
    
            temp_Matrix_B2 = new EnhancedDenseMatrixT<float>(actors.size()+1,actors.size()+1);
            sum_Matrix_B   = new EnhancedDenseMatrixT<float>(actors.size()+1,actors.size()+1);
            temp_Matrix_B->zeros(actors.size()+1 ,actors.size()+1);
            temp_Matrix_B2->zeros(actors.size()+1, actors.size()+1);
             
            matrix::multiply(*(weights_matrix), *(influence_matrix), *(sus_influence)); 
			std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator it_curr_bel = temp_Matrix_Map_B.find((*at_iter1));
            std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator it_curr_bel2 = temp_Matrix_Map_B2.find((*at_iter1));

			matrix::multiply(*(sus_influence), *(it_curr_bel->second), *(temp_Matrix_B));
			matrix::multiply(*(it_curr_bel2->second),*I_weights_matrix,*temp_Matrix_B2);
            for(int c_row = 0; c_row < temp_Matrix_B->rows(); ++c_row){
				for(int c_col = 0; c_col < temp_Matrix_B->cols(); ++c_col){
					sum_Matrix_B->set(c_row, c_col, (*temp_Matrix_B)(c_row,c_col) + (*temp_Matrix_B2)(c_row,c_col));
				}
			}

           // matrix::multiply(*(I_weights_matrix), *(it_curr_bel2->second), *(temp_Matrix_B2));
		//	(it_curr_bel2->second)->dump();
          //  I_weights_matrix->dump();
		//	sum_Matrix_B->dump();

		//	std::cout<<"rows  "<<temp_Matrix_B2->rows()<<std::endl;
            
		//	std::cout<<"cols  "<<temp_Matrix_B2->cols()<<std::endl;

            
		//	matrix::sum(*temp_Matrix_B,*temp_Matrix_B2,*sum_Matrix_B);
            temp_Matrix_Map_B.insert(std::make_pair((*at_iter1),sum_Matrix_B));
            temp_Matrix_Map_B2.insert(std::make_pair((*at_iter1),temp_Matrix_B2));
		//	std::cout<<"influence"<<std::endl;
         //   sus_influence->dump();
		//	std::cout<<"belief"<<std::endl;
       //     (it_curr_bel->second)->dump();
        //    std::cout<<"result"<<std::endl;
	//		temp_Matrix_B->dump();
	//		temp_Matrix_B2->dump();
	//		 sum_Matrix_B->dump();
	//		std::cout<< "hola  "<<std::endl;
			
		   // EnhancedDenseMatrixT<float>* matrix_act_act = tendenciesCube.find((*at_iter1));
			
			//temp_Matrix_Map->insert(std::make_pair((*at_iter1), temp_Matrix));
        }
        bel_Cube_Aux.insert(std::make_pair((*act_iter20)->name(),temp_Matrix_Map_B));
//		bel_Cube_time1_Aux.insert(std::make_pair((*act_iter20)->name(),temp_Matrix_Map_B2));
	//	bel_Cube.insert(std::make_pair((*act_iter20)->name(),*temp_Matrix_Map));
	//	bel_Cube_time1.insert(std::make_pair((*act_iter20)->name(),*temp_Matrix_Map));
	}
}

void simulation_engine::compute_probability_actions_actor(std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, matrixContainer > &bel_Cube_Aux, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_x_actor_x_actor,std::map<std::string, matrixContainer> &actorActionProbabilities){
 std::map<std::string, EnhancedDenseMatrixT<float>*> temp_Matrix_Map_B;
   std::map<std::string, EnhancedDenseMatrixT<float>*> temp_Matrix_Map_B2;
   std::map<std::string, EnhancedDenseMatrixT<float>*>* temp_Matrix_Map;
   EnhancedDenseMatrixT<float>* temp_Matrix_B;
   
   
   EnhancedDenseMatrixT<float>* temp_ActorActorAction;
   EnhancedDenseMatrixT<float>* sumProbabilities;
   for(std::vector<actor*>::iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
	std::map<std::string, matrixContainer >::iterator it_beliefs= bel_Cube_Aux.find((*act_iter20)->name());
    temp_Matrix_Map_B = (it_beliefs->second);
	temp_Matrix_Map= new std::map<std::string, EnhancedDenseMatrixT<float>*>;
   //  actorActionProbabilities = new std::map<std::string, matrixContainer >;
	for(std::vector<std::string>::iterator at_iter1 = action_types.begin(); at_iter1 != action_types.end(); ++at_iter1){
	   //std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator curr_bel = bel_Cube_Aux.find((*at_iter1));
       std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator curr_aaa = action_x_actor_x_actor.find((*at_iter1));
       std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator curr_mb = temp_Matrix_Map_B.find((*at_iter1));

	   temp_Matrix_B = curr_mb->second;
	   temp_ActorActorAction = curr_aaa->second;
	                           
	   sumProbabilities = new EnhancedDenseMatrixT<float>(actors.size()+1,actors.size()+1);
	  // matrix::multiply((*temp_ActorActorAction), (*temp_Matrix_B), *sumProbabilities);
	  
	   for(int c_row = 0; c_row < temp_Matrix_B->rows(); ++c_row){
				for(int c_col = 0; c_col < temp_Matrix_B->cols(); ++c_col){
			sumProbabilities->set(c_row, c_col, fabs((*temp_ActorActorAction)(c_row,c_col) * (*temp_Matrix_B)(c_row,c_col)));
				
				}
			}
       
     
       for(c_row = 0; c_row < temp_Matrix_B->rows(); ++c_row){
		        double suma = 0;
		        for(int c_col = 0; c_col < temp_Matrix_B->cols(); ++c_col){
					suma = suma + (*sumProbabilities)(c_row,c_col);
				}
				 for( c_col = 0; c_col < temp_Matrix_B->cols(); ++c_col){
					if (suma != 0){
					//	std::cout<<"  "<<(*sumProbabilities)(c_row,c_col) <<" ++ "<< ((*sumProbabilities)(c_row,c_col))/suma <<" "<<suma<<std::endl;
			sumProbabilities->set(c_row, c_col, ((*sumProbabilities)(c_row,c_col))/suma);
					}
				
				} 
			} 
			
			
	 //  std::cout<<(*at_iter1)<<std::endl;
      // temp_ActorActorAction->dump();
     //  temp_Matrix_B->dump();
	//  sumProbabilities->dump();
	   temp_Matrix_Map->insert(std::make_pair((*at_iter1),sumProbabilities));

	}
    actorActionProbabilities.insert(std::make_pair((*act_iter20)->name(),*temp_Matrix_Map));
   }
}

void simulation_engine::sort_actions_actor(std::map<std::string, matrixContainer> actorActionProbabilities, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, std::multimap<float, std::string, std::greater<float> >* >  &actionRanks){
    int row=0;
  //  std::map<std::string, float>* actorRanks;
	std::multimap<float,std::string, std::greater<float> >* theRanks;
//	std::vector<std::string> theRanks;
	for(std::vector<actor*>::iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
	  // fout<<"actor Name "<<(*act_iter20)->name()<<std::endl;
	  // actorRanks = new std::map<std::string, float>;
	   theRanks = new std::multimap<float, std::string, std::greater<float> >;
       std::map<std::string, matrixContainer>::iterator tempMatrixMap= actorActionProbabilities.find((*act_iter20)->name());
	   for(std::vector<std::string>::iterator at_iter = action_types.begin(); at_iter != action_types.end(); at_iter++){
		  // fout<<"===========================Action Type==="<<(*at_iter)<<std::endl;
		 //  matrixContainer::iterator tempActorActor = tempMatrixMap->find((*at_iter));
		   std::map<std::string, EnhancedDenseMatrixT<float>*> tempActorActor = (*tempMatrixMap).second;
           std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator tempMatrix = tempActorActor.find((*at_iter));
           EnhancedDenseMatrixT<float>* temp_ActorActorAction = tempMatrix->second;
		   int row=0;
           for(std::vector<actor*>::iterator act_iterS = actors.begin(); act_iterS != actors.end(); ++act_iterS){

                int col=0;
				for(std::vector<actor*>::iterator act_iterT = actors.begin(); act_iterT != actors.end(); ++act_iterT){
					if ((*temp_ActorActorAction)(row,col)!=0){
					//	fout<<(*act_iterS)->name()<<" +++++++++++++++++++++++++"<<(*act_iterT)->name()<<" "<<(*temp_ActorActorAction)(row,col)<<std::endl;
					std::string compName = (*at_iter)+";"+(*act_iterT)->name();
				//	actorRanks->insert(std::make_pair(compName, (*temp_ActorActorAction)(row,col)));
					theRanks->insert(std::make_pair((*temp_ActorActorAction)(row,col), compName));
				
					}
					col++;

				}
				row++;
		   }
	   }

//	   std::cout<<(*act_iter20)->name()<<"  lllliiijjjj "<<std::endl;
	   typedef std::multimap<float,std::string, std::greater<float> >::const_iterator mapPointer;
//	   for (mapPointer p=theRanks->begin(); p!= theRanks->end(); p++){
//		   std::cout<< p->first<<" **** "<<p->second<<std::endl;
//	   }
	   

	  // std::copy(actorRanks->begin(), actorRanks->end(), std::cout);
	   actionRanks.insert(std::make_pair((*act_iter20)->name(), theRanks));
	}



}

void simulation_engine::filter_specific_actions_to_10(std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &spec_actions_Rank, std::vector<actor*> &actors, std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &filtered_spec_actions_Rank){
	std::multimap<float,std::string, std::greater<float> >* tempMap;
    std::multimap<float,std::string, std::greater<float> >* newMap;

	for(std::vector<actor*>::iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
		std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >::iterator it_spec_actions=spec_actions_Rank.find((*act_iter20)->name());
	    newMap=new std::multimap<float,std::string, std::greater<float> >;
		tempMap = it_spec_actions->second;
        std::multimap<float,std::string, std::greater<float> >::iterator mientras= tempMap->begin();
	//	std::cout<<"Actor      ::"<<(*act_iter20)->name()<<std::endl;
		int counter =0; 
		while ((mientras != tempMap->end()) && (counter < 11)){
		//	std::cout<<mientras->first<<" ::: "<<mientras->second<<std::endl;
			int len = (mientras->second).length();
			int pos = (mientras->second).find_last_of(";");
			std::string tempo = (mientras->second).substr(pos+1,len-pos);
			if (tempo != (*act_iter20)->name()){
			newMap->insert(std::make_pair(mientras->first,mientras->second));
			counter++;
			}
			mientras++;
		}
		filtered_spec_actions_Rank.insert(std::make_pair((*act_iter20)->name(),newMap));
	}
	
}

void simulation_engine::get_specific_actions_ranks(std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >  &spec_actions_Rank, std::vector<action*> &actions, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, std::multimap< float, std::string, std::greater<float> >* >  &actionRanks, std::vector<actor_action_proportions*> &action_proportions){
	std::multimap<float,std::string, std::greater<float> >* tempActorAction ;
	std::map<int, std::map<std::string,std::map<int, float>* >* > actor_action_proportions_map;
    std::map<int, float>* specActionValue;
    std::map<std::string,std::map<int, float>* >* actionTypeActions;
	std::multimap<float,std::string, std::greater<float> >* tavo;
	double probValue;
    std::string stTest;
	for(std::vector<actor*>::iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
		actionTypeActions = new std::map<std::string,std::map<int, float>* >;
		for(std::vector<std::string>::iterator at_iter = action_types.begin(); at_iter != action_types.end(); at_iter++){
			specActionValue = new std::map<int, float>;
            actionTypeActions->insert(std::make_pair((*at_iter), specActionValue));
            
		}
        actor_action_proportions_map.insert(std::make_pair((*act_iter20)->id(),actionTypeActions));
	}
  
//	std::cout<<"proportions :"<<action_proportions.size()<<std::endl;
	for(std::vector<actor_action_proportions*>::iterator ap_iter = action_proportions.begin(); ap_iter != action_proportions.end(); ++ap_iter){
		std::map<int, std::map<std::string,std::map<int, float>* >* >::iterator p_proportion = actor_action_proportions_map.find((*ap_iter)->actor_id());
	//	std::cout<<"Inserting "<<(*ap_iter)->action_id()<<" "<<(*ap_iter)->proportion()<<std::endl;
	//	std::cout<<"ACtion Type :"<<std::string((*ap_iter)->action_type())<<" "<<(*ap_iter)->actor_id()<<std::endl;
		std::map<std::string,std::map<int, float>* >::iterator p_action_types = p_proportion->second->find((*ap_iter)->action_type());
        std::map<int, float>* temp=p_action_types->second;
	//	std::cout<<"Inserting "<<(*ap_iter)->action_id()<<" "<<(*ap_iter)->proportion()<<std::endl;
		temp->insert(std::make_pair((*ap_iter)->action_id(),(*ap_iter)->proportion()));
	}

	for(act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
		std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >::iterator rankIter = actionRanks.find((*act_iter20)->name());
	     tempActorAction = rankIter->second;
	//	 std::cout<<"Salida :"<<rankIter->first<<std::endl;
		 typedef std::multimap<float,std::string, std::greater<float> >::const_iterator mPointer;
		 mPointer p = tempActorAction->begin();
        // std::cout<<"Salida22222 :"<<p->second<<std::endl;
		 tavo = new std::multimap<float,std::string, std::greater<float> >;
		 while(p != tempActorAction->end()){
		 int yyy =(p->second).find_first_of(";");
		 probValue = p->first;
         int len = (p->second).length();
        // std::cout<<"Salida22222 :"<<(p->second).substr(0,yyy)<<" "<<probValue<<std::endl;
	//	 std::cout<<"Salida22222 :"<<(p->second).substr(yyy+1,len-yyy)<<" "<<probValue<<std::endl;
		 std::string actionType = (p->second).substr(0,yyy);
		 std::string receiver = (p->second).substr(yyy+1,len-yyy);
		 
         std::map<int, std::map<std::string,std::map<int, float>* >* >::iterator pActions=actor_action_proportions_map.find((*act_iter20)->id());
         std::map<std::string,std::map<int, float>* >::iterator mapForActor = pActions->second->find(actionType);
		// std::cout<<"el tipoe de accion "<<pActions->first<<std::endl;
        // std::cout<<"el tipo de accion ess "<<temp3->first<<std::endl;

         std::map<int, float> mapActionProp=*(mapForActor->second);
		// std::cout<<"The map size is :"<<temp5.size()<<std::endl;
         
		 
         for(std::map<int, float>::iterator itActions=mapActionProp.begin();itActions!=mapActionProp.end(); itActions++){
			 char buffer[100];
			 itoa(itActions->first,buffer,10);
			 std::string theDirection = getDirection(actions,itActions->first);
			 stTest = actionType+";"+buffer+";"+receiver+";"+theDirection;
			 tavo->insert(std::make_pair(itActions->second * probValue,stTest));
		//	 std::cout<<itActions->second<<" "<<stTest<<std::endl;
		//	 std::string borra <<(p->second).substr(0,yyy)<<";"<<itActions->first<<std::endl;
			// spec_actions_Rank.insert(make_pair);
		 }
		 p++;
		 }
         spec_actions_Rank.insert(std::make_pair((*act_iter20)->name(),tavo));
	//	 std::map<int, float>::iterator  temp2 = (temp3)->find((p->second).substr(0,yyy));
	}
}

void simulation_engine::compute_attractiveness_actions(std::vector<action*> &actions, std::vector<actor*> &actors, std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &filetered_spec_actions_Rank, std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &attractiveness_actions_Rank){
    std::multimap<float,std::string, std::greater<float> >* tempMap;
    std::multimap<float,std::string, std::greater<float> >* newMap;
	double attractiveness;
	double skill;
    for(std::vector<actor*>::iterator act_iter20 = actors.begin(); act_iter20 != actors.end(); ++act_iter20){
		
		std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >::iterator specActions=filetered_spec_actions_Rank.find((*act_iter20)->name());
		tempMap = specActions->second;
		newMap = new std::multimap<float,std::string, std::greater<float> >;
		std::multimap<float,std::string, std::greater<float> >::iterator mientras= tempMap->begin();
        while (mientras != tempMap->end()){
//          std::cout<<mientras->first<<" ::: "<<mientras->second<<std::endl;
		  int initial = (mientras->second).find_first_of(";");
          int end     = (mientras->second).find_last_of(";")-1;
		  int actionNumber = atoi(((mientras->second).substr(initial+1,end-initial)).c_str());
		  action* theActionRecord = find_action(actions, actionNumber);
		  if (theActionRecord != NULL){
			skill = (*act_iter20)->skill();
            attractiveness = theActionRecord->economic_impact()+theActionRecord->population_impact();
			attractiveness = (theActionRecord->symbolic_impact()+theActionRecord->physical_impact()+ attractiveness)/4;
			attractiveness = attractiveness -(1-skill)*((theActionRecord->physical_difficulty()+ theActionRecord->planning_time()+theActionRecord->resources_needed())/3);
		//	std::cout<<"Found  :"<<theActionRecord->name()<<"  "<<attractiveness<<std::endl;
			std::string textAction = mientras->second+";"+theActionRecord->direction();
			newMap->insert(std::make_pair(attractiveness,textAction));

            
		  }
		  mientras++;
		}
        attractiveness_actions_Rank.insert(std::make_pair((*act_iter20)->name(),newMap));
	}
}

void simulation_engine::execute_actions(int timePeriod, std::map<std::string, EnhancedDenseMatrixT<float>*> &goal_x_resource_type_x_actor, EnhancedDenseMatrixT<float>* &hostilityMat, EnhancedDenseMatrixT<float>* influence_matrix, std::map<std::string, EnhancedDenseMatrixT<float>*>  &actor_resources_receiver, std::vector<std::string> resource_types, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* initial_actor_x_resources, std::map<std::string, agent_measures*>* &actor_measures){
	    EnhancedDenseMatrixT<float>* hist_actor_effect;
        EnhancedDenseMatrixT<float>* hist_effect_actor;;
//        EnhancedDenseMatrixT<float>* influence;
//	    EnhancedDenseMatrixT<float>* hostilityDeltaMat;
		EnhancedDenseMatrixT<float>*  actor_effects_actor;
        EnhancedDenseMatrixT<float>*  delta_hostility;
		double hostVal;
		double sumInfl;
		EnhancedDenseMatrixT<float>*  goal_resource;
		EnhancedDenseMatrixT<float>*  resource_goal;
		//EnhancedDenseMatrixT<float>*  delta_hostility;


	   hist_actor_effect = new 	EnhancedDenseMatrixT<float>(actors.size()+1, resource_types.size()+1);
       hist_effect_actor = new 	EnhancedDenseMatrixT<float>( resource_types.size()+1, actors.size()+1);
	   actor_effects_actor = new 	EnhancedDenseMatrixT<float>(actors.size()+1, actors.size()+1);
       delta_hostility = new 	EnhancedDenseMatrixT<float>(actors.size()+1, actors.size()+1);
       hist_actor_effect->zeros(actors.size()+1, resource_types.size()+1);
	   addMatrixActorNames(hist_actor_effect, actors, 1);
       addMatrixResourceNames(hist_actor_effect, resource_types, 0);
	for(std::vector<actor*>::iterator a_iter=actors.begin(); a_iter != actors.end(); a_iter++){
		std::map<std::string,agent_measures*>::iterator itMeasures = actor_measures->find((*a_iter)->name());
        std::vector<simulation_events*> sEvents = (*itMeasures->second).getSendedEvents();
	//	std::cout<<"valor  "<<(*a_iter)->name()<<" ****** "<<sEvents.size()<<std::endl;
        std::vector<simulation_events*> rEvents = (*itMeasures->second).getReceivedEvents();
//		std::cout<<"Received  "<<(*a_iter)->name()<<" ****** "<<rEvents.size()<<std::endl;
		int rowActor = initial_actor_x_resources->rowIndex((*a_iter)->name());
		
		//Update Sender Resources
		for (std::vector<simulation_events*>::iterator itEvents= sEvents.begin(); itEvents != sEvents.end(); itEvents++){
            int timeStart = (*itEvents)->getStartTime();
			if (timeStart == timePeriod){
		//		std::cout<<"el Valor por ahora es "<<rowActor<<std::endl;
				std::map<std::string, float> itRes = (*itEvents)->getResources();
				for(std::map<std::string, float>::iterator iR=itRes.begin(); iR != itRes.end(); iR++){
					std::string rType = (*iR).first;
					int colResource = initial_actor_x_resources->colIndex(rType);
					std::string rDirection = (*itEvents)->getDirection();
					if (rDirection== "hostile"){
						(*itMeasures->second).increaseHostileSent();
                        (*itMeasures->second).increaseActionsTaken();
					}
					if (rDirection== "friendly"){
						(*itMeasures->second).increaseFriendlySent();
                        (*itMeasures->second).increaseActionsTaken();
					}
					if (rDirection== "neutral"){
						(*itMeasures->second).increaseNeutralSent();
                        (*itMeasures->second).increaseActionsTaken();
					}
				//	if (rDirection== "hostile"){
						(initial_actor_x_resources)->set(rowActor, colResource,(*initial_actor_x_resources)(rowActor, colResource)-(*iR).second);
				//	} else{
                //
					//		(initial_actor_x_resources)->set(rowActor, colResource,(*initial_actor_x_resources)(rowActor, colResource)-(*iR).second);
				//	}
				}
			}
		}

		// Update Receiver Resources
        // Once the action ends
	   for (itEvents= rEvents.begin(); itEvents != rEvents.end(); itEvents++){
            int timeEnd = (*itEvents)->getEndTime();
			if (timeEnd == timePeriod){
//				std::cout<<"el Valor por ahora es "<<rowActor<<std::endl;
				std::map<std::string, float> itRes = (*itEvents)->getResources();
				for(std::map<std::string, float>::iterator iR=itRes.begin(); iR != itRes.end(); iR++){
					std::string rType = (*iR).first;
					int colResource = initial_actor_x_resources->colIndex(rType);
					std::string sDirection = (*itEvents)->getDirection();
					std::string aSender = (*itEvents)->getSender();
                    int actCol = hostilityMat->rowIndex(aSender);
					if (sDirection== "hostile"){
						(*itMeasures->second).increaseHostileReceived();
                        (*itMeasures->second).increaseActionsReceived();
					} else if (sDirection== "friendly"){
						(*itMeasures->second).increaseFriendlyReceived();
                        (*itMeasures->second).increaseActionsReceived();
					} else if (sDirection== "neutral"){
						(*itMeasures->second).increaseNeutralReceived();
                        (*itMeasures->second).increaseActionsReceived();
					}
					if (sDirection == "hostile"){
						hostilityMat->set(rowActor, actCol, (*hostilityMat)(rowActor, actCol)-.05);
					}else
						hostilityMat->set(rowActor, actCol, (*hostilityMat)(rowActor, actCol)-.05);
					if ((*hostilityMat)(rowActor, actCol) < -1)
						hostilityMat->set(rowActor, actCol, -1);
					if ((*hostilityMat)(rowActor, actCol) > 1)
						hostilityMat->set(rowActor, actCol, 1);

				//	if (sDirection== "hostile"){
					(initial_actor_x_resources)->set(rowActor, colResource,(*initial_actor_x_resources)(rowActor, colResource)+(*iR).second);
				//	} else {
				//	(initial_actor_x_resources)->set(rowActor, colResource,(*initial_actor_x_resources)(rowActor, colResource)-(*iR).second);
				//	}
				}
			}
		}

	   //All This is for Updating Hostility  
	   
        //std::vector<simulation_events*> rEvents 
	    get_history_actor_effect_matrix(hist_actor_effect, rEvents, timePeriod);
	   //addColResourceNames(hist_actor_effect, resource_types);
       hist_effect_actor->zeros(resource_types.size()+1, actors.size()+1);
	   matrix::transpose(*hist_actor_effect, *hist_effect_actor); 
	   actor_effects_actor->zeros(actors.size()+1, actors.size()+1);
	   std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator it_matrix =actor_resources_receiver.find((*a_iter)->name());
	   matrix::multiply(*((*it_matrix).second), *hist_effect_actor, *actor_effects_actor);

       // Makes zeros the diagonal of strengths or actor-effects-actors
	   for(int i = 0; i < actor_effects_actor->rows(); i++){
		   for (int j = 0; j < actor_effects_actor->cols(); j++){
			   if (i == j)
				actor_effects_actor->set(i, j, 0);
		   }
	   }
     //  hostilityMat->dump();
	   matrix::multiply(*influence_matrix, *actor_effects_actor, *delta_hostility);
	   rowActor = hostilityMat->rowIndex(std::string((*a_iter)->name()));
	 //  for (i = 0; i < hostilityMat->rows(); i++){
       for (int j=0; j < hostilityMat->cols(); j++){
		   double hostVal = ((*delta_hostility)(i,j))/3;
		   hostVal = hostVal +(*hostilityMat)(rowActor, j);
		   if (hostVal > 1) 
			   hostVal = 1;
		   if (hostVal < -1)
			   hostVal = -1;
		   if( j == i)
			   hostVal = 0;
		   hostilityMat->set(rowActor,j,hostVal);
	   }
	  // }

	  // compute
    //   delta_hostility->dump();
	//   hostilityMat->dump();
        
     //  std::cout<<"Hola "<<std::endl;

     //Ends Update Hostility [influence] [actor x effect] x [effect x actor] = [deltaHMat]
	 //Starts Update Influence 
	  std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator it_goalRes=goal_x_resource_type_x_actor.find((*a_iter)->name());
      goal_resource = (*it_goalRes).second;
	  resource_goal = new EnhancedDenseMatrixT<float>(goal_resource->cols(), goal_resource->rows());
	  EnhancedDenseMatrixT<float>* temp_actor_goal;
	  EnhancedDenseMatrixT<float>* temp_goal_actor;
	  EnhancedDenseMatrixT<float>* delta_influence;
      temp_actor_goal = new EnhancedDenseMatrixT<float>(actors.size()+1, goal_resource->rows());
	  temp_goal_actor = new EnhancedDenseMatrixT<float>( goal_resource->rows(), actors.size()+1);
      delta_influence = new EnhancedDenseMatrixT<float>( actors.size()+1, actors.size()+1);
	  matrix::transpose(*goal_resource, *resource_goal);
      matrix::multiply(*hist_actor_effect,*resource_goal,*temp_actor_goal);
      matrix::multiply(*goal_resource,*hist_effect_actor,*temp_goal_actor);
	  matrix::multiply(*temp_actor_goal, *temp_goal_actor, *delta_influence);
    //  influence_matrix->dump();
	//  delta_influence->dump();
	 // for (i = 0; i < hostilityMat->rows(); i++){
	   sumInfl = 0;
       for ( j=0; j < hostilityMat->cols(); j++){
		   hostVal = ((fabs((double)(*delta_influence)(rowActor,j)))+(*influence_matrix)(rowActor, j));
		   if (hostVal > 1) 
			   hostVal = 1;
		   if (hostVal < -1)
			   hostVal = -1;
		   sumInfl = sumInfl + hostVal;
		   influence_matrix->set(rowActor,j,hostVal);
	   }
	  // }
	   for (j=0; j < hostilityMat->cols(); j++){
		   if (sumInfl > 0){
			   float newValue = (float)(*influence_matrix)(rowActor, j)/(float)sumInfl;
			influence_matrix->set(rowActor,j, newValue);
		   }
			else 
				influence_matrix->set(rowActor,j,0);
	   }
	//	influence_matrix->dump();
     std::cout<<"the Values Are: "<<std::endl;
     
	 //Ends  Update Influence
     

	}

}

void simulation_engine::get_history_actor_effect_matrix(EnhancedDenseMatrixT<float>* &hist_actor_effect, std::vector<simulation_events*> &rEvents, int timePeriod){
	for (std::vector<simulation_events*>::iterator itEvents= rEvents.begin(); itEvents != rEvents.end(); itEvents++){
		int timeEnd = (*itEvents)->getEndTime();
		std::string theReceiver = (*itEvents)->getSender();
		std::string rDirection = (*itEvents)->getDirection();
			if (timeEnd <= timePeriod){
				std::map<std::string, float> itRes = (*itEvents)->getResources();
				for(std::map<std::string, float>::iterator iR=itRes.begin(); iR != itRes.end(); iR++){
					std::string rType = (*iR).first;
					int colM = hist_actor_effect->colIndex(rType);
					int rowM = hist_actor_effect->rowIndex(theReceiver);
					if (rDirection == "hostile")
					hist_actor_effect->set(rowM, colM, (*hist_actor_effect)(rowM, colM)-(*iR).second);
					else
					hist_actor_effect->set(rowM, colM, (*hist_actor_effect)(rowM, colM)+(*iR).second);
					
				}
			}
	}
	
}

void simulation_engine::update_tendencies(EnhancedDenseMatrixT<float>* tendencyMat, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string,std::vector<std::string> > &performedActions){
   for(std::vector<actor*>::iterator a_iter=actors.begin(); a_iter != actors.end(); a_iter++){
		std::map<std::string,std::vector<std::string> >::iterator tempActions=performedActions.find((*a_iter)->name());
		int rowActor = tendencyMat->rowIndex((*a_iter)->name());
		std::vector<std::string> theActorActions = tempActions->second;
		std::vector<std::string>::reverse_iterator it_actions=theActorActions.rbegin();
		std::string theActionType = (*it_actions).substr(0, (*it_actions).find(";"));
		int colAction = tendencyMat->colIndex(theActionType);

		double current_value = (*tendencyMat)(rowActor,colAction);
        double logValue = fabs(log10(current_value));
		double test = ceil(logValue);
		//if ((logValue - test) >= 0.5)
		//	test = test + 1;

		if (current_value < .00000001){
			test = 0;

		} else if (current_value == 1){
			test = 1;
		}
		else{
			double test1 = pow(10, test);
		    test = ((current_value * test1) + 1)/test1;
		}

		tendencyMat->set(rowActor,colAction, test);
   }
}

void simulation_engine::compute_action_resource_reqs(std::vector<std::string> &action_types, std::vector<std::string> &resource_types,std::vector<action_resource_required*> &action_resource_requireds,EnhancedDenseMatrixT<float>* &action_x_resource_required ){
	int rows = 0;
	for(std::vector<std::string>::const_iterator a_iter = action_types.begin(); a_iter != action_types.end(); ++a_iter){
		action_x_resource_required->rowname(rows, (*a_iter));
		rows++;
	}
	int cols =0;
	for(std::vector<std::string>::const_iterator rt_iter = resource_types.begin(); rt_iter != resource_types.end(); rt_iter++){
		action_x_resource_required->colname(cols, (*rt_iter));
		cols++;
	}
    for(std::vector<action_resource_required*>::iterator arr_iter = action_resource_requireds.begin(); arr_iter != action_resource_requireds.end(); arr_iter++){
		int row = action_x_resource_required->rowIndex((*arr_iter)->action_type());
		int col = action_x_resource_required->colIndex((*arr_iter)->resource_type());
        action_x_resource_required->set(row, col, (*arr_iter)->min_level());
	}

 //   action_x_resource_required->dump();
}

void simulation_engine::time_update_resources(std::vector<actor*> &actors, std::vector<std::string> &resource_types,EnhancedDenseMatrixT<float>* initial_actor_x_resources, EnhancedDenseMatrixT<float>* growth_actor_x_resources){
	for(std::vector<actor*>::iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
		for(std::vector<std::string>::const_iterator r_iter = resource_types.begin(); r_iter != resource_types.end(); ++r_iter){
			int row = initial_actor_x_resources->rowIndex(std::string((*act_iter2)->name()));
			int col = initial_actor_x_resources->colIndex((*r_iter));
		//	std::cout<<" Row "<<row<<" Col "<< col<<std::endl;
            initial_actor_x_resources->set(row, col, (*initial_actor_x_resources)(row, col)+(*growth_actor_x_resources)(row,col));
		}
	}
}

bool simulation_engine::find_action_effect(std::vector<action_effect*> &action_effects, std::string at, std::string aname, std::string rt, double* aeVal){
	for(std::vector<action_effect*>::iterator aeIt= action_effects.begin(); aeIt != action_effects.end(); aeIt++){

		if ((at == (*aeIt)->action_type()) && ( aname == (*aeIt)->actor()) && (rt == (*aeIt)->resource_type())){
			*aeVal = (*aeIt)->level();
			return true;
		}
	}
	return false;
}

void simulation_engine::compute_action_type_resource_type_actor(std::vector<action_effect*> &action_effects, std::vector<std::string> &resource_types, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_type_x_resource_type_x_actor ){
//void simulation_engine::compute_action_type_resource_type_actor(Connection &con, std::vector<std::string> &resource_types, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_type_x_resource_type_x_actor ){
	for(std::vector<actor*>::const_iterator aiter2 = actors.begin(); aiter2!= actors.end(); ++ aiter2){

		EnhancedDenseMatrixT<float>* action_type_by_resource_type_matrix = new EnhancedDenseMatrixT<float>(action_types.size()+1, resource_types.size()+1);
		action_type_by_resource_type_matrix->zeros(action_types.size()+1, resource_types.size()+1);

		int aidx = 0;
	// for(std::vector<action*>::const_iterator giter = goals.begin(); giter != goals.end(); ++giter){
		for(std::vector<std::string>::const_iterator at_iter = action_types.begin(); at_iter != action_types.end(); ++at_iter){
			action_type_by_resource_type_matrix->rowname(aidx, *(at_iter));
			++aidx;
		}
		int rt_idx = 0;
		for(std::vector<std::string>::const_iterator rt_iter = resource_types.begin(); rt_iter != resource_types.end(); ++rt_iter){
			action_type_by_resource_type_matrix->colname(aidx, *(rt_iter));
			++rt_idx;
		}

		aidx = 0;
        for(at_iter = action_types.begin(); at_iter != action_types.end(); ++at_iter){
		
			int rt_idx = 0;
			for(std::vector<std::string>::const_iterator rt_iter = resource_types.begin(); rt_iter != resource_types.end(); ++rt_iter){
				action_type_by_resource_type_matrix->colname(rt_idx, (*rt_iter));
				/*	action_effect ae(&con);
					if(ae.open(*(at_iter), (*aiter2)->name(), (*rt_iter))){
			
						action_type_by_resource_type_matrix->set(aidx, rt_idx, ae.level());
				
				} */
				double aeVal =0;
				if (find_action_effect(action_effects, (*at_iter), std::string((*aiter2)->name()), (*rt_iter), &aeVal)){
					action_type_by_resource_type_matrix->set(aidx, rt_idx, aeVal);
				}
			++rt_idx;
			}
		++aidx;
	 // matrix::
     // std::cout<<(*aiter2)->name()<<std::endl;
		
		
		}
//		action_type_by_resource_type_matrix->dump();
       action_type_x_resource_type_x_actor.insert(std::make_pair(std::string((*aiter2)->name()), action_type_by_resource_type_matrix));
	}
}

void simulation_engine::compute_actor_action_type_actor( std::vector<std::string> &action_types, std::vector<actor*> &actors, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_type_x_resource_type_x_actor, std::map<std::string, EnhancedDenseMatrixT<float>*> &results_actor_resources, std::map<std::string, EnhancedDenseMatrixT<float>*> &actor_x_action_type_x_actor){
	EnhancedDenseMatrixT<float>* temp_results_matrixA;
	for(std::vector<actor*>::const_iterator aiter2 = actors.begin(); aiter2!= actors.end(); ++ aiter2){
		std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator cur=action_type_x_resource_type_x_actor.find((*aiter2)->name());
		std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator cur2=results_actor_resources.find((*aiter2)->name());
		temp_results_matrixA = new EnhancedDenseMatrixT<float>(actors.size()+1, action_types.size()+1);
		matrix::multiply_transpose2(*(cur2->second),*(cur->second),*temp_results_matrixA);
	//	std::cout<<(*aiter2)->name()<<std::endl;
	//	temp_results_matrixA->dump();
		actor_x_action_type_x_actor.insert(std::make_pair(std::string((*aiter2)->name()), temp_results_matrixA));
   

	}
}

void simulation_engine::transform_to_action_type_actor_actor(std::vector<std::string> &action_types, std::vector<actor*> &actors, std::map<std::string, EnhancedDenseMatrixT<float>*> &actor_x_action_type_x_actor, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_x_actor_x_actor){
	int col_count = 0;
	EnhancedDenseMatrixT<float>* actor_x_actor_action;
	EnhancedDenseMatrixT<float>* temporal_copy;
	for(std::vector<std::string>::const_iterator at_iter = action_types.begin(); at_iter != action_types.end(); ++at_iter){
		actor_x_actor_action = new EnhancedDenseMatrixT<float>(actors.size()+1, actors.size()+1);
		int row_count = 0;
		for(std::vector<actor*>::iterator aiter2 = actors.begin(); aiter2!= actors.end(); ++ aiter2){
			std::map<std::string, EnhancedDenseMatrixT<float>*>::iterator curr=actor_x_action_type_x_actor.find((*aiter2)->name());
			temporal_copy=curr->second;
			int row_count2 = 0;
			for(std::vector<actor*>::const_iterator aiter2a = actors.begin(); aiter2a!= actors.end(); ++ aiter2a){
				actor_x_actor_action->set(row_count2, row_count,(*temporal_copy)(row_count2, col_count));
				++row_count2;
			}
			++row_count;
		}
	   //actor_x_actor_action->dump();
		action_x_actor_x_actor.insert(std::make_pair((*at_iter), actor_x_actor_action));
		col_count++;
			
	}

			
 }


bool simulation_engine::find_tendency(std::vector<tendency*> &tendencies, std::string aname, std::string atype, double* tendValue){

	for(std::vector<tendency*>::iterator itTendency = tendencies.begin(); itTendency != tendencies.end(); itTendency++){
		std::string actor = (*itTendency)->actor();
		std::string acttype = (*itTendency)->action_type();
		if ((actor == aname) && (atype == acttype)){
			*tendValue = (*itTendency)->level();
			return true;
			}
	} 
	return false;
}


void simulation_engine::compute_inital_tendency_mat(std::vector<tendency*> &tendencies, std::vector<actor*> &actors, std::vector<std::string> &action_types,EnhancedDenseMatrixT<float>* &tendencyMat){
//void simulation_engine::compute_inital_tendency_mat(std::vector<tendency*> &tendencies, std::vector<actor*> &actors, std::vector<std::string> &action_types, EnhancedDenseMatrixT<float>* &tendencyMat){
	
	tendencyMat->zeros(actors.size()+1, action_types.size()+1);
    int rows=0;
	for(std::vector<actor*>::iterator act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
        //influence_matrix->colname(infexc, (*act_iter2)->name());
		tendencyMat->rowname(rows, std::string((*act_iter2)->name()));
		 rows++;
	}
	int cols=0;
	for(std::vector<std::string>::const_iterator at_iter7 = action_types.begin(); at_iter7 != action_types.end(); ++at_iter7){
		tendencyMat->colname(cols, (*at_iter7));
		cols++;
	}
	rows=0;
	for(act_iter2 = actors.begin(); act_iter2 != actors.end(); ++act_iter2){
	     int cols = 0;
        for(std::vector<std::string>::const_iterator at_iter7 = action_types.begin(); at_iter7 != action_types.end(); ++at_iter7){
		
			double tendValue;
			if (find_tendency(tendencies, std::string((*act_iter2)->name()), (*at_iter7), &tendValue))
				tendencyMat->set(rows, cols, tendValue); 

		cols++;	
		}
		rows++;
      }
 // tendencyMat->dump();
  // std::cout<<"hola  "<<std::endl;
  }

bool simulation_engine::find_actor_goal(std::vector<actor_goal*> &actor_goals, int act, int goal, double* val ){
	for(std::vector<actor_goal*>::iterator agIt = actor_goals.begin(); agIt != actor_goals.end(); agIt++){
		if (((*agIt)->actor_id() == act) && ((*agIt)->goal_id() == goal)){
			*val = (*agIt)->value();
			return true;
		}
	}
	return false;

}

//void simulation_engine::compute_Actor_by_Goal(EnhancedDenseMatrixT<float>* &actor_x_goal, unsigned int sid, Connection &con, std::vector<std::string> &action_types, std::vector<actor*> &actors, std::vector<goal*> &goals  ){
void simulation_engine::compute_Actor_by_Goal(EnhancedDenseMatrixT<float>* &actor_x_goal, unsigned int sid, std::vector<actor_goal*> &actor_goals, std::vector<std::string> &action_types, std::vector<actor*> &actors, std::vector<goal*> &goals  ){
	int infex2 = 0;
   // for(std::vector<action*>::const_iterator giter = goals.begin(); giter != goals.end(); ++giter){
      for(std::vector<actor*>::const_iterator act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
      actor_x_goal->rowname(infex2, (*act_iter)->name());
	  //influence_matrix->colname(infex2, (*act_iter)->name());
      ++infex2;
	  };
   infex2 = 0;
   for(std::vector<goal*>::const_iterator g_iter = goals.begin(); g_iter != goals.end(); ++g_iter){
      actor_x_goal->colname(infex2, (*g_iter)->name());
	  //influence_matrix->colname(infex2, (*act_iter)->name());
      ++infex2;
	  };
  int mx = 0;
  //std::cerr<<"entreeeeee"<<"\r\n";
  for(act_iter = actors.begin(); act_iter != actors.end(); ++act_iter){
    int mmx = 0;
	for(g_iter = goals.begin(); g_iter != goals.end(); ++g_iter){
		/* actor_goal ag(&con);
		if(ag.open( (*act_iter)->id(),(*g_iter)->id())){
          actor_x_goal->set(mx, mmx, ag.value());
		} */
		double goalVal = 0;
		
		if (find_actor_goal(actor_goals, (*act_iter)->id(),(*g_iter)->id(), &goalVal )){
			
			actor_x_goal->set(mx, mmx, goalVal);
		}
		++mmx;
  }
	++mx;
  }
 // std::cerr<<"esta es la matrizzzzzzzzzzzzzzzzz"<<"\r\n";
 //  actor_x_goal->dump();
//   std::cout<<"pare"<<std::endl;
}

void simulation_engine::saveInfluences(int ru, int ti, periodOut* po,EnhancedDenseMatrixT<float>* influence_matrix, std::vector<actor*> &actors){
	for(std::vector<actor*>::iterator itS = actors.begin(); itS != actors.end(); itS++){
		for(std::vector<actor*>::iterator itT = actors.begin(); itT != actors.end(); itT++){
			int row = influence_matrix->rowIndex(std::string((*itS)->name()));
			int col = influence_matrix->colIndex(std::string((*itT)->name()));
			
		//	if (((*influence_matrix)(row, col)) != 0){
				influenceOut* ino = new influenceOut(ru, ti);
				ino->addInfluence(std::string((*itS)->name()), std::string((*itT)->name()),(*influence_matrix)(row, col) );
				po->addInfluence(ino);
		//	}
		}
	}
	
}

void simulation_engine::saveResources(int ru, int ti, periodOut* po,EnhancedDenseMatrixT<float>* actor_resource_Mat, std::vector<actor*> &actors, std::vector<std::string> &resource_types){
	for(std::vector<actor*>::iterator itS = actors.begin(); itS != actors.end(); itS++){
		for(std::vector<std::string>::iterator riT = resource_types.begin(); riT != resource_types.end(); riT++){
			int row = actor_resource_Mat->rowIndex(std::string((*itS)->name()));
			int col = actor_resource_Mat->colIndex((*riT));
			
		//	if (((*influence_matrix)(row, col)) != 0){
				resourceOut* ro = new resourceOut(ru, ti);
				ro->addResource(std::string((*itS)->name()), (*riT),(*actor_resource_Mat)(row, col) );
				po->addResource(ro);
		//	}
		}
	}
	
}



//saveTendencies(runs,times,po,tendencyMat, actors,action_types);
void simulation_engine::saveTendencies(int ru, int ti, periodOut* po,EnhancedDenseMatrixT<float>* tendencyMat, std::vector<actor*> &actors, std::vector<std::string> &action_types){
	for(std::vector<actor*>::iterator itS = actors.begin(); itS != actors.end(); itS++){
		for(std::vector<std::string>::iterator itAT = action_types.begin(); itAT != action_types.end(); itAT++){
			int row = tendencyMat->rowIndex(std::string((*itS)->name()));
			int col = tendencyMat->colIndex((*itAT));
			
		//	if (((*influence_matrix)(row, col)) != 0){
				tendencyOut* to = new tendencyOut(ru, ti);
				to->addTendency(std::string((*itS)->name()), (*itAT),(*tendencyMat)(row, col) );
				po->addTendency(to);
		//	}
		}
	}
	
}



 void simulation_engine::saveHostilities(int ru, int ti, periodOut* po,EnhancedDenseMatrixT<float>* hostilityMat, std::vector<actor*> &actors){
	for(std::vector<actor*>::iterator itS = actors.begin(); itS != actors.end(); itS++){
		for(std::vector<actor*>::iterator itT = actors.begin(); itT != actors.end(); itT++){
			int row = hostilityMat->rowIndex(std::string((*itS)->name()));
			int col = hostilityMat->colIndex(std::string((*itT)->name()));
			
		//	if (((*hostilityMat)(row, col)) != 0){
				hostilityOut* hoo = new hostilityOut(ru, ti);
				hoo->addHostility(std::string((*itS)->name()), std::string((*itT)->name()),(*hostilityMat)(row, col) );
				po->addHostility(hoo);
		//	}
		}
	}
	
}

 void simulation_engine::log_Hostility_Influence(EnhancedDenseMatrixT<float>* hostilityMat, EnhancedDenseMatrixT<float>* influence_matrix, std::map<std::string, agent_measures*>* &actor_measures){
	 int numActors =0;
	 for(std::map<std::string, agent_measures*>::iterator itM= actor_measures->begin(); itM != actor_measures->end(); itM++){
		 std::string theActor = (*itM).first;
		 std::cout<< theActor;
		 int hos_Index = hostilityMat->rowIndex(theActor);
         int inf_Index = influence_matrix->rowIndex(theActor);
         std::cout<<"hola\n";
		 double avgHostility = 0;
		 double avgInfluence = 0;
		 int counter = 0;
		 for(int j=0; j < hostilityMat->cols(); j++){
			 avgHostility = avgHostility + (*hostilityMat)(hos_Index, j);
			 avgInfluence = avgInfluence + (*influence_matrix)(inf_Index, j);
			 counter++;
		 }
		 avgHostility = avgHostility/counter;
         avgInfluence = avgInfluence/counter;
		 (*itM).second->add_hostility_avg(avgHostility);
		 (*itM).second->add_influence_avg(avgInfluence);

	 }



 }
			


void simulation_engine::start_simulation(){
	  
	actor_x_goal = new EnhancedDenseMatrixT<float>(actors.size()+1, goals.size()+1);
	actor_resource_Mat= new EnhancedDenseMatrixT<float>(actors.size()+1,resource_types.size()+1); 
	influence_matrix = new EnhancedDenseMatrixT<float>(actors.size()+1, actors.size()+1);
	weights_matrix = new EnhancedDenseMatrixT<float>(actors.size()+1, actors.size()+1);
	I_weights_matrix = new EnhancedDenseMatrixT<float>(actors.size()+1, actors.size()+1);
	InfluenceWeigthtsMat = new EnhancedDenseMatrixT<float>(actors.size()+1, actors.size()+1);
	std::map<int,hostility*> actorsHostility;
	hostilityMat = new EnhancedDenseMatrixT<float>(actors.size()+1,actors.size()+1); 
	tendencyMat = new EnhancedDenseMatrixT<float>(actors.size()+1,action_types.size()+1); 

	compute_Actor_by_Goal(actor_x_goal, sid, actor_goals, action_types, actors, goals  );	
	compute_goal_resource_type_actor(goal_effects, goals, actors, resource_types, goal_x_resource_type_x_actor);

    compute_actor_resource_type(actor_x_goal, resource_types, goal_x_resource_type_x_actor, actors, results_actor_resources); 
	compute_actor_resource_receiver(resource_types, actors,results_actor_resources,actor_resources_receiver);
	
	compute_action_type_resource_type_actor(action_effects, resource_types, actors, action_types, action_type_x_resource_type_x_actor );
  
    compute_actor_action_type_actor(action_types, actors,action_type_x_resource_type_x_actor,results_actor_resources,actor_x_action_type_x_actor);
    transform_to_action_type_actor_actor(action_types, actors, actor_x_action_type_x_actor, action_x_actor_x_actor);


   
   init_actions_taken(actors, performedActions);

   for (int runs =1; runs <= nRuns; runs++){
	    runOut* tempRunOut = new runOut(scenario_id,runs);
		currentOutput->addRun(tempRunOut);
		get_initial_influence_matrix(influences, actors, influence_matrix);
		get_actor_resources_matrix(actor_resources, actors, resource_types, actor_resource_Mat);
  
		get_hostility_matrix(hostilities, actors, hostilityMat);
	//	hostilityMat->dump();
       // influence_matrix->dump();
		getSimilarities(actors, hostilityMat, influence_matrix, similarities);
		tendencyMat->zeros(actors.size()+1, action_types.size()+1);
  
		compute_inital_tendency_mat(tendencies, actors, action_types,tendencyMat);
      // tendencyMat->dump();
		growth_actor_x_resources = new EnhancedDenseMatrixT<float>(actors.size()+1, resource_types.size()+1);
		initial_actor_x_resources = new EnhancedDenseMatrixT<float>(actors.size()+1, resource_types.size()+1);
		action_x_resource_required = new EnhancedDenseMatrixT<float>(action_types.size()+1, resource_types.size()+1);
		action_x_resource_required->zeros(action_types.size()+1, resource_types.size()+1);

		compute_initial_actor_resources_matrix(actor_resources, actors, resource_types, initial_actor_x_resources);
		compute_growth_actor_resources_matrix(actor_resources, actors, resource_types, growth_actor_x_resources);
		compute_action_resource_reqs(action_types, resource_types,action_resource_requireds, action_x_resource_required );
  
		
		actor_measures = new std::map<std::string, agent_measures*>;
		
		populate_initial_measures(actors, actor_measures);
		//tempRunOut->addMeasures(actor_measures);
		for (int times =1; times <= nTimePeriods; times++){
			periodOut* po= new periodOut(scenario_id,runs,times);
			tempRunOut->addPeriod(po);
			actor_measures = new std::map<std::string, agent_measures*>;
		
			populate_initial_measures(actors, actor_measures);
			po->addMeasures(actor_measures);
			saveInfluences(runs,times,po,influence_matrix, actors);
            saveHostilities(runs,times,po,hostilityMat, actors);
			saveTendencies(runs,times,po,tendencyMat, actors,action_types);
            saveResources(runs,times,po,initial_actor_x_resources, actors,resource_types);
			get_weights_matrices_influence_weights(actors, influence_matrix, weights_matrix, I_weights_matrix, InfluenceWeigthtsMat);
			compute_actionType_tendencies_cube(action_types, actors, tendencyMat, hostilityMat, tendenciesCube);
			compute_beliefs(similarities, actors, action_types, intelligences, tendenciesCube,bel_Cube);
			compute_beliefs(similarities, actors, action_types, intelligences, tendenciesCube,bel_Cube_time1);
			compute_sum_beliefs_weights_Iweights(weights_matrix, I_weights_matrix, influence_matrix, actors, action_types, bel_Cube,bel_Cube_time1,  bel_Cube_Aux);
	        compute_sum_beliefs_weights_Iweights(weights_matrix, I_weights_matrix, influence_matrix, actors, action_types, bel_Cube,bel_Cube_time1,  bel_Cube_time1_Aux);
	        compute_probability_actions_actor(actors, action_types, bel_Cube_Aux, action_x_actor_x_actor,  actorActionProbabilities);
		//	print_actions_actor(actorActionProbabilities, actors, action_types);
			sort_actions_actor(actorActionProbabilities, actors, action_types, actionRanks);
          //  print_action_rankings(actionRanks, actors, action_types);
			get_specific_actions_ranks(spec_actions_Rank, actions, actors, action_types, actionRanks, action_proportions);
         //   print_action_rankings(spec_actions_Rank, actors, action_types);
			filter_specific_actions_to_10(spec_actions_Rank, actors, filtered_spec_actions_Rank);
			time_update_resources(actors, resource_types, initial_actor_x_resources, growth_actor_x_resources);
			compute_attractiveness_actions(actions, actors, filtered_spec_actions_Rank, attractiveness_actions_Rank);
		//	print_action_rankings(attractiveness_actions_Rank, actors, action_types);
		//	select_actions(times, hostilityMat,actions,  resource_types, attractiveness_actions_Rank, actors, action_x_resource_required, performedActions, initial_actor_x_resources, actor_measures, action_type_x_resource_type_x_actor);
            select_actions(times, hostilityMat,actions,  resource_types, filtered_spec_actions_Rank, actors, action_x_resource_required, performedActions, initial_actor_x_resources, actor_measures, action_type_x_resource_type_x_actor);
            log_Hostility_Influence(hostilityMat,influence_matrix, actor_measures);
			execute_actions(times, goal_x_resource_type_x_actor, hostilityMat, influence_matrix, actor_resources_receiver, resource_types, actors, initial_actor_x_resources, actor_measures);
			
			update_tendencies(tendencyMat, actors, action_types, performedActions);
		//	print_actor_measures(actor_measures); 
            
			
		}
  
	
   }
   
	currentOutput->print();
}













