#include <cstdio>
#include <memory>
#include "halton.h"
#include "timer.h"

#define N 10000000
#define DIM 100

//MersenneTwister* pmt = MersenneTwister::Instance();

int main(void)
{
START
	std::shared_ptr<halton> p(new halton());
	p->init(DIM, true, true); // DIM dimensions, random start, randomly permuted
	//printf("Running...\n");

	for (size_t i = 1; i <= N; i++)
	{
		//Generate the ith vector in the halton sequence, the (i-1)th vector will be discarded
		p->genHalton();
		//pmt->genrand64_real3();
		//for(uint32 j = 1; j <= DIM; j++)
		//	//Use the jth dimension of the ith vector
		//	fprintf(stderr, "%1.16f ", p -> get_rnd(j)); 
		//fprintf(stderr, "\n");

	}
STOP
	return 0;
}