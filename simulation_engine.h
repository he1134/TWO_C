
/* Copyright (c) 2001-2005 Kathleen M. Carley - Center for Computational Analysis of Social and Organizational Systems (CASOS), 
Institute for Software Research International (ISRI), School of Computer Science (SCS), Carnegie Mellon University (CMU), 
5000 Forbes Avenue - Pittsburgh, PA 15213-3890 - All Rights Reserved. */
/***************************************************************************
                          simulation_engine.h  -  description
                             -------------------
    begin                : March 2005
    email                : ojuarez@cs.cmu.edu
 ***************************************************************************/

#ifndef _simulation_engine_h_
#define _simulation_engine_h_
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
#include "RandGenerator.h"
#include "action_set.h"
#include "agent_measures.h"
#include "resource.h"
#include "scenarioOBJ.h"
#include "outputOBJ.h"
#include "goal.h"
#include "math.h"
#include <sstream>
#include <stdlib.h>

#include "Connection.h"
#include <map>

class simulation_engine {

	private: std::string		config_filename ;
	private: int				scenario_id;  
	private: std::string		aggregation;
	private: std::string		dsn; // DB Parameters
	private: std::string		dbuser;
	private: std::string		dbpwd;
	private: int				sid;  //scenario ID
	private: Connection			con;
	private: int                nRuns;
	private: int                nTimePeriods;
    private: scenarioOBJ*       currentScenario;
	private: outputOBJ*         currentOutput;
	private: RandGenerator*     randGenerator;

	private: std::vector<actor*>						actors;
	private: std::vector<actor_goal*>					actor_goals;
	private: std::vector<goal_effect*>	                goal_effects;
	private: std::vector<action_effect*>	            action_effects;
	private: std::vector<goal*>							goals;
	private: std::vector<action*>						actions;
	private: std::vector<resource*>						resources;
	private: std::vector<hostility*>					hostilities;
	private: std::vector<tendency*>						tendencies;
	private: std::vector<std::string>					resource_types; 
	private: std::vector<std::string>					action_types; 
	private: std::vector<actor_resource*>				actor_resources;
	private: std::vector<beliefaction*>					beliefss;
	private: std::vector<beliefactor*>					beliefsact;
	private: std::vector<action_resource_required*>		action_resource_requireds;
	private: std::vector<influence*>					influences; 
	private: std::vector<actor_action_proportions*>		action_proportions;
	private: std::vector<actor_resource_proportions*>	resource_proportions;
	private: std::map<int,hostility*> actorsHostility;
	private: volatilityContainer						volatilities;
	private: tensionContainer							tensions;
	private: intellContainer							intelligences;

	
	private: std::map<std::string, EnhancedDenseMatrixT<float>*> goal_x_resource_type_x_actor;
	private: std::map<std::string, EnhancedDenseMatrixT<float>*>  results_actor_resources;
	private: std::map<std::string, EnhancedDenseMatrixT<float>*>  actor_resources_receiver;
	private: std::map<std::string, EnhancedDenseMatrixT<float>*> action_type_x_resource_type_x_actor;
	private: std::map<std::string, EnhancedDenseMatrixT<float>*> actor_x_action_type_x_actor;
	private: std::map<std::string, EnhancedDenseMatrixT<float>*> action_x_actor_x_actor;
	private: EnhancedDenseMatrixT<float>* actor_x_goal;
	private: std::map<std::string, agent_measures*>* actor_measures;
	private: EnhancedDenseMatrixT<float>* actor_resource_Mat;
	private: EnhancedDenseMatrixT<float>* influence_matrix;
	private: EnhancedDenseMatrixT<float>* weights_matrix;
	private: EnhancedDenseMatrixT<float>* I_weights_matrix;
	private: EnhancedDenseMatrixT<float>* InfluenceWeigthtsMat;
	private: EnhancedDenseMatrixT<float>* hostilityMat;
//	EnhancedDenseMatrixT<float>* tendencyMat = new EnhancedDenseMatrixT<float>(actors.size()+1,action_types.size()+1); 
    private: std::map<std::string, std::map<std::string, float>* > similarities;
	private: std::map<std::string, EnhancedDenseMatrixT<float>*> tendenciesCube;
	private: std::map<std::string, matrixContainer > bel_Cube;
	private: std::map<std::string, matrixContainer > bel_Cube_time1;
    private: std::map<std::string, matrixContainer > bel_Cube_Aux;
	private: std::map<std::string, matrixContainer > bel_Cube_time1_Aux;
	private: std::map<std::string, matrixContainer> actorActionProbabilities;
	private: std::map<std::string, std::vector<std::string> > performedActions;
	private: EnhancedDenseMatrixT<float>* tendencyMat;

	private: EnhancedDenseMatrixT<float>* initial_actor_x_resources;
    private: EnhancedDenseMatrixT<float>* growth_actor_x_resources;
	private: EnhancedDenseMatrixT<float>* action_x_resource_required;
	private: std::map<std::string, std::multimap<float, std::string, std::greater<float> >* >  actionRanks;
	private: std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >  spec_actions_Rank;
	private: std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >  filtered_spec_actions_Rank;
	private: std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >  attractiveness_actions_Rank;
   
	
  
//	private: init_actions_taken(actors, performedActions);

	public: simulation_engine(scenarioOBJ* sc, outputOBJ* oo);
	public: virtual ~simulation_engine();
    public: void start_simulation();
//	public:	void compute_Actor_by_Goal(EnhancedDenseMatrixT<float>* &actor_x_goal, unsigned int sid, Connection &con, std::vector<std::string> &action_types, std::vector<actor*> &actors, std::vector<goal*> &goals  );
    public: void compute_Actor_by_Goal(EnhancedDenseMatrixT<float>* &actor_x_goal, unsigned int sid, std::vector<actor_goal*> &actor_goals, std::vector<std::string> &action_types, std::vector<actor*> &actors, std::vector<goal*> &goals  );

			//public: void compute_Actor_by_Goal(EnhancedDenseMatrixT<float>* &actor_x_goal, unsigned int sid, Connection &con, std::vector<std::string> &action_types, std::vector<actor*> &actors, std::vector<goal*> &goals  );
    public: std::string getDirection(std::vector<action*> &actions, int actId);
    public: void compute_goal_resource_type_actor(std::vector<goal_effect*> goal_effects, std::vector<goal*> &goals, std::vector<actor*> &actors, std::vector<std::string> &resource_types,  std::map<std::string, EnhancedDenseMatrixT<float>*> &goal_x_resource_type_x_actor);
	//public: void compute_goal_resource_type_actor(Connection &con, std::vector<goal*> &goals, std::vector<actor*> &actors, std::vector<std::string> &resource_types,  std::map<std::string, EnhancedDenseMatrixT<float>*> &goal_x_resource_type_x_actor);
    public: void compute_actor_resource_type(EnhancedDenseMatrixT<float>* &actor_x_goal, std::vector<std::string> &resource_types, std::map<std::string, EnhancedDenseMatrixT<float>*> &goal_x_resource_type_x_actor, std::vector<actor*> &actors, std::map<std::string, EnhancedDenseMatrixT<float>*>  &results_actor_resources);
    public: void addMatrixActorNames(EnhancedDenseMatrixT<float>* theMatrix, std::vector<actor*> &actors, int row);
    public: void addMatrixResourceNames(EnhancedDenseMatrixT<float>* theMatrix, std::vector<std::string> &resource_types, int row);
    public: void compute_actor_resource_receiver( std::vector<std::string> &resource_types, std::vector<actor*> &actors, std::map<std::string, EnhancedDenseMatrixT<float>*>  &results_actor_resources, std::map<std::string, EnhancedDenseMatrixT<float>*>  &actor_resources_receiver);
    public: void init_actions_taken(std::vector<actor*> &actors, std::map<std::string, std::vector<std::string> >  &actionsTaken);
    //public: void compute_action_type_resource_type_actor(Connection &con, std::vector<std::string> &resource_types, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_type_x_resource_type_x_actor );
    public: void compute_action_type_resource_type_actor(std::vector<action_effect*> &action_effects, std::vector<std::string> &resource_types, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_type_x_resource_type_x_actor );

	public: void compute_actor_action_type_actor( std::vector<std::string> &action_types, std::vector<actor*> &actors, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_type_x_resource_type_x_actor, std::map<std::string, EnhancedDenseMatrixT<float>*> &results_actor_resources, std::map<std::string, EnhancedDenseMatrixT<float>*> &actor_x_action_type_x_actor);
    public: void transform_to_action_type_actor_actor( std::vector<std::string> &action_types, std::vector<actor*> &actors, std::map<std::string, EnhancedDenseMatrixT<float>*> &actor_x_action_type_x_actor, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_x_actor_x_actor);
    //public: void get_initial_influence_matrix(Connection &con, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &influence_matrix);
    public: void get_initial_influence_matrix(std::vector<influence*> &influences, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &influence_matrix);
	public: void populate_initial_measures(std::vector<actor*> &actors, std::map<std::string, agent_measures*>* &actor_measures);
    public: void get_actor_resources_matrix(std::vector<actor_resource*> &actor_resources, std::vector<actor*> &actors, std::vector<std::string> &resource_types, EnhancedDenseMatrixT<float>* actor_resource_Mat);
    public: std::string find_actor_name(std::vector<actor*> &actors, int actor_id);
    //public: void get_hostility_matrix(Connection &con, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &hostilityMat);
    public: void get_hostility_matrix(std::vector<hostility*> &hostilities, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &hostilityMat);
	public: void getSimilarities(std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &hostilityMat, EnhancedDenseMatrixT<float>* &influence_matrix, std::map<std::string, simContainer* > &similarities);
    public: void compute_inital_tendency_mat(std::vector<tendency*> &tendencies, std::vector<actor*> &actors, std::vector<std::string> &action_types,EnhancedDenseMatrixT<float>* &tendencyMat);
    public: void compute_initial_actor_resources_matrix(std::vector<actor_resource*> &actor_resources, std::vector<actor*> &actors, std::vector<std::string> &resource_types, EnhancedDenseMatrixT<float>* initial_actor_x_resources);
    public: void compute_growth_actor_resources_matrix(std::vector<actor_resource*> &actor_resources, std::vector<actor*> &actors, std::vector<std::string> &resource_types, EnhancedDenseMatrixT<float>* growth_actor_x_resources);
    public: void compute_action_resource_reqs(std::vector<std::string> &action_types, std::vector<std::string> &resource_types,std::vector<action_resource_required*> &action_resource_requireds,EnhancedDenseMatrixT<float>* &action_x_resource_required );
    public: void compute_actionType_tendencies_cube(std::vector<std::string> &action_types, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &tendencyMat, EnhancedDenseMatrixT<float>* &hostilityMat, std::map<std::string, EnhancedDenseMatrixT<float>*> &tendenciesCube);
    public: void get_weights_matrices_influence_weights(std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* &influence_matrix, EnhancedDenseMatrixT<float>* &weights_matrix, EnhancedDenseMatrixT<float>* &I_weights_matrix, EnhancedDenseMatrixT<float>* &InfluenceWeigthtsMat);
    public: void compute_beliefs(std::map<std::string, simContainer* > &similarities, std::vector<actor*> &actors, std::vector<std::string> &action_types, intellContainer &intelligences, std::map<std::string, EnhancedDenseMatrixT<float>*> &tendenciesCube, std::map<std::string, matrixContainer > &bel_Cube);
    public: void compute_sum_beliefs_weights_Iweights(EnhancedDenseMatrixT<float>* &weights_matrix, EnhancedDenseMatrixT<float>* &I_weights_matrix, EnhancedDenseMatrixT<float>* &influence_matrix, std::vector<actor*> actors, std::vector<std::string> action_types, std::map<std::string, matrixContainer > &bel_Cube, std::map<std::string, matrixContainer > &bel_Cube_time1,std::map<std::string, matrixContainer >   &bel_Cube_Aux);
    public: void compute_probability_actions_actor(std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, matrixContainer > &bel_Cube_Aux, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_x_actor_x_actor,std::map<std::string, matrixContainer> &actorActionProbabilities);
    public: void sort_actions_actor(std::map<std::string, matrixContainer> actorActionProbabilities, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, std::multimap<float, std::string, std::greater<float> >* >  &actionRanks);
    public: void get_specific_actions_ranks(std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >  &spec_actions_Rank, std::vector<action*> &actions, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string, std::multimap< float, std::string, std::greater<float> >* >  &actionRanks, std::vector<actor_action_proportions*> &action_proportions);
	public: void simulation_engine::filter_specific_actions_to_10(std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &spec_actions_Rank, std::vector<actor*> &actors, std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &filtered_spec_actions_Rank);
	public: void time_update_resources(std::vector<actor*> &actors, std::vector<std::string> &resource_types,EnhancedDenseMatrixT<float>* initial_actor_x_resources, EnhancedDenseMatrixT<float>* growth_actor_x_resources);
	public: void compute_attractiveness_actions(std::vector<action*> &actions, std::vector<actor*> &actors, std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &filetered_spec_actions_Rank, std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &attractiveness_actions_Rank);
	public: action* find_action(std::vector<action*> &actions, int aNumber);
	public: void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);
    public: void select_actions(int period, EnhancedDenseMatrixT<float>* &hostilityMat, std::vector<action*> actions, std::vector<std::string> &resource_types, std::map<std::string, std::multimap<float,std::string, std::greater<float> >* > &attractiveness_actions_Rank ,std::vector<actor*> &actors,EnhancedDenseMatrixT<float>* action_resource_required_matrix, std::map<std::string, std::vector<std::string> > &performedActions, EnhancedDenseMatrixT<float>* actor_resources_matrix,std::map<std::string, agent_measures*>* &actor_measures, std::map<std::string, EnhancedDenseMatrixT<float>*> &action_type_x_resource_type_x_actor);
	public: void execute_actions(int timePeriod, std::map<std::string, EnhancedDenseMatrixT<float>*> &goal_x_resource_type_x_actor, EnhancedDenseMatrixT<float>* &hostilityMat, EnhancedDenseMatrixT<float>* influence_matrix, std::map<std::string, EnhancedDenseMatrixT<float>*>  &actor_resources_receiver, std::vector<std::string> resource_types, std::vector<actor*> &actors, EnhancedDenseMatrixT<float>* initial_actor_x_resources, std::map<std::string, agent_measures*>* &actor_measures);
	public: void get_history_actor_effect_matrix(EnhancedDenseMatrixT<float>* &hist_actor_effect, std::vector<simulation_events*> &rEvents, int timePeriod);
	public: void update_tendencies(EnhancedDenseMatrixT<float>* tendencyMat, std::vector<actor*> &actors, std::vector<std::string> &action_types, std::map<std::string,std::vector<std::string> > &performedActions);
	public: void print_actor_measures(std::map<std::string, agent_measures*>* &actor_measures);
	public: bool find_actor_goal(std::vector<actor_goal*> &actor_goals, int act, int goal, double* val );
	public: bool find_goal_effects(std::vector<goal_effect*> &goal_effects, int gid, int aid, std::string rt, double* geVal );
	public: bool find_action_effect(std::vector<action_effect*> &action_effects, std::string at, std::string aname, std::string rt, double* aeVal);
	public: bool find_influence(std::vector<influence*> &influences, int aid1, int aid2, double* infVal);
	public: bool find_hostility(std::vector<hostility*> &hostilities, int act1, int act2, double* hosVal);
	public: void print_action_rankings(std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >  &spec_actions_Rank, std::vector<actor*> &actors, std::vector<std::string> &action_types);
	
	public: bool find_tendency(std::vector<tendency*> &tendencies, std::string aname, std::string atype, double* tendValue);

	public: void print_actions_actor(std::map<std::string, matrixContainer> actorActionProbabilities, std::vector<actor*> &actors, std::vector<std::string> &action_types);
	public: void saveInfluences(int ru, int ti, periodOut* po,EnhancedDenseMatrixT<float>* influence_matrix, std::vector<actor*> &actors);
	public: void saveHostilities(int ru, int ti, periodOut* po,EnhancedDenseMatrixT<float>* hostilityMat, std::vector<actor*> &actors);
    public: void saveTendencies(int ru, int ti, periodOut* po,EnhancedDenseMatrixT<float>* tendencyMat, std::vector<actor*> &actors, std::vector<std::string> &action_types);
	public: void saveResources(int ru, int ti, periodOut* po,EnhancedDenseMatrixT<float>* actor_resource_Mat, std::vector<actor*> &actors, std::vector<std::string> &resource_types);
	public: void log_Hostility_Influence(EnhancedDenseMatrixT<float>* hostilityMat, EnhancedDenseMatrixT<float>* influence_matrix, std::map<std::string, agent_measures*>* &actor_measures);	
			//	public: std::multimap<float,std::string, std::greater<float> >::iterator find_iterator(std::map<std::string, std::multimap<float,std::string, std::greater<float> >* >::iterator &actionTaken, int limit);

  /*
  public: simulation_events(int sTime, int eTime, std::string sSender, std::string rReceiver, std::string dDirection, std::map<std::string, float> resources, std::string aName, std::string aType);
  public: simulation_events();
  public: virtual ~simulation_events();

  
  public: std::string getSender();
  public: std::string getReceiver();
  public: std::string getActName();
  public: std::string getActType();
  public: int getStartTime();
  public: int getEndTime();
  public: std::string getDirection();
  public: std::map<std::string, float> getResources();
 
  public: void setStartTime(int sTime);
  public: void setEndTime(int eTime);
  public: void setSender(std::string sSender);
  public: void setReceiver(std::string rReceiver);
  public: void setDirection(std::string direction);
  public: void setResource(std::map<std::string, float> rResources);
  public: void setActName(std::string aName);
  public: void setActType(std::string aType);
*/
    
};

#endif












