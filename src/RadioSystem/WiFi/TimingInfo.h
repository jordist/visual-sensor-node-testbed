/*
 * timing_info.h
 *
 *  Created on: Jul 21, 2013
 *      Author: brykt
 */

#ifndef TIMINGINFO_H_
#define TIMINGINFO_H_

#include "time.h"
#include "string"

struct StartTime {
	int hour_;
	int minute_;
	int second_;
	int millisecond_;
};

class TimingInfo {
public:
	TimingInfo();
	virtual ~TimingInfo();
	int get_hour() const;
	int get_minute() const;
	int get_second() const;
	int get_millisecond() const;
	void Print();
//  TimingInfoAsn_t* GetAsn1Struct(TimingInfoAsn_t* ti_asn);
	double GetSecondsPassed() const;
	void update_starttime();

private:
	tm* tm_;
	struct timeval tv_;
	double startTime_;
	//clock_t start_time_;
};

#endif /* TIMINGINFO_H_ */
