/*
 * timing_info.h
 *
 *  Created on: Jul 21, 2013
 *      Author: brykt
 */

#include "TimingInfo.h"
#include "sys/time.h"
#include "sstream"
#include "getRealTime.h"
#include <stdio.h>

TimingInfo::TimingInfo() {
  gettimeofday(&tv_, NULL);
  tm_ = localtime(&tv_.tv_sec);
  //start_time_ = clock();
  startTime_ = getRealTime();
}

TimingInfo::~TimingInfo() {
  // TODO Auto-generated destructor stub
}

int TimingInfo::get_hour() const {
  return tm_->tm_hour;
}

int TimingInfo::get_minute() const {
  return tm_->tm_min;
}

int TimingInfo::get_second() const {
  return tm_->tm_sec;
}

int TimingInfo::get_millisecond() const {
  return (int) (tv_.tv_usec / 1000);
}

void TimingInfo::Print() {
  printf("Hour:%d ", tm_->tm_hour);
  printf("Minute:%d ", tm_->tm_min);
  printf("Second:%d ", tm_->tm_sec);
  printf("Milliseconds:%d ", (int) tv_.tv_usec / 1000);
  printf("\n");
}

/*TimingInfoAsn_t* TimingInfo::GetAsn1Struct(TimingInfoAsn_t* ti_asn) {
  ti_asn->hour = tm_->tm_hour;
  ti_asn->minute = tm_->tm_min;
  ti_asn->second = tm_->tm_sec;
  ti_asn->millisecond = (int) tv_.tv_usec / 1000;
  return ti_asn;
}*/

double TimingInfo::GetSecondsPassed() const {
//  clock_t current_time = clock();
//  int diff_time = current_time - start_time_;
//  float s = ((float) diff_time) / CLOCKS_PER_SEC;
//  return s;
  double endTime = getRealTime( );
  return (endTime - startTime_);
}

void TimingInfo::update_starttime(){
	gettimeofday(&tv_, NULL);
	tm_ = localtime(&tv_.tv_sec);
	startTime_ = getRealTime();
}
