/*
 * threaded_que.h
 *
 *  Created on: Jun 7, 2013
 *      Author: brykt
 */

#ifndef THREADED_QUE_H_
#define THREADED_QUE_H_

#include <queue>

#include "ThreadClass.h"
#include "stdlib.h"
#include "debug.h"
#include <iostream>
#include "stdio.h"
#include <unistd.h>
#include <typeinfo>
#include "string.h"

void release_mutex(void* mutex) {
  //debug(printf("Before release in release_mutex()\n"));
  pthread_mutex_unlock((pthread_mutex_t*) mutex);
  //debug(printf("After release in  release_mutex()\n"));
}

template <typename T> class ThreadedQue : public ThreadClass {
 public:
  ThreadedQue() {
    pthread_mutex_init(&que_mutex_, NULL); //PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_init(&que_cond_, NULL); // = PTHREAD_COND_INITIALIZER;

    que_ = new std::queue<T>;
  }

  virtual ~ThreadedQue() {
    // debug(printf("In ThreadedQue dtor.\n"));
    pthread_mutex_destroy(&que_mutex_);
    pthread_cond_destroy(&que_cond_);
    delete que_;
  }

  void AddToQue(T to_add) {
    //debug(printf("Before locking mutex in AddToQue.\n"));
    pthread_mutex_lock(&que_mutex_);
    //debug(printf("After locking mutex in AddToQue.\n"));
    que_->push(to_add);

    //debug(printf("Added object to que. Que is %zd long \n", que_->size()));
    pthread_mutex_unlock(&que_mutex_);
    //debug(printf("Unlocked mutex in AddToQue()\n"));
    pthread_cond_signal(&que_cond_);
    //debug(printf("Sent signal\n"));
  }

T GetNextObjectFromQue() {
	rc = 1;
    while ((GetQueSize() == 0) && (rc != 0)) {
		clock_gettime(CLOCK_REALTIME, &t_);
		t_.tv_nsec += 50*1000000; // 50ms
		pthread_mutex_lock(&que_mutex_);
   		rc = pthread_cond_timedwait(&que_cond_, &que_mutex_, &t_);
		pthread_mutex_unlock(&que_mutex_);
		if (rc == 0) {
			//debug(printf("Got the signal!\n"));
		} else {
			//debug(printf("Will call now TimeTracking()\n"));
			TimeTracking();
		}
	}
	//debug(printf("In GetNextObjectFromQue(), before lock!\n"));
    pthread_mutex_lock(&que_mutex_);
    T to_return = que_->front();
    que_->pop();
    //debug(printf("Pop'd object from que. Que size: %zd.\n", que_->size()));
    pthread_mutex_unlock(&que_mutex_);
	return to_return;
}


  int GetQueSize() {
    //debug(printf("Before lock in GetQueSize().\n"));
    int result = pthread_mutex_lock(&que_mutex_);
    //debug(printf("Lock returned: %d, in GetQueSize().\n", result));
    int size = que_->size();
    pthread_mutex_unlock(&que_mutex_);
    //debug(printf("After unlock in GetQueSize().\n"));
    return size;
  }

  std::queue<T>* que_;

 private:
  void ThreadEntry() {
    pthread_cleanup_push(release_mutex, &que_mutex_);

    //while (!get_should_stop()) {
    while (true) {
      pthread_testcancel();
      //debug(printf("Will now call the DoStuffToObject() function!\n"));
      DoStuffToObject(GetNextObjectFromQue());
      //debug(printf("Back from DoStuffToObject()!\n"));
    }
    //debug(printf("Exiting from ThreadEntry()!\n"));
  pthread_cleanup_pop(0);
  }


  virtual int DoStuffToObject(T object) = 0;
	virtual int TimeTracking() = 0;
  pthread_mutex_t que_mutex_;
  pthread_cond_t que_cond_;
  timespec t_;
  int rc;
};

#endif /* THREADED_QUE_H_ */
