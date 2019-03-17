#pragma once
#ifndef GRANDOM_H
#define GRANDOM_H

#include "GMathCore.h"

namespace GMath
{
// Mersenne Twister random number generator
class GRandom { 
public:
  inline GRandom()									{ if (!init) seed(5489UL); init = true; }
  inline GRandom(uint32 _seed)						{ seed(_seed); init = true; }
  inline GRandom(const uint32* _array, int size)	{ seed(_array, size); init = true; }
  virtual ~GRandom() {}

  static void seed(uint32);
  static void seed(const uint32*, Int32 size);

  inline uint32 random()							{ return rand_int32(); }

  uint32 random(uint32 min, uint32 max);
  inline uint32 random(uint32 max)					{return random(0U,max);}
  inline Real random_zeroOne()						{return static_cast<float>(rand_int32())/4294967295.0f; }// divided by 2^32 - 1
  
protected:
  uint32 rand_int32();									// generate 32 bit random integer

private:
	
	inline uint32 twiddle(uint32 u, uint32 v) const		// private functions used to generate the pseudo random numbers
	{
		return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1)^((v & 1UL) ? 0x9908B0DFUL : 0x0UL);
	}
	void gen_state();									// generate new state

	// prevent copy
	GRandom(const GRandom&);							
	void operator=(const GRandom&);

	static const int n = 624, m = 397;					// compile time constants
	static uint32 state[n];								// state vector array
	static int p;										// position in state array
	static bool init;									// true if init function is called
};


inline uint32 GRandom::rand_int32() 
{
  if (p == n)
	  gen_state(); // new state vector needed
  unsigned long x = state[p++];
  x ^= (x >> 11);
  x ^= (x << 7) & 0x9D2C5680UL;
  x ^= (x << 15) & 0xEFC60000UL;
  return x ^ (x >> 18);
}

}		//EO namespace GMath

#endif // GRANDOM_H

