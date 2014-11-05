/*
 * debug.h
 *
 *  Created on: May 28, 2013
 *      Author: brykt
 */

#ifndef DEBUG_H_
#define DEBUG_H_

		#define DEBUG

#ifdef DEBUG
  #define debug(x) x
#else
  #define debug(x)
#endif


#endif /* DEBUG_H_ */
