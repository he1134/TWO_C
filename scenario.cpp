/* Copyright (c) 2001-2005 Kathleen M. Carley - Center for Computational Analysis of Social and Organizational Systems (CASOS), 
Institute for Software Research International (ISRI), School of Computer Science (SCS), Carnegie Mellon University (CMU), 
5000 Forbes Avenue - Pittsburgh, PA 15213-3890 - All Rights Reserved. */
/***************************************************************************
                          scenario.cpp  -  description
                             -------------------
    begin                : January 2005
    email                : ojuarez@cs.cmu.edu
 ***************************************************************************/

#include "scenario.h"
#include <sstream>
#include <DBTypes.h>

struct scenario_impl {
  scenario_impl(): col_id(0), col_runs(0), col_time_periods(0){ }
  ~scenario_impl(){}
  void clear(){
	col_id=0;
	col_runs=0;
	col_time_periods=0;
  }
  int col_id;
  int col_runs;
  int col_time_periods;
};

scenario::scenario(Connection* c, int id /*=0*/ ){
  _data = new impl;
  con  = c;
  if(id){
    _data->col_id = id;
    open(true);
  }
}

scenario::~scenario(){
  if (_data) delete _data;
}

bool scenario::open(bool assume_one_result/*=false*/){

  if(0==con) return false;
  
  if(!(stmt.str()).size() && _data->col_id){
    stmt <<"select col_id, col_runs, col_time_periods from tbl_scenario where col_id="<<_data->col_id;
  }

#ifdef DEBUG
  std::cerr<<"OUTPUT: going to execute statement "<<stmt.str()<<std::endl;
#endif

  pstmt = con->preparedStatement(stmt.str());

  if(!execute()){
    reset();
    return false;
  }
  rset = pstmt->getResultSet();
  
  rset->bind(&_data->col_id, 1, TYPE_INT);
  rset->bind(&_data->col_runs, 2, TYPE_INT);
  rset->bind(&_data->col_time_periods, 3, TYPE_INT);
  
  if(assume_one_result){
    if(!next()){
      reset();
      return false;
    }
    reset();     
    return true;
  }

  return true;
}

bool scenario::next(){
   if( rset == 0) return false;
  _data->clear();
  if(!rset->next()) {
    return false;
  }  
  return true;
}

int scenario::id() const{
  return _data->col_id;
}

int scenario::runs() const{
  return _data->col_runs;
}

int scenario::time_periods() const{
  return _data->col_time_periods;
}










