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
email: lxu@math.fsu.edu, okten@math.fsu.edu
*/

#include <cassert>
#include <cmath>
#include <cstring>
#include "halton.h"


MersenneTwister* halton::pmt = MersenneTwister::Instance();

uint32 halton::base[HALTON_DIM];
uint64 halton::pwr[HALTON_DIM][WIDTH];
uint32** halton::ppm = NULL;

halton::halton(bool isMaster)
{
	isRandomStart = false;
	isRandomlyPermuted = false;
	isPowerInitialized = false;
	isMasterThread = isMaster;
	dim = 0;
	isPermutationReady = false;
}

void halton::set_power_buffer()
{
	for (uint16 d = 0; d < dim; d++)
		for (uint8 j = 0; j < WIDTH; j++)
		{
			if (j == 0)
				pwr[d][j] = base[d];
			else
				pwr[d][j] = pwr[d][j - 1] * base[d];
		}
	isPowerInitialized = true;
}

void halton::clear_buffer()
{
	memset(digit, 0, sizeof(digit));
	memset(rnd, 0, sizeof(rnd));
}

void halton::init_expansion()
{
	uint16 i;
	int8 j;
	uint64 n = 0;
	uint32 d = 0;
	for (i = 0; i < dim; i++)
	{
		n = start[i] - 1;
		j = 0;
		while (n > 0)
		{
			digit[i][j] = n % base[i];
			n = n / base[i];
			j++;
		}
		j--;
		while (j >= 0)
		{
			d = digit[i][j];
			if (isRandomlyPermuted)
				d = permute(i, j);
			rnd[i][j] = rnd[i][j + 1] + d * 1.0 / pwr[i][j];
			j--;
		}
	}
}

void halton::genHalton()
{
	uint16 i;
	int8 j;
	uint64 n = 0;
	uint32 d = 0;

	for (i = 0; i < dim; i++)
	{
		j = 0;
		while (digit[i][j] + 1 >= base[i])
			j++;
		digit[i][j]++;
		d = digit[i][j];
		if (isRandomlyPermuted)
			d = permute(i, j);
		rnd[i][j] = rnd[i][j + 1] + d * 1.0 / pwr[i][j];
		for (j = j - 1; j >= 0; j--)
		{
			digit[i][j] = 0;
			d = 0;
			if (isRandomlyPermuted)
				d = permute(i, j);
			rnd[i][j] = rnd[i][j + 1] + d * 1.0 / pwr[i][j];
		}
	}
}

uint32 inline halton::permute(uint8 i, uint8 j)
{
	return *(*(ppm + i) + digit[i][j]);
}

void halton::set_permutation()
{
	if (ppm)
	{
		for (uint32 i = 0; i < dim; i++)
		{
			delete[] * (ppm + i);
			*(ppm + i) = NULL;
		}
		delete[] ppm;
		ppm = NULL;
	}
	ppm = new uint32*[dim];

	uint32 j, k, tmp;

	for (uint32 i = 0; i < dim; i++)
	{
		*(ppm + i) = new uint32[base[i]];
		for (j = 0; j < base[i]; j++)
			*(*(ppm + i) + j) = j;

		for (j = 1; j < base[i]; j++)
		{
			tmp = (uint32)floor(pmt->genrand64_real3() * base[i]);
			if (tmp != 0)
			{
				k = *(*(ppm + i) + j);
				*(*(ppm + i) + j) = *(*(ppm + i) + tmp);
				*(*(ppm + i) + tmp) = k;
			}
		}
	}
	isPermutationReady = true;
}

void halton::get_prime(uint16 n, uint32 *p)
{
	if (n <= 0) assert(0);
	uint32 prime = 1;
	do
	{
		prime++;
		*p++ = prime;
		n--;
		for (uint32 i = 2; i <= sqrt(prime * 1.0); i++)
			if (prime % i == 0)
			{
				n++;
				p--;
				break;
			}
	} while (n > 0);
}

void halton::set_dim(uint16 d)
{
	assert(d <= HALTON_DIM);
	dim = d;
}

void halton::set_start()
{
	for (uint32 i = 0; i < dim; i++)
	{
		if (isRandomStart)
			start[i] = rnd_start(pmt->genrand64_real3(), base[i]);
		else
			start[i] = 1;
		//printf("%ulld\n", start[i]);
	}
}

void halton::alter_start(uint32 d, uint64 rs)
{
	start[d - 1] = rs;
}

void halton::set_base()
{
	get_prime(dim, base);
}

real halton::get_rnd(uint16 d)
{
	return rnd[d - 1][0];
}

uint64 halton::rnd_start(real r, uint32 base)
{
	uint64 z = 0;
	uint16 cnt = 0;
	uint64 b = base;
	while (r > 1e-16) // Potential deal loop?
	{
		cnt = 0;
		if (r >= 1.0 / b)
		{
			cnt = (uint16)floor(r * b);
			r = r - cnt * 1.0 / b;
			z += cnt * b / base;
		}
		b *= base;
	}
	return z;
}

void halton::init(uint16 dim, bool rs, bool rp)
{
	set_dim(dim);
	if (isMasterThread)
		set_base();
	set_random_start_flag(rs);
	set_permute_flag(rp);
	configure();
}

void halton::configure()
{
	if (isMasterThread)
		set_start();
	if (isMasterThread && !isPowerInitialized)
		set_power_buffer();
	clear_buffer();
	if (isMasterThread && isRandomlyPermuted && !isPermutationReady)
		set_permutation();
	init_expansion();
}
