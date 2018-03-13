#pragma comment(lib, "Shlwapi.lib")
#include "Utils.h"
#include "Shlwapi.h"


CHAR Utils::ExeDir[MAX_PATH];

void Utils::SetPathToExecutableA()
{
	GetModuleFileNameA(NULL, ExeDir, MAX_PATH);
	PathRemoveFileSpecA(ExeDir);
}

void Utils::FullPathToFileA(CHAR * fileName)
{
	PathCombineA(fileName, ExeDir, fileName);
}

void Utils::Mix(double input, double &output)
{
	double value = input + output;
	if (value > 1.0) value = 1.0;
	else if (value < -1.0) value = -1.0;
	output = value;
}

void Utils::Mix(double inputs[2], double(&outputs)[2])
{
	Mix(inputs[0], outputs[0]);
	Mix(inputs[1], outputs[1]);
}

double Utils::Rms(double *buffer, size_t size)
{
	double sum = 0.0;

	for (size_t i = 0; i < size; i++)
	{
		sum += buffer[i] * buffer[i];
	}

	return sqrt(sum / size);
}
