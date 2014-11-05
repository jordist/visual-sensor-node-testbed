/*
 * thread_class.h
 *
 *  Created on: May 13, 2013
 *      Author: brykt
 */

#ifndef THREADCLASS_H_
#define THREADCLASS_H_

#include "pthread.h"

static void release_mutex(void*);

class ThreadClass {
public:
 ThreadClass();
 virtual ~ThreadClass();

 bool StartThread();
 int WaitForThreadToExit();
 void set_should_stop_to_true();
 bool get_should_stop();

protected: // TODO Why is this protected?
 virtual void ThreadEntry() = 0;

private:
 static void* ThreadEntryFunc(void* This);
 pthread_t* thread_;
 bool running_status_;
 bool detached_status_;
};

#endif /* THREADCLASS_H_ */
