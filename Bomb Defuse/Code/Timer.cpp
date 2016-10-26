/// \file timer.cpp
/// \brief Code for timer class CTimer.

#include "timer.h"
#include "debug.h"

CTimer::CTimer(): m_nStartTime(0){ 
} //constructor

/// Start the timer from zero.

void CTimer::start(){
  m_nStartTime = timeGetTime();
} //start

/// Get the time.
/// \return The time in milliseconds.

int CTimer::time(){ 
  return timeGetTime() - m_nStartTime;
} //time

/// The elapsed function is a useful function for measuring repeating time 
/// intervals. Given the start and duration times, this function returns TRUE 
/// if the interval is over, and has the side-effect of resetting the start
/// time when that happens, thus setting things up for the next interval.
/// \param start Start of time interval
/// \param interval Duration of time interval
/// \return TRUE if time interval is over

bool CTimer::elapsed(int &start, int interval){
  int curtime = time(); //current time

  if(curtime >= start + interval){ //if interval is over
    start = curtime; //reset the start 
    return true; //succeed
  } //if

  else return false; //otherwise, fail
} //elapsed