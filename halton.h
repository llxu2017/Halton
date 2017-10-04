/*
A C++ program for Random-start randomly permuted Halton sequence.
Coded by Dr. Linlin Xu and Prof. Giray Okten.

Copyright (C) 2017, Linlin Xu and Giray Okten,
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. The names of its contributors may not be used to endorse or promote
products derived from this software without specific prior written
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

References:
Okten, G., Generalized von Neumann-Kakutani transformation and random-start
scrambled Halton sequences. Journal of Complexity, 2009,
Vol 25, No 4, 318--331.

Xu, L., & Okten, G., High-performance financial Simulation Using
randomized quasi-Monte Carlo Methods. Quantitative Finance, 2015,
Vol 15, No 8, 1425-1436.

Any feedback is very welcome.
email: lxu@math.fsu.com, okten@math.fsu.edu
*/

#ifndef _HALTON_H
#define _HALTON_H

#include "mt.h"


typedef double real;

typedef unsigned short uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;

typedef short int8;
typedef int int16;
typedef long int32;
typedef long long int64;

#define HALTON_DIM 1000		// Maximum dimension allowed. Match minimum need to save memory.
// Support up to 4096 dimensions if memory permits
#define WIDTH 64			// Integer width

class halton
{
public:
	halton(bool isMaster = true);
	~halton()
	{
		if (isMasterThread && isRandomlyPermuted)
		{
			if (ppm)
			{
				for (uint16 i = 0; i < dim; i++)
					delete[] * (ppm + i);
				delete[] ppm;
				ppm = NULL;
			}
		}
	}
	void init(uint16 dim, bool rs, bool rp);
	void configure();
	void init_expansion();
	void set_dim(uint16 d);
	void set_base();
	void set_start();
	void alter_start(uint32 d, uint64 rs);
	void set_permutation();
	void set_permute_flag(bool rp) { isRandomlyPermuted = rp; }
	void set_random_start_flag(bool rs) { isRandomStart = rs; }
	void set_power_buffer();
	void clear_buffer();
	uint64 rnd_start(real r, uint32 base);

	void genHalton();

	inline uint32 permute(uint8 i, uint8 j);
	uint64 get_start(uint32 d) { return start[d - 1]; }
	void get_prime(uint16 n, uint32 *p);
	real get_rnd(uint16 d);

private:
	uint16 dim;
	uint64 start[HALTON_DIM];
	static uint32 base[HALTON_DIM];
	real rnd[HALTON_DIM][WIDTH];
	uint32 digit[HALTON_DIM][WIDTH];
	static uint64 pwr[HALTON_DIM][WIDTH];
	static uint32 **ppm;
	static MersenneTwister *pmt; //Pseudorandom number generator handler
	bool isRandomlyPermuted;
	bool isRandomStart;
	bool isPowerInitialized;
	bool isMasterThread;
	bool isPermutationReady;
};

#endif
