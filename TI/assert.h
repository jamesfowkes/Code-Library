/*
 * assert.h
 *
 *  Created on: 6 Jul 2012
 *      Author: james
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#ifdef DEBUG
#define assert(x) { while (!(x)); }
#else
#define assert(x)
#endif

#endif /* ASSERT_H_ */
