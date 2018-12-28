#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <time.h>

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

//乱数ジェネレータ
class generator
{
public:
	unsigned long mt[N]; /* the array for the state vector  */
	int mti = N + 1; /* mti==N+1 means mt[N] is not initialized */
public:
	/* initializes mt[N] with a seed */
	void init(unsigned long s)
	{
		mt[0] = s & 0xffffffffUL;
		for(mti = 1; mti<N; mti++) {
			mt[mti] =
				(1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
			/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
			/* In the previous versions, MSBs of the seed affect   */
			/* only MSBs of the array mt[].                        */
			/* 2002/01/09 modified by Makoto Matsumoto             */
			mt[mti] &= 0xffffffffUL;
			/* for >32 bit machines */
		}
	}

	/* initialize by an array with array-length */
	/* init_key is the array for initializing keys */
	/* key_length is its length */
	/* slight change for C++, 2004/2/26 */
	void init_by_array(unsigned long init_key[],int key_length)
	{
		int i,j,k;
		init(19650218UL);
		i = 1; j = 0;
		k = (N>key_length ? N : key_length);
		for(; k; k--) {
			mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL))
				+ init_key[j] + j; /* non linear */
			mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
			i++; j++;
			if(i >= N) { mt[0] = mt[N - 1]; i = 1; }
			if(j >= key_length) j = 0;
		}
		for(k = N - 1; k; k--) {
			mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941UL))
				- i; /* non linear */
			mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
			i++;
			if(i >= N) { mt[0] = mt[N - 1]; i = 1; }
		}

		mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
	}

private:
	/* generates a random number on [0,0xffffffff]-interval */
	unsigned long rand_int32(void)
	{
		unsigned long y;
		static unsigned long mag01[2] = {0x0UL,MATRIX_A};
		/* mag01[x] = x * MATRIX_A  for x=0,1 */

		if(mti >= N) { /* generate N words at one time */
			int kk;

			if(mti == N + 1)   /* if init_genrand() has not been called, */
				init(5489UL); /* a default initial seed is used */

			for(kk = 0;kk<N - M;kk++) {
				y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
				mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
			}
			for(;kk<N - 1;kk++) {
				y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
				mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
			}
			y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
			mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

			mti = 0;
		}

		y = mt[mti++];

		/* Tempering */
		y ^= (y >> 11);
		y ^= (y << 7) & 0x9d2c5680UL;
		y ^= (y << 15) & 0xefc60000UL;
		y ^= (y >> 18);

		return y;
	}

	/* generates a random number on [0,0x7fffffff]-interval */
	long rand_int31(void)
	{
		return (long)(rand_int32() >> 1);
	}

	/* generates a random number on [0,1]-real-interval */
	double rand_real1(void)
	{
		return rand_int32()*(1.0 / 4294967295.0);
		/* divided by 2^32-1 */
	}

	/* generates a random number on [0,1)-real-interval */
	double rand_real2(void)
	{
		return rand_int32()*(1.0 / 4294967296.0);
		/* divided by 2^32 */
	}

	/* generates a random number on (0,1)-real-interval */
	double rand_real3(void)
	{
		return (((double)rand_int32()) + 0.5)*(1.0 / 4294967296.0);
		/* divided by 2^32 */
	}

	/* generates a random number on [0,1) with 53-bit resolution*/
	double rand_real53(void)
	{
		unsigned long a = rand_int32() >> 5,b = rand_int32() >> 6;
		return(a*67108864.0 + b)*(1.0 / 9007199254740992.0);
	}

public:
	/* generates a random number on [0,1)-real-interval */
	double rand(void)
	{
		return rand_real53();
	}

	/* 一様乱数[min,sup)*/
	double rand(double min,double sup)
	{
		return min + (sup - min) * rand();
	}

	/* generates a random number */
	double standard_normal_distribution()
	{
		double X = rand_real3();
		double Y = rand_real3();
		return sqrt(-2 * log(X)) * cos(6.283185307179586476925286766559 * Y);
	}
};

#undef N
#undef M
#undef MATRIX_A
#undef UPPER_MASK
#undef LOWER_MASK


int main(int argc,char** argv)
{
	//coef
	std::vector<double> a;

	//input
	int t_count = atoi(argv[1]);
	for(int i = 2;i < argc;i++)a.push_back(atof(argv[i]));

	//timeseries
	int t_eq = 1000;
	std::vector<double> tseries;
	tseries.resize(t_count + t_eq);

	//init
	generator gen;
	gen.init(time(NULL));
	for(int t = 0;t < a.size();t++)tseries[t] = gen.standard_normal_distribution();

	//printf("%lf\n",a[0]);

	//time evolution
	for(int t = a.size();t < t_eq;t++)
	{
		tseries[t] = gen.standard_normal_distribution();
		for(int tau = 0;tau < a.size();tau++)
		{
			tseries[t] += a[tau] * tseries[t - tau - 1];
		}
	}

	//平衡状態が達成されたと考える
	for(int t = t_eq;t < t_count + t_eq;t++)
	{
		tseries[t] = gen.standard_normal_distribution();
		for(int tau = 0;tau < a.size();tau++)
		{
			tseries[t] += a[tau] * tseries[t - tau - 1];
		}
	}

	//output
	for(int t = t_eq;t < t_count + t_eq;t++)printf("%lf\n",tseries[t]);

	return 0;
}

