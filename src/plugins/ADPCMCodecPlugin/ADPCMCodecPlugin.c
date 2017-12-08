/* Automatically generated by
	VMPluginCodeGenerator VMMaker.oscog-eem.2292 uuid: be5cc6cd-dd15-479b-903e-8dadd3746d9b
   from
	ADPCMCodecPlugin VMMaker.oscog-eem.2292 uuid: be5cc6cd-dd15-479b-903e-8dadd3746d9b
	ADPCMCodec Sound-pre.64 uuid: 1e82fad7-2c1d-ea42-9b55-cfa86185e99b
 */
static char __buildInfo[] = "ADPCMCodecPlugin VMMaker.oscog-eem.2292 uuid: be5cc6cd-dd15-479b-903e-8dadd3746d9b\n\
ADPCMCodec Sound-pre.64 uuid: 1e82fad7-2c1d-ea42-9b55-cfa86185e99b " __DATE__ ;



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
#endif

#include "sqMemoryAccess.h"


/*** Function Prototypes ***/
EXPORT(const char*) getModuleName(void);
EXPORT(sqInt) primitiveDecodeMono(void);
EXPORT(sqInt) primitiveDecodeStereo(void);
EXPORT(sqInt) primitiveEncodeMono(void);
EXPORT(sqInt) primitiveEncodeStereo(void);
EXPORT(sqInt) setInterpreter(struct VirtualMachine*anInterpreter);


/*** Variables ***/

#if !defined(SQUEAK_BUILTIN_PLUGIN)
static sqInt (*failed)(void);
static void * (*fetchArrayofObject)(sqInt fieldIndex, sqInt objectPointer);
static sqInt (*fetchIntegerofObject)(sqInt fieldIndex, sqInt objectPointer);
static sqInt (*pop)(sqInt nItems);
static sqInt (*stackIntegerValue)(sqInt offset);
static sqInt (*stackValue)(sqInt offset);
static sqInt (*storeIntegerofObjectwithValue)(sqInt index, sqInt oop, sqInt integer);
static sqInt (*success)(sqInt aBoolean);
#else /* !defined(SQUEAK_BUILTIN_PLUGIN) */
extern sqInt failed(void);
extern void * fetchArrayofObject(sqInt fieldIndex, sqInt objectPointer);
extern sqInt fetchIntegerofObject(sqInt fieldIndex, sqInt objectPointer);
extern sqInt pop(sqInt nItems);
extern sqInt stackIntegerValue(sqInt offset);
extern sqInt stackValue(sqInt offset);
extern sqInt storeIntegerofObjectwithValue(sqInt index, sqInt oop, sqInt integer);
extern sqInt success(sqInt aBoolean);
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"ADPCMCodecPlugin VMMaker.oscog-eem.2292 (i)"
#else
	"ADPCMCodecPlugin VMMaker.oscog-eem.2292 (e)"
#endif
;



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

	/* ADPCMCodec>>#primitiveDecodeMono */
EXPORT(sqInt)
primitiveDecodeMono(void)
{
    sqInt bit;
    sqInt bitPosition;
    sqInt bitsPerSample;
    sqInt byteIndex;
    sqInt count;
    sqInt currentByte;
    sqInt delta;
    sqInt deltaSignMask;
    sqInt deltaValueHighBit;
    sqInt deltaValueMask;
    unsigned char *encodedBytes;
    sqInt frameSizeMask;
    sqInt i;
    sqInt index;
    short int *indexTable;
    sqInt predicted;
    sqInt predictedDelta;
    sqInt rcvr;
    sqInt remaining;
    sqInt remaining1;
    sqInt remaining2;
    sqInt result;
    sqInt result1;
    sqInt result2;
    sqInt sampleIndex;
    short int *samples;
    sqInt shift;
    sqInt shift1;
    sqInt shift2;
    short int step;
    short int *stepSizeTable;

	delta = 0;
	rcvr = stackValue(1);
	count = stackIntegerValue(0);
	predicted = fetchIntegerofObject(0, rcvr);
	index = fetchIntegerofObject(1, rcvr);
	deltaSignMask = fetchIntegerofObject(2, rcvr);
	deltaValueMask = fetchIntegerofObject(3, rcvr);
	deltaValueHighBit = fetchIntegerofObject(4, rcvr);
	frameSizeMask = fetchIntegerofObject(5, rcvr);
	currentByte = fetchIntegerofObject(6, rcvr);
	bitPosition = fetchIntegerofObject(7, rcvr);
	byteIndex = fetchIntegerofObject(8, rcvr);
	encodedBytes = fetchArrayofObject(9, rcvr);
	encodedBytes -= 1;
	samples = fetchArrayofObject(10, rcvr);
	samples -= 1;
	sampleIndex = fetchIntegerofObject(12, rcvr);
	bitsPerSample = fetchIntegerofObject(13, rcvr);
	stepSizeTable = fetchArrayofObject(14, rcvr);
	stepSizeTable -= 1;
	indexTable = fetchArrayofObject(15, rcvr);
	indexTable -= 1;
	if (failed()) {
		return null;
	}
	for (i = 1; i <= count; i += 1) {
		if ((i & frameSizeMask) == 1) {

			/* start of frame; read frame header */
			/* begin nextBits: */
			result = 0;
			remaining = 16;
			while(1) {
				shift = remaining - bitPosition;
				if (shift > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result += ((sqInt)((usqInt)(currentByte) << shift));
					remaining -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result += ((usqInt) currentByte) >> (0 - shift);

					/* mask out the consumed bits: */
					bitPosition -= remaining;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					predicted = result;
					goto l1;
				}
			}
	l1:	/* end nextBits: */;
			if (predicted > 0x7FFF) {
				predicted -= 65536;
			}
			/* begin nextBits: */
			result1 = 0;
			remaining1 = 6;
			while(1) {
				shift1 = remaining1 - bitPosition;
				if (shift1 > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result1 += ((sqInt)((usqInt)(currentByte) << shift1));
					remaining1 -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result1 += ((usqInt) currentByte) >> (0 - shift1);

					/* mask out the consumed bits: */
					bitPosition -= remaining1;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					index = result1;
					goto l2;
				}
			}
	l2:	/* end nextBits: */;
			samples[(sampleIndex += 1)] = predicted;
		}
		else {
			/* begin nextBits: */
			result2 = 0;
			remaining2 = bitsPerSample;
			while(1) {
				shift2 = remaining2 - bitPosition;
				if (shift2 > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result2 += ((sqInt)((usqInt)(currentByte) << shift2));
					remaining2 -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result2 += ((usqInt) currentByte) >> (0 - shift2);

					/* mask out the consumed bits: */
					bitPosition -= remaining2;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					delta = result2;
					goto l3;
				}
			}
	l3:	/* end nextBits: */;
			step = stepSizeTable[index + 1];
			predictedDelta = 0;
			bit = deltaValueHighBit;
			while (bit > 0) {
				if ((delta & bit) > 0) {
					predictedDelta += step;
				}
				step = ((usqInt) step >> 1);
				bit = ((usqInt) bit >> 1);
			}
			predictedDelta += step;
			if ((delta & deltaSignMask) > 0) {
				predicted -= predictedDelta;
			}
			else {
				predicted += predictedDelta;
			}
			if (predicted > 0x7FFF) {
				predicted = 0x7FFF;
			}
			else {
				if (predicted < -32768) {
					predicted = -32768;
				}
			}
			index += indexTable[(delta & deltaValueMask) + 1];
			if (index < 0) {
				index = 0;
			}
			else {
				if (index > 88) {
					index = 88;
				}
			}
			samples[(sampleIndex += 1)] = predicted;
		}
	}
	if (failed()) {
		return null;
	}
	storeIntegerofObjectwithValue(0, rcvr, predicted);
	storeIntegerofObjectwithValue(1, rcvr, index);
	storeIntegerofObjectwithValue(6, rcvr, currentByte);
	storeIntegerofObjectwithValue(7, rcvr, bitPosition);
	storeIntegerofObjectwithValue(8, rcvr, byteIndex);
	storeIntegerofObjectwithValue(12, rcvr, sampleIndex);
	pop(1);
	return 0;
}

	/* ADPCMCodec>>#primitiveDecodeStereo */
EXPORT(sqInt)
primitiveDecodeStereo(void)
{
    sqInt bit;
    sqInt bitPosition;
    sqInt bitsPerSample;
    sqInt byteIndex;
    sqInt count;
    sqInt currentByte;
    sqInt deltaLeft;
    sqInt deltaRight;
    sqInt deltaSignMask;
    sqInt deltaValueHighBit;
    sqInt deltaValueMask;
    unsigned char *encodedBytes;
    sqInt frameSizeMask;
    sqInt i;
    short int *index;
    sqInt indexLeft;
    sqInt indexRight;
    short int *indexTable;
    short int *predicted;
    sqInt predictedDeltaLeft;
    sqInt predictedDeltaRight;
    sqInt predictedLeft;
    sqInt predictedRight;
    sqInt rcvr;
    sqInt remaining;
    sqInt remaining1;
    sqInt remaining2;
    sqInt remaining3;
    sqInt remaining4;
    sqInt remaining5;
    sqInt result;
    sqInt result1;
    sqInt result2;
    sqInt result3;
    sqInt result4;
    sqInt result5;
    short int *rightSamples;
    sqInt sampleIndex;
    short int *samples;
    sqInt shift;
    sqInt shift1;
    sqInt shift2;
    sqInt shift3;
    sqInt shift4;
    sqInt shift5;
    short int stepLeft;
    short int stepRight;
    short int *stepSizeTable;


	/* make local copies of decoder state variables */
	deltaLeft = 0;
	deltaRight = 0;
	rcvr = stackValue(1);
	count = stackIntegerValue(0);
	predicted = fetchArrayofObject(0, rcvr);
	predicted -= 1;
	index = fetchArrayofObject(1, rcvr);
	index -= 1;
	deltaSignMask = fetchIntegerofObject(2, rcvr);
	deltaValueMask = fetchIntegerofObject(3, rcvr);
	deltaValueHighBit = fetchIntegerofObject(4, rcvr);
	frameSizeMask = fetchIntegerofObject(5, rcvr);
	currentByte = fetchIntegerofObject(6, rcvr);
	bitPosition = fetchIntegerofObject(7, rcvr);
	byteIndex = fetchIntegerofObject(8, rcvr);
	encodedBytes = fetchArrayofObject(9, rcvr);
	encodedBytes -= 1;
	samples = fetchArrayofObject(10, rcvr);
	samples -= 1;
	rightSamples = fetchArrayofObject(11, rcvr);
	rightSamples -= 1;
	sampleIndex = fetchIntegerofObject(12, rcvr);
	bitsPerSample = fetchIntegerofObject(13, rcvr);
	stepSizeTable = fetchArrayofObject(14, rcvr);
	stepSizeTable -= 1;
	indexTable = fetchArrayofObject(15, rcvr);
	indexTable -= 1;
	if (failed()) {
		return null;
	}
	predictedLeft = predicted[1];
	predictedRight = predicted[2];
	indexLeft = index[1];
	indexRight = index[2];
	for (i = 1; i <= count; i += 1) {
		if ((i & frameSizeMask) == 1) {

			/* start of frame; read frame header */
			/* begin nextBits: */
			result = 0;
			remaining = 16;
			while(1) {
				shift = remaining - bitPosition;
				if (shift > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result += ((sqInt)((usqInt)(currentByte) << shift));
					remaining -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result += ((usqInt) currentByte) >> (0 - shift);

					/* mask out the consumed bits: */
					bitPosition -= remaining;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					predictedLeft = result;
					goto l1;
				}
			}
	l1:	/* end nextBits: */;
			/* begin nextBits: */
			result1 = 0;
			remaining1 = 6;
			while(1) {
				shift1 = remaining1 - bitPosition;
				if (shift1 > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result1 += ((sqInt)((usqInt)(currentByte) << shift1));
					remaining1 -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result1 += ((usqInt) currentByte) >> (0 - shift1);

					/* mask out the consumed bits: */
					bitPosition -= remaining1;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					indexLeft = result1;
					goto l2;
				}
			}
	l2:	/* end nextBits: */;
			/* begin nextBits: */
			result2 = 0;
			remaining2 = 16;
			while(1) {
				shift2 = remaining2 - bitPosition;
				if (shift2 > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result2 += ((sqInt)((usqInt)(currentByte) << shift2));
					remaining2 -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result2 += ((usqInt) currentByte) >> (0 - shift2);

					/* mask out the consumed bits: */
					bitPosition -= remaining2;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					predictedRight = result2;
					goto l3;
				}
			}
	l3:	/* end nextBits: */;
			/* begin nextBits: */
			result3 = 0;
			remaining3 = 6;
			while(1) {
				shift3 = remaining3 - bitPosition;
				if (shift3 > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result3 += ((sqInt)((usqInt)(currentByte) << shift3));
					remaining3 -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result3 += ((usqInt) currentByte) >> (0 - shift3);

					/* mask out the consumed bits: */
					bitPosition -= remaining3;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					indexRight = result3;
					goto l4;
				}
			}
	l4:	/* end nextBits: */;
			if (predictedLeft > 0x7FFF) {
				predictedLeft -= 65536;
			}
			if (predictedRight > 0x7FFF) {
				predictedRight -= 65536;
			}
			samples[(sampleIndex += 1)] = predictedLeft;
			rightSamples[sampleIndex] = predictedRight;
		}
		else {
			/* begin nextBits: */
			result4 = 0;
			remaining4 = bitsPerSample;
			while(1) {
				shift4 = remaining4 - bitPosition;
				if (shift4 > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result4 += ((sqInt)((usqInt)(currentByte) << shift4));
					remaining4 -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result4 += ((usqInt) currentByte) >> (0 - shift4);

					/* mask out the consumed bits: */
					bitPosition -= remaining4;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					deltaLeft = result4;
					goto l5;
				}
			}
	l5:	/* end nextBits: */;
			/* begin nextBits: */
			result5 = 0;
			remaining5 = bitsPerSample;
			while(1) {
				shift5 = remaining5 - bitPosition;
				if (shift5 > 0) {

					/* consumed currentByte buffer; fetch next byte */
					result5 += ((sqInt)((usqInt)(currentByte) << shift5));
					remaining5 -= bitPosition;
					currentByte = encodedBytes[(byteIndex += 1)];
					bitPosition = 8;
				}
				else {

					/* still some bits left in currentByte buffer */
					result5 += ((usqInt) currentByte) >> (0 - shift5);

					/* mask out the consumed bits: */
					bitPosition -= remaining5;
					currentByte = currentByte & (((usqInt) 0xFF) >> (8 - bitPosition));
					deltaRight = result5;
					goto l6;
				}
			}
	l6:	/* end nextBits: */;
			stepLeft = stepSizeTable[indexLeft + 1];
			stepRight = stepSizeTable[indexRight + 1];
			predictedDeltaLeft = (predictedDeltaRight = 0);
			bit = deltaValueHighBit;
			while (bit > 0) {
				if ((deltaLeft & bit) > 0) {
					predictedDeltaLeft += stepLeft;
				}
				if ((deltaRight & bit) > 0) {
					predictedDeltaRight += stepRight;
				}
				stepLeft = ((usqInt) stepLeft >> 1);
				stepRight = ((usqInt) stepRight >> 1);
				bit = ((usqInt) bit >> 1);
			}
			predictedDeltaLeft += stepLeft;
			predictedDeltaRight += stepRight;
			if ((deltaLeft & deltaSignMask) > 0) {
				predictedLeft -= predictedDeltaLeft;
			}
			else {
				predictedLeft += predictedDeltaLeft;
			}
			if ((deltaRight & deltaSignMask) > 0) {
				predictedRight -= predictedDeltaRight;
			}
			else {
				predictedRight += predictedDeltaRight;
			}
			if (predictedLeft > 0x7FFF) {
				predictedLeft = 0x7FFF;
			}
			else {
				if (predictedLeft < -32768) {
					predictedLeft = -32768;
				}
			}
			if (predictedRight > 0x7FFF) {
				predictedRight = 0x7FFF;
			}
			else {
				if (predictedRight < -32768) {
					predictedRight = -32768;
				}
			}
			indexLeft += indexTable[(deltaLeft & deltaValueMask) + 1];
			if (indexLeft < 0) {
				indexLeft = 0;
			}
			else {
				if (indexLeft > 88) {
					indexLeft = 88;
				}
			}
			indexRight += indexTable[(deltaRight & deltaValueMask) + 1];
			if (indexRight < 0) {
				indexRight = 0;
			}
			else {
				if (indexRight > 88) {
					indexRight = 88;
				}
			}
			samples[(sampleIndex += 1)] = predictedLeft;
			rightSamples[sampleIndex] = predictedRight;
		}
	}
	predicted[1] = predictedLeft;
	predicted[2] = predictedRight;
	index[1] = indexLeft;
	index[2] = indexRight;
	if (failed()) {
		return null;
	}
	storeIntegerofObjectwithValue(6, rcvr, currentByte);
	storeIntegerofObjectwithValue(7, rcvr, bitPosition);
	storeIntegerofObjectwithValue(8, rcvr, byteIndex);
	storeIntegerofObjectwithValue(12, rcvr, sampleIndex);
	pop(1);
	return 0;
}

	/* ADPCMCodec>>#primitiveEncodeMono */
EXPORT(sqInt)
primitiveEncodeMono(void)
{
    sqInt bestIndex;
    sqInt bit;
    sqInt bitPosition;
    sqInt bitsAvailable;
    sqInt bitsAvailable1;
    sqInt bitsAvailable2;
    sqInt bitsPerSample;
    sqInt buf;
    sqInt buf1;
    sqInt buf2;
    sqInt bufBits;
    sqInt bufBits1;
    sqInt bufBits2;
    sqInt byteIndex;
    sqInt count;
    sqInt currentByte;
    sqInt delta;
    sqInt deltaSignMask;
    sqInt deltaValueHighBit;
    sqInt diff;
    sqInt diff1;
    unsigned char *encodedBytes;
    sqInt frameSizeMask;
    sqInt i;
    sqInt index;
    short int *indexTable;
    sqInt j;
    sqInt p;
    sqInt predicted;
    sqInt predictedDelta;
    sqInt rcvr;
    sqInt sampleIndex;
    short int *samples;
    sqInt shift;
    sqInt shift1;
    sqInt shift2;
    sqInt sign;
    short int step;
    short int *stepSizeTable;

	rcvr = stackValue(1);
	count = stackIntegerValue(0);
	predicted = fetchIntegerofObject(0, rcvr);
	index = fetchIntegerofObject(1, rcvr);
	deltaSignMask = fetchIntegerofObject(2, rcvr);
	deltaValueHighBit = fetchIntegerofObject(4, rcvr);
	frameSizeMask = fetchIntegerofObject(5, rcvr);
	currentByte = fetchIntegerofObject(6, rcvr);
	bitPosition = fetchIntegerofObject(7, rcvr);
	byteIndex = fetchIntegerofObject(8, rcvr);
	encodedBytes = fetchArrayofObject(9, rcvr);
	encodedBytes -= 1;
	samples = fetchArrayofObject(10, rcvr);
	samples -= 1;
	sampleIndex = fetchIntegerofObject(12, rcvr);
	bitsPerSample = fetchIntegerofObject(13, rcvr);
	stepSizeTable = fetchArrayofObject(14, rcvr);
	stepSizeTable -= 1;
	indexTable = fetchArrayofObject(15, rcvr);
	indexTable -= 1;
	if (failed()) {
		return null;
	}
	step = stepSizeTable[1];
	for (i = 1; i <= count; i += 1) {
		if ((i & frameSizeMask) == 1) {
			predicted = samples[(sampleIndex += 1)];
			if (((p = predicted)) < 0) {
				p += 65536;
			}
			/* begin nextBits:put: */
			buf = p;
			bufBits = 16;
			while(1) {
				bitsAvailable = 8 - bitPosition;

				/* either left or right shift */
				/* append high bits of buf to end of currentByte: */
				shift = bitsAvailable - bufBits;
				if (shift < 0) {

					/* currentByte buffer filled; output it */
					currentByte += ((usqInt) buf) >> (0 - shift);
					encodedBytes[(byteIndex += 1)] = currentByte;
					bitPosition = 0;

					/* clear saved high bits of buf: */
					currentByte = 0;
					buf = buf & ((1U << (0 - shift)) - 1);
					bufBits -= bitsAvailable;
				}
				else {

					/* still some bits available in currentByte buffer */
					currentByte += ((sqInt)((usqInt)(buf) << shift));
					bitPosition += bufBits;
					goto l2;
				}
			}
	l2:	/* end nextBits:put: */;
			if (i < count) {
				/* begin indexForDeltaFrom:to: */
				diff1 = (samples[sampleIndex + 1]) - predicted;
				if (diff1 < 0) {
					diff1 = 0 - diff1;
				}
				bestIndex = 0x3F;
				for (j = 1; j <= 0x3E; j += 1) {
					if (bestIndex == 0x3F) {
						if ((stepSizeTable[j]) >= diff1) {
							bestIndex = j;
						}
					}
				}
				index = bestIndex;
			}
			/* begin nextBits:put: */
			buf1 = index;
			bufBits1 = 6;
			while(1) {
				bitsAvailable1 = 8 - bitPosition;

				/* either left or right shift */
				/* append high bits of buf to end of currentByte: */
				shift1 = bitsAvailable1 - bufBits1;
				if (shift1 < 0) {

					/* currentByte buffer filled; output it */
					currentByte += ((usqInt) buf1) >> (0 - shift1);
					encodedBytes[(byteIndex += 1)] = currentByte;
					bitPosition = 0;

					/* clear saved high bits of buf: */
					currentByte = 0;
					buf1 = buf1 & ((1U << (0 - shift1)) - 1);
					bufBits1 -= bitsAvailable1;
				}
				else {

					/* still some bits available in currentByte buffer */
					currentByte += ((sqInt)((usqInt)(buf1) << shift1));
					bitPosition += bufBits1;
					goto l3;
				}
			}
	l3:	/* end nextBits:put: */;
		}
		else {

			/* compute sign and magnitude of difference from the predicted sample */
			sign = 0;
			diff = (samples[(sampleIndex += 1)]) - predicted;
			if (diff < 0) {
				sign = deltaSignMask;
				diff = 0 - diff;
			}
			delta = 0;
			predictedDelta = 0;
			bit = deltaValueHighBit;
			while (bit > 0) {
				if (diff >= step) {
					delta += bit;
					predictedDelta += step;
					diff -= step;
				}
				step = ((usqInt) step >> 1);
				bit = ((usqInt) bit >> 1);
			}

			/* compute and clamp new prediction */
			predictedDelta += step;
			if (sign > 0) {
				predicted -= predictedDelta;
			}
			else {
				predicted += predictedDelta;
			}
			if (predicted > 0x7FFF) {
				predicted = 0x7FFF;
			}
			else {
				if (predicted < -32768) {
					predicted = -32768;
				}
			}
			index += indexTable[delta + 1];
			if (index < 0) {
				index = 0;
			}
			else {
				if (index > 88) {
					index = 88;
				}
			}

			/* output encoded, signed delta */
			step = stepSizeTable[index + 1];
			/* begin nextBits:put: */
			buf2 = sign | delta;
			bufBits2 = bitsPerSample;
			while(1) {
				bitsAvailable2 = 8 - bitPosition;

				/* either left or right shift */
				/* append high bits of buf to end of currentByte: */
				shift2 = bitsAvailable2 - bufBits2;
				if (shift2 < 0) {

					/* currentByte buffer filled; output it */
					currentByte += ((usqInt) buf2) >> (0 - shift2);
					encodedBytes[(byteIndex += 1)] = currentByte;
					bitPosition = 0;

					/* clear saved high bits of buf: */
					currentByte = 0;
					buf2 = buf2 & ((1U << (0 - shift2)) - 1);
					bufBits2 -= bitsAvailable2;
				}
				else {

					/* still some bits available in currentByte buffer */
					currentByte += ((sqInt)((usqInt)(buf2) << shift2));
					bitPosition += bufBits2;
					goto l4;
				}
			}
	l4:	/* end nextBits:put: */;
		}
	}
	if (bitPosition > 0) {

		/* flush the last output byte, if necessary */
		encodedBytes[(byteIndex += 1)] = currentByte;
	}
	if (failed()) {
		return null;
	}
	storeIntegerofObjectwithValue(0, rcvr, predicted);
	storeIntegerofObjectwithValue(1, rcvr, index);
	storeIntegerofObjectwithValue(6, rcvr, currentByte);
	storeIntegerofObjectwithValue(7, rcvr, bitPosition);
	storeIntegerofObjectwithValue(8, rcvr, byteIndex);
	storeIntegerofObjectwithValue(12, rcvr, sampleIndex);
	pop(1);
	return 0;
}


/*	not yet implemented */

	/* ADPCMCodec>>#primitiveEncodeStereo */
EXPORT(sqInt)
primitiveEncodeStereo(void)
{
    sqInt count;
    sqInt rcvr;

	rcvr = stackValue(1);
	count = stackIntegerValue(0);
	if (failed()) {
		return null;
	}
	success(0);
	if (failed()) {
		return null;
	}
	pop(1);
	return 0;
}


/*	Note: This is coded so that it can be run in Squeak. */

	/* InterpreterPlugin>>#setInterpreter: */
EXPORT(sqInt)
setInterpreter(struct VirtualMachine*anInterpreter)
{
    sqInt ok;

	interpreterProxy = anInterpreter;
	ok = ((interpreterProxy->majorVersion()) == (VM_PROXY_MAJOR))
	 && ((interpreterProxy->minorVersion()) >= (VM_PROXY_MINOR));
	if (ok) {
		
#if !defined(SQUEAK_BUILTIN_PLUGIN)
		failed = interpreterProxy->failed;
		fetchArrayofObject = interpreterProxy->fetchArrayofObject;
		fetchIntegerofObject = interpreterProxy->fetchIntegerofObject;
		pop = interpreterProxy->pop;
		stackIntegerValue = interpreterProxy->stackIntegerValue;
		stackValue = interpreterProxy->stackValue;
		storeIntegerofObjectwithValue = interpreterProxy->storeIntegerofObjectwithValue;
		success = interpreterProxy->success;
#endif /* !defined(SQUEAK_BUILTIN_PLUGIN) */
	}
	return ok;
}


#ifdef SQUEAK_BUILTIN_PLUGIN

static char _m[] = "ADPCMCodecPlugin";
void* ADPCMCodecPlugin_exports[][3] = {
	{(void*)_m, "getModuleName", (void*)getModuleName},
	{(void*)_m, "primitiveDecodeMono\000\001", (void*)primitiveDecodeMono},
	{(void*)_m, "primitiveDecodeStereo\000\001", (void*)primitiveDecodeStereo},
	{(void*)_m, "primitiveEncodeMono\000\001", (void*)primitiveEncodeMono},
	{(void*)_m, "primitiveEncodeStereo\000\000", (void*)primitiveEncodeStereo},
	{(void*)_m, "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};

#else /* ifdef SQ_BUILTIN_PLUGIN */

signed char primitiveDecodeMonoAccessorDepth = 1;
signed char primitiveDecodeStereoAccessorDepth = 1;
signed char primitiveEncodeMonoAccessorDepth = 1;
signed char primitiveEncodeStereoAccessorDepth = 0;

#endif /* ifdef SQ_BUILTIN_PLUGIN */
