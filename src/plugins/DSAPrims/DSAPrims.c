/* Automatically generated by
	VMPluginCodeGenerator VMMaker.oscog-eem.2719 uuid: e40f3e94-3a54-411b-9613-5d19114ea131
   from
	DSAPlugin CryptographyPlugins-eem.14 uuid: 54292ff0-b538-43ba-9195-26766c158ba9
 */
static char __buildInfo[] = "DSAPlugin CryptographyPlugins-eem.14 uuid: 54292ff0-b538-43ba-9195-26766c158ba9 " __DATE__ ;



#include "config.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
#include "sqConfig.h"			/* Configuration options */
#include "sqVirtualMachine.h"	/*  The virtual machine proxy definition */
#include "sqPlatformSpecific.h"	/* Platform specific definitions */

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
# undef EXPORT
# define EXPORT(returnType) static returnType
# define INT_EXT "(i)"
#else
# define INT_EXT "(e)"
#endif

#include "sqMemoryAccess.h"


/*** Constants ***/
#define PrimErrBadArgument 3


/*** Function Prototypes ***/
static sqInt addBackDivisorDigitShift(sqInt digitShift);
static sqInt bigDivideLoop(void);
EXPORT(const char*) getModuleName(void);
static unsigned int leftRotateby(unsigned int anInteger, sqInt bits);
EXPORT(sqInt) primitiveBigDivide(void);
EXPORT(sqInt) primitiveBigMultiply(void);
EXPORT(sqInt) primitiveExpandBlock(void);
EXPORT(sqInt) primitiveHashBlock(void);
EXPORT(sqInt) primitiveHasSecureHashPrimitive(void);
EXPORT(sqInt) primitiveHighestNonZeroDigitIndex(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine *anInterpreter);
static sqInt subtractDivisorMultipliedByDigitdigitShift(sqInt digit, sqInt digitShift);


/*** Variables ***/
static sqInt divisorDigitCount;
static unsigned char* dsaDivisor;
static unsigned char* dsaQuotient;
static unsigned char* dsaRemainder;

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*classLargePositiveInteger)(void);
static sqInt (*fetchClassOf)(sqInt oop);
static void * (*firstIndexableField)(sqInt oop);
static sqInt (*isBytes)(sqInt oop);
static sqInt (*isWords)(sqInt oop);
static sqInt (*pop)(sqInt nItems);
static sqInt (*primitiveFailFor)(sqInt reasonCode);
static sqInt (*pushBool)(sqInt trueOrFalse);
static sqInt (*pushInteger)(sqInt integerValue);
static sqInt (*stSizeOf)(sqInt oop);
static sqInt (*stackValue)(sqInt offset);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt classLargePositiveInteger(void);
extern sqInt fetchClassOf(sqInt oop);
extern void * firstIndexableField(sqInt oop);
extern sqInt isBytes(sqInt oop);
extern sqInt isWords(sqInt oop);
extern sqInt pop(sqInt nItems);
extern sqInt primitiveFailFor(sqInt reasonCode);
extern sqInt pushBool(sqInt trueOrFalse);
extern sqInt pushInteger(sqInt integerValue);
extern sqInt stSizeOf(sqInt oop);
extern sqInt stackValue(sqInt offset);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName = "DSAPrims CryptographyPlugins-eem.14 " INT_EXT;
static sqInt remainderDigitCount;



/*	Add back the divisor shifted left by the given number of digits. This is
	done only when the estimate of quotient digit was one larger than the
	correct value.
 */

	/* DSAPlugin>>#addBackDivisorDigitShift: */
static sqInt
addBackDivisorDigitShift(sqInt digitShift)
{
    sqInt carry;
    sqInt i;
    sqInt rIndex;
    sqInt sum;

	carry = 0;
	rIndex = digitShift + 1;
	for (i = 1; i <= divisorDigitCount; i += 1) {
		sum = ((dsaRemainder[rIndex]) + (dsaDivisor[i])) + carry;
		dsaRemainder[rIndex] = (sum & 0xFF);
		carry = ((usqInt) sum >> 8);
		rIndex += 1;
	}
	sum = (dsaRemainder[rIndex]) + carry;
	dsaRemainder[rIndex] = (sum & 0xFF);
	return 0;
}


/*	This is the core of the divide algorithm. This loop steps through the
	digit positions of the quotient, each time estimating the right quotient
	digit, subtracting from the remainder the divisor times the quotient digit
	shifted left by the appropriate number of digits. When the loop
	terminates, all digits of the quotient have been filled in and the
	remainder contains a value less than the divisor. The tricky bit is
	estimating the next quotient digit. Knuth shows that the digit estimate
	computed here will never be less than it should be and cannot be more than
	one over what it should be. Furthermore, the case where the estimate is
	one too large is extremely rare. For example, in a typical test of 100000
	random 60-bit division problems, the rare case only occured five times.
	See Knuth, volume 2 ('Semi-Numerical Algorithms') 2nd edition, pp. 257-260
 */
/*	extract the top two digits of the divisor */

	/* DSAPlugin>>#bigDivideLoop */
static sqInt
bigDivideLoop(void)
{
    sqInt borrow;
    sqInt carry;
    unsigned char d1;
    unsigned char d2;
    sqInt digitShift;
    unsigned char firstDigit;
    sqInt firstTwoDigits;
    sqInt i;
    sqInt i1;
    sqInt j;
    sqInt prod;
    sqInt q;
    sqInt qTooBig;
    sqInt resultDigit;
    sqInt rIndex;
    sqInt rIndex1;
    sqInt sum;
    unsigned char thirdDigit;

	d1 = dsaDivisor[divisorDigitCount];
	d2 = dsaDivisor[divisorDigitCount - 1];
	for (j = remainderDigitCount; j >= (divisorDigitCount + 1); j += -1) {

		/* extract the top several digits of remainder. */
		firstDigit = dsaRemainder[j];
		firstTwoDigits = (((usqInt) firstDigit << 8)) + (dsaRemainder[j - 1]);

		/* estimate q, the next digit of the quotient */
		thirdDigit = dsaRemainder[j - 2];
		if (firstDigit == d1) {
			q = 0xFF;
		}
		else {
			q = firstTwoDigits / d1;
		}
		if ((d2 * q) > ((((usqInt) (firstTwoDigits - (q * d1)) << 8)) + thirdDigit)) {
			q -= 1;
			if ((d2 * q) > ((((usqInt) (firstTwoDigits - (q * d1)) << 8)) + thirdDigit)) {
				q -= 1;
			}
		}
		digitShift = (j - divisorDigitCount) - 1;
		if (q > 0) {
			/* begin subtractDivisorMultipliedByDigit:digitShift: */
			borrow = 0;
			rIndex1 = digitShift + 1;
			for (i1 = 1; i1 <= divisorDigitCount; i1 += 1) {
				prod = ((dsaDivisor[i1]) * q) + borrow;
				borrow = ((usqInt) prod >> 8);
				resultDigit = (dsaRemainder[rIndex1]) - (prod & 0xFF);
				if (resultDigit < 0) {

					/* borrow from the next digit */
					resultDigit += 256;
					borrow += 1;
				}
				dsaRemainder[rIndex1] = resultDigit;
				rIndex1 += 1;
			}
			if (borrow == 0) {
				qTooBig = 0;
				goto l1;
			}
			resultDigit = (dsaRemainder[rIndex1]) - borrow;
			if (resultDigit < 0) {

				/* digit was too large (this case is quite rare) */
				dsaRemainder[rIndex1] = (resultDigit + 256);
				qTooBig = 1;
				goto l1;
			}
			else {
				dsaRemainder[rIndex1] = resultDigit;
				qTooBig = 0;
				goto l1;
			}
	l1:	/* end subtractDivisorMultipliedByDigit:digitShift: */;
			if (qTooBig) {

				/* this case is extremely rare */
				/* begin addBackDivisorDigitShift: */
				carry = 0;
				rIndex = digitShift + 1;
				for (i = 1; i <= divisorDigitCount; i += 1) {
					sum = ((dsaRemainder[rIndex]) + (dsaDivisor[i])) + carry;
					dsaRemainder[rIndex] = (sum & 0xFF);
					carry = ((usqInt) sum >> 8);
					rIndex += 1;
				}
				sum = (dsaRemainder[rIndex]) + carry;
				dsaRemainder[rIndex] = (sum & 0xFF);
				q -= 1;
			}
		}
		dsaQuotient[digitShift + 1] = q;
	}
	return 0;
}


/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

	/* InterpreterPlugin>>#getModuleName */
EXPORT(const char*)
getModuleName(void)
{
	return moduleName;
}


/*	Rotate the given 32-bit integer left by the given number of bits and
	answer the result.
 */

	/* DSAPlugin>>#leftRotate:by: */
static unsigned int
leftRotateby(unsigned int anInteger, sqInt bits)
{
	return (((usqInt)(anInteger) << bits)) | (((usqInt) anInteger) >> (32 - bits));
}


/*	Called with three LargePositiveInteger arguments, rem, div, quo. Divide
	div into rem and store the quotient into quo, leaving the remainder in
	rem. 
 */
/*	Assume: quo starts out filled with zeros. */

	/* DSAPlugin>>#primitiveBigDivide */
EXPORT(sqInt)
primitiveBigDivide(void)
{
    sqInt borrow;
    sqInt carry;
    sqInt clpi;
    unsigned char d1;
    unsigned char d2;
    sqInt digitShift;
    sqInt div;
    unsigned char firstDigit;
    sqInt firstTwoDigits;
    sqInt i;
    sqInt i1;
    sqInt j;
    sqInt prod;
    sqInt q;
    sqInt qTooBig;
    sqInt quo;
    sqInt rem;
    sqInt resultDigit;
    sqInt rIndex;
    sqInt rIndex1;
    sqInt sum;
    unsigned char thirdDigit;

	quo = stackValue(0);
	div = stackValue(1);
	rem = stackValue(2);
	clpi = classLargePositiveInteger();
	if (!(((fetchClassOf(rem)) == clpi)
		 && (((fetchClassOf(div)) == clpi)
		 && ((fetchClassOf(quo)) == clpi)))) {
		primitiveFailFor(PrimErrBadArgument);
	}
	dsaRemainder = firstIndexableField(rem);
	dsaDivisor = firstIndexableField(div);
	dsaQuotient = firstIndexableField(quo);
	divisorDigitCount = stSizeOf(div);

	/* adjust pointers for base-1 indexing */
	remainderDigitCount = stSizeOf(rem);
	dsaRemainder -= 1;
	dsaDivisor -= 1;
	dsaQuotient -= 1;
	/* begin bigDivideLoop */
	d1 = dsaDivisor[divisorDigitCount];
	d2 = dsaDivisor[divisorDigitCount - 1];
	for (j = remainderDigitCount; j >= (divisorDigitCount + 1); j += -1) {

		/* extract the top several digits of remainder. */
		firstDigit = dsaRemainder[j];
		firstTwoDigits = (((usqInt) firstDigit << 8)) + (dsaRemainder[j - 1]);

		/* estimate q, the next digit of the quotient */
		thirdDigit = dsaRemainder[j - 2];
		if (firstDigit == d1) {
			q = 0xFF;
		}
		else {
			q = firstTwoDigits / d1;
		}
		if ((d2 * q) > ((((usqInt) (firstTwoDigits - (q * d1)) << 8)) + thirdDigit)) {
			q -= 1;
			if ((d2 * q) > ((((usqInt) (firstTwoDigits - (q * d1)) << 8)) + thirdDigit)) {
				q -= 1;
			}
		}
		digitShift = (j - divisorDigitCount) - 1;
		if (q > 0) {
			/* begin subtractDivisorMultipliedByDigit:digitShift: */
			borrow = 0;
			rIndex1 = digitShift + 1;
			for (i1 = 1; i1 <= divisorDigitCount; i1 += 1) {
				prod = ((dsaDivisor[i1]) * q) + borrow;
				borrow = ((usqInt) prod >> 8);
				resultDigit = (dsaRemainder[rIndex1]) - (prod & 0xFF);
				if (resultDigit < 0) {

					/* borrow from the next digit */
					resultDigit += 256;
					borrow += 1;
				}
				dsaRemainder[rIndex1] = resultDigit;
				rIndex1 += 1;
			}
			if (borrow == 0) {
				qTooBig = 0;
				goto l1;
			}
			resultDigit = (dsaRemainder[rIndex1]) - borrow;
			if (resultDigit < 0) {

				/* digit was too large (this case is quite rare) */
				dsaRemainder[rIndex1] = (resultDigit + 256);
				qTooBig = 1;
				goto l1;
			}
			else {
				dsaRemainder[rIndex1] = resultDigit;
				qTooBig = 0;
				goto l1;
			}
	l1:	/* end subtractDivisorMultipliedByDigit:digitShift: */;
			if (qTooBig) {

				/* this case is extremely rare */
				/* begin addBackDivisorDigitShift: */
				carry = 0;
				rIndex = digitShift + 1;
				for (i = 1; i <= divisorDigitCount; i += 1) {
					sum = ((dsaRemainder[rIndex]) + (dsaDivisor[i])) + carry;
					dsaRemainder[rIndex] = (sum & 0xFF);
					carry = ((usqInt) sum >> 8);
					rIndex += 1;
				}
				sum = (dsaRemainder[rIndex]) + carry;
				dsaRemainder[rIndex] = (sum & 0xFF);
				q -= 1;
			}
		}
		dsaQuotient[digitShift + 1] = q;
	}
	pop(3);
	return 0;
}


/*	Multiple f1 by f2, placing the result into prod. f1, f2, and prod must be
	LargePositiveIntegers, and the length of prod must be the sum of the
	lengths of f1 and f2.
 */
/*	Assume: prod starts out filled with zeros */

	/* DSAPlugin>>#primitiveBigMultiply */
EXPORT(sqInt)
primitiveBigMultiply(void)
{
    sqInt carry;
    sqInt clpi;
    unsigned char digit;
    sqInt f1;
    sqInt f1Len;
    unsigned char *f1Ptr;
    sqInt f2;
    sqInt f2Len;
    unsigned char *f2Ptr;
    sqInt i;
    sqInt j;
    sqInt k;
    sqInt prod;
    sqInt prodLen;
    unsigned char *prodPtr;
    sqInt sum;

	prod = stackValue(0);
	f2 = stackValue(1);
	f1 = stackValue(2);
	clpi = classLargePositiveInteger();
	if (!(((fetchClassOf(prod)) == clpi)
		 && (((fetchClassOf(f2)) == clpi)
		 && ((fetchClassOf(f1)) == clpi)))) {
		primitiveFailFor(PrimErrBadArgument);
	}
	prodLen = stSizeOf(prod);
	f1Len = stSizeOf(f1);
	f2Len = stSizeOf(f2);
	if (!(prodLen == (f1Len + f2Len))) {
		primitiveFailFor(PrimErrBadArgument);
	}
	prodPtr = firstIndexableField(prod);
	f2Ptr = firstIndexableField(f2);
	f1Ptr = firstIndexableField(f1);
	for (i = 0; i < f1Len; i += 1) {
		if (((digit = f1Ptr[i])) != 0) {
			carry = 0;

			/* Loop invariants: 0 <= carry <= 16rFF, k = i + j - 1 */
			k = i;
			for (j = 0; j < f2Len; j += 1) {
				sum = (((f2Ptr[j]) * digit) + (prodPtr[k])) + carry;
				carry = ((usqInt) sum >> 8);
				prodPtr[k] = (sum & 0xFF);
				k += 1;
			}
			prodPtr[k] = carry;
		}
	}
	pop(3);
	return 0;
}


/*	Expand a 64 byte ByteArray (the first argument) into and an Bitmap of 80
	32-bit words (the second argument). When reading a 32-bit integer from the
	ByteArray, consider the first byte to contain the most significant bits of
	the word (i.e., use big-endian byte ordering).
 */

	/* DSAPlugin>>#primitiveExpandBlock */
EXPORT(sqInt)
primitiveExpandBlock(void)
{
    sqInt buf;
    unsigned char *bytePtr;
    sqInt expanded;
    sqInt i;
    sqInt src;
    usqInt v;
    unsigned int *wordPtr;

	expanded = stackValue(0);
	buf = stackValue(1);
	if (!((isWords(expanded))
		 && ((isBytes(buf))
		 && (((stSizeOf(expanded)) == 80)
		 && ((stSizeOf(buf)) == 64))))) {
		primitiveFailFor(PrimErrBadArgument);
	}
	wordPtr = firstIndexableField(expanded);
	bytePtr = firstIndexableField(buf);
	src = 0;
	for (i = 0; i <= 15; i += 1) {
		v = (((((usqInt)((bytePtr[src])) << 24)) + (((usqInt)((bytePtr[src + 1])) << 16))) + (((usqInt)((bytePtr[src + 2])) << 8))) + (bytePtr[src + 3]);
		wordPtr[i] = v;
		src += 4;
	}
	for (i = 16; i <= 79; i += 1) {
		v = (((wordPtr[i - 3]) ^ (wordPtr[i - 8])) ^ (wordPtr[i - 14])) ^ (wordPtr[i - 16]);
		/* begin leftRotate:by: */
		v = ((unsigned int) ((((usqInt)((((unsigned int) v))) << 1)) | (((usqInt) (((unsigned int) v))) >> (0x1F))));
		wordPtr[i] = v;
	}
	pop(2);
	return 0;
}


/*	Hash a Bitmap of 80 32-bit words (the first argument), using the given
	state (the second argument).
 */

	/* DSAPlugin>>#primitiveHashBlock */
EXPORT(sqInt)
primitiveHashBlock(void)
{
    unsigned int a;
    unsigned int b;
    sqInt buf;
    unsigned int *bufPtr;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    sqInt i;
    sqInt state;
    unsigned int *statePtr;
    unsigned int tmp;

	state = stackValue(0);
	buf = stackValue(1);
	if (!((isWords(state))
		 && ((isWords(buf))
		 && (((stSizeOf(state)) == 5)
		 && ((stSizeOf(buf)) == 80))))) {
		primitiveFailFor(PrimErrBadArgument);
	}
	statePtr = firstIndexableField(state);
	bufPtr = firstIndexableField(buf);
	a = statePtr[0];
	b = statePtr[1];
	c = statePtr[2];
	d = statePtr[3];
	e = statePtr[4];
	for (i = 0; i <= 19; i += 1) {
		tmp = (((1518500249 + ((b & c) | (((unsigned int)~b) & d))) + ((((usqInt)(a) << 5)) | (((usqInt) a) >> (27)))) + e) + (bufPtr[i]);
		e = d;
		d = c;
		/* begin leftRotate:by: */
		c = (((usqInt)(b) << 30)) | (((usqInt) b) >> (2));
		b = a;
		a = tmp;
	}
	for (i = 20; i <= 39; i += 1) {
		tmp = (((1859775393 + ((b ^ c) ^ d)) + ((((usqInt)(a) << 5)) | (((usqInt) a) >> (27)))) + e) + (bufPtr[i]);
		e = d;
		d = c;
		/* begin leftRotate:by: */
		c = (((usqInt)(b) << 30)) | (((usqInt) b) >> (2));
		b = a;
		a = tmp;
	}
	for (i = 40; i <= 59; i += 1) {
		tmp = (((2400959708U + (((b & c) | (b & d)) | (c & d))) + ((((usqInt)(a) << 5)) | (((usqInt) a) >> (27)))) + e) + (bufPtr[i]);
		e = d;
		d = c;
		/* begin leftRotate:by: */
		c = (((usqInt)(b) << 30)) | (((usqInt) b) >> (2));
		b = a;
		a = tmp;
	}
	for (i = 60; i <= 79; i += 1) {
		tmp = (((3395469782U + ((b ^ c) ^ d)) + ((((usqInt)(a) << 5)) | (((usqInt) a) >> (27)))) + e) + (bufPtr[i]);
		e = d;
		d = c;
		/* begin leftRotate:by: */
		c = (((usqInt)(b) << 30)) | (((usqInt) b) >> (2));
		b = a;
		a = tmp;
	}
	statePtr[0] = ((statePtr[0]) + a);
	statePtr[1] = ((statePtr[1]) + b);
	statePtr[2] = ((statePtr[2]) + c);
	statePtr[3] = ((statePtr[3]) + d);
	statePtr[4] = ((statePtr[4]) + e);
	pop(2);
	return 0;
}


/*	Answer true if the secure hash primitive is implemented. */

	/* DSAPlugin>>#primitiveHasSecureHashPrimitive */
EXPORT(sqInt)
primitiveHasSecureHashPrimitive(void)
{
	pop(1);
	pushBool(1);
	return 0;
}


/*	Called with one LargePositiveInteger argument. Answer the index of the
	top-most non-zero digit.
 */

	/* DSAPlugin>>#primitiveHighestNonZeroDigitIndex */
EXPORT(sqInt)
primitiveHighestNonZeroDigitIndex(void)
{
    sqInt arg;
    unsigned char *bigIntPtr;
    sqInt i;

	arg = stackValue(0);
	if (!((fetchClassOf(arg)) == (classLargePositiveInteger()))) {
		primitiveFailFor(PrimErrBadArgument);
	}
	bigIntPtr = firstIndexableField(arg);
	i = stSizeOf(arg);
	while ((i > 0)
	 && ((bigIntPtr[(i -= 1)]) == 0)) {
	}
	pop(1);
	pushInteger(i + 1);
	return 0;
}


/*	Note: This is coded so that it can be run in Squeak. */

	/* InterpreterPlugin>>#setInterpreter: */
EXPORT(sqInt)
setInterpreter(struct VirtualMachine *anInterpreter)
{
    sqInt ok;


	/* This may seem tautological, but in a real plugin it checks that the VM provides
	   the version the plugin was compiled against which is the version the plugin expects. */
	interpreterProxy = anInterpreter;
	ok = ((interpreterProxy->majorVersion()) == (VM_PROXY_MAJOR))
	 && ((interpreterProxy->minorVersion()) >= (VM_PROXY_MINOR));
	if (ok) {
		
#if !defined(SQUEAK_BUILTIN_PLUGIN)
		classLargePositiveInteger = interpreterProxy->classLargePositiveInteger;
		fetchClassOf = interpreterProxy->fetchClassOf;
		firstIndexableField = interpreterProxy->firstIndexableField;
		isBytes = interpreterProxy->isBytes;
		isWords = interpreterProxy->isWords;
		pop = interpreterProxy->pop;
		primitiveFailFor = interpreterProxy->primitiveFailFor;
		pushBool = interpreterProxy->pushBool;
		pushInteger = interpreterProxy->pushInteger;
		stSizeOf = interpreterProxy->stSizeOf;
		stackValue = interpreterProxy->stackValue;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}


/*	Multiply the divisor by the given digit (an integer in the range 0..255),
	shift it left by the given number of digits, and subtract the result from
	the current remainder. Answer true if there is an excess borrow,
	indicating that digit was one too large. (This case is quite rare.)
 */

	/* DSAPlugin>>#subtractDivisorMultipliedByDigit:digitShift: */
static sqInt
subtractDivisorMultipliedByDigitdigitShift(sqInt digit, sqInt digitShift)
{
    sqInt borrow;
    sqInt i;
    sqInt prod;
    sqInt resultDigit;
    sqInt rIndex;

	borrow = 0;
	rIndex = digitShift + 1;
	for (i = 1; i <= divisorDigitCount; i += 1) {
		prod = ((dsaDivisor[i]) * digit) + borrow;
		borrow = ((usqInt) prod >> 8);
		resultDigit = (dsaRemainder[rIndex]) - (prod & 0xFF);
		if (resultDigit < 0) {

			/* borrow from the next digit */
			resultDigit += 256;
			borrow += 1;
		}
		dsaRemainder[rIndex] = resultDigit;
		rIndex += 1;
	}
	if (borrow == 0) {
		return 0;
	}
	resultDigit = (dsaRemainder[rIndex]) - borrow;
	if (resultDigit < 0) {

		/* digit was too large (this case is quite rare) */
		dsaRemainder[rIndex] = (resultDigit + 256);
		return 1;
	}
	else {
		dsaRemainder[rIndex] = resultDigit;
		return 0;
	}
}


#ifdef SQUEAK_BUILTIN_PLUGIN

static char _m[] = "DSAPrims";
void* DSAPrims_exports[][3] = {
	{(void*)_m, "getModuleName", (void*)getModuleName},
	{(void*)_m, "primitiveBigDivide\000\001", (void*)primitiveBigDivide},
	{(void*)_m, "primitiveBigMultiply\000\001", (void*)primitiveBigMultiply},
	{(void*)_m, "primitiveExpandBlock\000\001", (void*)primitiveExpandBlock},
	{(void*)_m, "primitiveHashBlock\000\001", (void*)primitiveHashBlock},
	{(void*)_m, "primitiveHasSecureHashPrimitive\000\377", (void*)primitiveHasSecureHashPrimitive},
	{(void*)_m, "primitiveHighestNonZeroDigitIndex\000\001", (void*)primitiveHighestNonZeroDigitIndex},
	{(void*)_m, "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};

#else /* ifdef SQ_BUILTIN_PLUGIN */

signed char primitiveBigDivideAccessorDepth = 1;
signed char primitiveBigMultiplyAccessorDepth = 1;
signed char primitiveExpandBlockAccessorDepth = 1;
signed char primitiveHashBlockAccessorDepth = 1;
signed char primitiveHighestNonZeroDigitIndexAccessorDepth = 1;

#endif /* ifdef SQ_BUILTIN_PLUGIN */
