#pragma once

#include "common.h"


class Utils
{
public:
	static CHAR ExeDir[];

public:
	Utils() = delete;
	~Utils() = delete;
	
	static void SetPathToExecutableA();
	static void FullPathToFileA(CHAR *fileName);

	static void Mix(double input, double &output);
	static void Mix(double inputs[2], double(&outputs)[2]);

	static double Rms(double *buffer, size_t size);
};

