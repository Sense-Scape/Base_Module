#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

/*
*
	Every chunk gets allocated its own header file

	These are then inlcuded here and are built
	into an exectuable

	This should then be run to check the chunk
	functionality
*/

// B
#include "BaseChunkTests.h"

// C
#include "ChunkDuplicatorUtilityTests.h"
#include "GPSChunkTests.h"

// D
#include "DetectionBinChunkTests.h"
#include "DirectionBinChunkTests.h"

// F
#include "FFTChunkTests.h"

// J
#include "JSONChunkTests.h"

// Q
#include "QueueLengthChunkTests.h"

// S
#include "SessionControllerTests.h"

// T
#include "TimeChunkTests.h"

// W
#include "WAVChunkTests.h"
#include "WatchdogChunkTests.h"
