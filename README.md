## Halton

Fast Halton sequence generator. 
Generate random-start randomly permuted Halton sequence of low discrepancy numbers. 
Peak performance 10^8 numbers in 1.2 second. As a reference, 2x10^8 numbers in 1.2 second using Mersenne Twister pseudorandom number generator on the same computer (Intel i7-2630QM CPU @ 2.00GHz, DDR3 1333MHz). On a computer with larger cache and faster RAM, the performance of halton will be closer to Mersenne Twister.

TODO: Follow C++ 17 standard and remove dimension limitation.

## References:

Ökten, G., Shah, M. and Goncharov, Y., Random and deterministic digit permutations of the Halton sequence. Monte Carlo and Quasi-Monte Carlo Methods, Springer, 2010, 609-622.<br />
Ökten, G., Generalized von Neumann-Kakutani transformation and random-start scrambled Halton sequences. Journal of Complexity, 2009, Vol 25, No 4, 318--331.<br />
Xu, L., & Ökten, G., High-performance financial simulation using randomized quasi-Monte Carlo methods. Quantitative Finance, 2015, Vol 15, No 8, 1425-1436.
