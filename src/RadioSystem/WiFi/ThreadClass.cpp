/*
 * thread_class.h
 *
 *  Created on: May 13, 2013
 *      Author: brykt
 */

#include "ThreadClass.h"
#include "stdlib.h"
#include "stdio.h"
#include <errno.h>

void release_mutex(void*) {
}

ThreadClass::ThreadClass() :
    thread_(0), running_status_(false), detached_status_(false) {
  thread_ = (pthread_t*) malloc(sizeof(pthread_t));
}

ThreadClass::~ThreadClass() {
//	  debug(printf("In ThreadClass dtor.\n"));

	  if (running_status_ == true) {
	    pthread_cancel(*thread_);
	  }
	  if (running_status_ == true && detached_status_ == false) {
//	    debug(printf("About to detach thread.\n"));
	    pthread_detach(*thread_);
//	    debug(printf("Thread detached.\n"));
	  }
	  free(thread_);
//	  debug(printf("Thread class: Thread free'd.\n"));
}

bool ThreadClass::StartThread() {
	  int result;
	  result = pthread_create(thread_, NULL, ThreadEntryFunc, this);
	  if (result  != 0) {
	    fprintf(stderr, "Thread could not be started! \n");
	  }
	  if (result  == 0) {
	    running_status_ = true;
//	    debug(printf("Thread started! \n"));
	  }
	  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	  return result;
}

int ThreadClass::WaitForThreadToExit() {
//	  debug(printf("In WaitForThreadToExit, running status: %d.\n",running_status_));
	  int result = -1;
//	  debug(printf("After setting result = -1.\n"));
	  if (running_status_ == 1) {
//	    debug(printf("In WaitForThreadToExit, before pthread_join().\n"));
	    result = pthread_join(*thread_, NULL);
	    if (result != 0) {
//	      debug(fprintf(stderr, "In WaitForThreadToExit, pthread_join() returned %d.\n", result));
	    } else {
	      detached_status_ = true;
//	      debug(printf("In WaitForThreadToExit, pthread_join() returned %d.\n",  result));
	    }
	  }
//	  debug(printf("In WaitForThreadToExit, after joining threads. \n"));
	  return result;
}

void ThreadClass::set_should_stop_to_true() {
//	  debug(printf("Before pthread_cancel in set_should_stop_to_true().\n"));
	  errno = pthread_cancel(*thread_);
	  if (errno != 0) {
	        fprintf(stderr,
	            "pthread_cancel in set_should_stop_to_true() returned %d.\n",
	            errno);

	        perror("pthread_cancel in set_should_stop_to_true()");
	      }
}

void* ThreadClass::ThreadEntryFunc(void* This) {
    ((ThreadClass*) This)->ThreadEntry();
//  debug(printf("Exiting from ThreadEntryFunc()!\n"));
  pthread_exit(NULL);
}
