// Logger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

#ifndef Logger_h
#define Logger_h

#ifdef UNREAL_ENGINE

#include "UnrealLogger.h"

#else

#include <stdio.h>

inline
void Log(const char* msg)
{	puts(msg);
}

inline
void LogError(const char* msg)
{	printf("ERROR: %s",msg);
}

#endif
#endif
