/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
 
/* Note: This module was derived from the random.c developed as
   part of BSD Unix as stated in the copyright above.  It was
   modified to strip out all but the default "type 3" random
   number generation in order to save space and speed up the
   code. */

#define int long
#define unsigned unsigned long
 
extern long random(void);

/*
 * random.c:
 * An improved random number generation package.  In addition to the standard
 * rand()/srand() like interface, this package also has a special state info
 * interface.  The initstate() routine is called with a seed, an array of
 * bytes, and a count of how many bytes are being passed in; this array is then
 * initialized to contain information for random number generation with that
 * much state information.  Good sizes for the amount of state information are
 * 32, 64, 128, and 256 bytes.	The state can be switched by calling the
 * setstate() routine with the same array as was initiallized with initstate().
 * By default, the package runs with 128 bytes of state information and
 * generates far better random numbers than a linear congruential generator.
 * If the amount of state information is less than 32 bytes, a simple linear
 * congruential R.N.G. is used.
 * Internally, the state information is treated as an array of longs; the
 * zeroeth element of the array is the type of R.N.G. being used (small
 * integer); the remainder of the array is the state information for the
 * R.N.G.  Thus, 32 bytes of state information will give 7 longs worth of
 * state information, which will allow a degree seven polynomial.  (Note: the 
 * zeroeth word of state information also has some other information stored
 * in it -- see setstate() for details).
 * The random number generation technique is a linear feedback shift register
 * approach, employing trinomials (since there are fewer terms to sum up that
 * way).  In this approach, the least significant bit of all the numbers in
 * the state table will act as a linear feedback shift register, and will have
 * period 2^deg - 1 (where deg is the degree of the polynomial being used,
 * assuming that the polynomial is irreducible and primitive).	The higher
 * order bits will have longer periods, since their values are also influenced
 * by pseudo-random carries out of the lower bits.  The total period of the
 * generator is approximately deg*(2**deg - 1); thus doubling the amount of
 * state information has a vast influence on the period of the generator.
 * Note: the deg*(2**deg - 1) is an approximation only good for large deg,
 * when the period of the shift register is the dominant factor.  With deg
 * equal to seven, the period is actually much longer than the 7*(2**7 - 1)
 * predicted by this formula.
 */

/*
 * For each of the currently supported random number generators, we have a
 * break value on the amount of state information (you need at least this
 * many bytes of state info to support this random number generator), a degree
 * for the polynomial (actually a trinomial) that the R.N.G. is based on, and
 * the separation between the two lower order coefficients of the trinomial.
 */

#define 	TYPE_3		3		/* x**31 + x**3 + 1 */
#define 	BREAK_3 	128
#define 	DEG_3		31
#define 	SEP_3		3

#define 	MAX_TYPES	5		/* max number of types above */
/*
 * Initially, everything is set up as if from :
 *		initstate( 1, &randtbl, 128 );
 * Note that this initialization takes advantage of the fact that srandom()
 * advances the front and rear pointers 10*rand_deg times, and hence the
 * rear pointer which starts at 0 will also end up at zero; thus the zeroeth
 * element of the state information, which contains info about the current
 * position of the rear pointer is just
 *	MAX_TYPES*(rptr - state) + TYPE_3 == TYPE_3.
 */

static	long		randtbl[ DEG_3 + 1 ]	= { TYPE_3,
			    0x9a319039, 0x32d9c024, 0x9b663182, 0x5da1f342, 
			    0xde3b81e0, 0xdf0a6fb5, 0xf103bc02, 0x48f340fb, 
			    0x7449e56b, 0xbeb1dbb0, 0xab5c5918, 0x946554fd, 
			    0x8c2e680f, 0xeb3d799f, 0xb11ee0b7, 0x2d436b86, 
			    0xda672e2a, 0x1588ca88, 0xe369735d, 0x904f35f7, 
			    0xd7158fd6, 0x6fa6f051, 0x616e6b96, 0xac94efdc, 
			    0x36413f93, 0xc622c298, 0xf5a42ab8, 0x8a88d77b, 
					0xf5ad9d0e, 0x8999220b, 0x27fb47b9 };

/*
 * fptr and rptr are two pointers into the state info, a front and a rear
 * pointer.  These two pointers are always rand_sep places aparts, as they cycle
 * cyclically through the state information.  (Yes, this does mean we could get
 * away with just one pointer, but the code for random() is more efficient this
 * way).  The pointers are left positioned as they would be from the call
 *			initstate( 1, randtbl, 128 )
 * (The position of the rear pointer, rptr, is really 0 (as explained above
 * in the initialization of randtbl) because the state table pointer is set
 * to point to randtbl[1] (as explained below).
 */

static	long		*fptr			= &randtbl[ SEP_3 + 1 ];
static	long		*rptr			= &randtbl[ 1 ];

/*
 * The following things are the pointer to the state information table,
 * the type of the current generator, the degree of the current polynomial
 * being used, and the separation between the two pointers.
 * Note that for efficiency of random(), we remember the first location of
 * the state information, not the zeroeth.  Hence it is valid to access
 * state[-1], which is used to store the type of the R.N.G.
 * Also, we remember the last location, since this is more efficient than
 * indexing every time to find the address of the last element to see if
 * the front and rear pointers have wrapped.
 */

static	long		*state			= &randtbl[ 1 ];
static	int		rand_deg		= DEG_3;
static	int		rand_sep		= SEP_3;
static	long		*end_ptr		= &randtbl[ DEG_3 + 1 ];

/*
 * srandom:
 * Initialize the random number generator based on the given seed.
 * Initializes state[] based on the given "seed" via a linear
 * congruential generator.  Then, the pointers are set to known locations
 * that are exactly rand_sep places apart.  Lastly, it cycles the state
 * information a given number of times to get rid of any initial dependencies
 * introduced by the L.C.R.N.G.
 * Note that the initialization of randtbl[] for default usage relies on
 * values produced by this routine.
 */

void
srandom(unsigned x)
{
	register int i, j;

    j = 1;
    state[ 0 ] = x;
    for (i = 1; i < rand_deg; i++)  {
		state[i] = 1103515245*state[i - 1] + 12345;
    }
    fptr = &state[ rand_sep ];
    rptr = &state[ 0 ];
    for (i = 0; i < 10*rand_deg; i++)
    	random();
}

/*
 * random:
 * We do our fancy trinomial stuff, which is the
 * same in all the other cases due to all the global variables that have been
 * set up.  The basic operation is to add the number at the rear pointer into
 * the one at the front pointer.  Then both pointers are advanced to the next
 * location cyclically in the table.  The value returned is the sum generated,
 * reduced to 31 bits by throwing away the "least random" low bit.
 * Note: the code takes advantage of the fact that both the front and
 * rear pointers can't wrap on the same call by not testing the rear
 * pointer if the front one has wrapped.
 * Returns a 31-bit random number.
 */

long
random(void)
{
	long		i;
    *fptr += *rptr;
    i = (*fptr >> 1)&0x7fffffff;	/* chucking least random bit */
    if (++fptr  >=  end_ptr)  {
		fptr = state;
		++rptr;
    }
    else  {
		if (++rptr  >=  end_ptr)
			rptr = state;
    }
	return i;
}

