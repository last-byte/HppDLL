#include "pch.h"
#include "hppdll.h"

void PrintDebug(std::string output)
{
	std::ofstream outFile;
	outFile.open("C:\\debug.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	outFile << output << std::endl;
	outFile.close();
	return;
}