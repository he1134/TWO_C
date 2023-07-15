/* Copyright (c) 2001-2005 Kathleen M. Carley - Center for Computational Analysis of Social and Organizational Systems (CASOS), 
Institute for Software Research International (ISRI), School of Computer Science (SCS), Carnegie Mellon University (CMU), 
5000 Forbes Avenue - Pittsburgh, PA 15213-3890 - All Rights Reserved. */
/***************************************************************************
                          scenario.h  -  description
                             -------------------
    begin                : January 2005
    email                : ojuarez@cs.cmu.edu
 ***************************************************************************/

#ifndef _scenario_h_
#define _scenario_h_
#include "dbobase.h"

#include <Connection.h>

struct scenario_impl;

class scenario : public dbobase{
  typedef scenario_impl impl;

  public: scenario(Connection* c, int id=0);
  public: virtual ~scenario();

  // when called from constructor open()  assumes that there is only one record to be
  // returned. If you issue a custom select and call open() we assume that the user knows
  // that there will be a result set and will need to issue next() to populate members
  public: bool open(bool assume_one_result=false);

  public: bool next();
  
  public: int id() const;
  public: int runs() const;
  public: int time_periods() const;

  protected: impl* _data;
};

#endif












