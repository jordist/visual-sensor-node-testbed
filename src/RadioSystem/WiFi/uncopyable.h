/*
 * uncopyable.h
 *
 *  Created on: Jun 26, 2013
 *      Author: brykt
 *
 *
 */

// From Scott Meyers book "Effective C++"

#ifndef UNCOPYABLE_H_
#define UNCOPYABLE_H_

class Uncopyable {
 protected:
  // allow construction
  Uncopyable() {} // and destruction of
  ~Uncopyable() {} // derived objects...
 private:
  Uncopyable(const Uncopyable&);
  // ...but prevent copying
  Uncopyable& operator=(const Uncopyable&);
};


#endif /* UNCOPYABLE_H_ */
