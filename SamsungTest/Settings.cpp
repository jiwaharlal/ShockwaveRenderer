#include "Settings.h"

#include <windows.h>
#include <stdlib.h>

Settings::Settings(void)
{
	myMinRadix = atoi(getValue("minRadix").c_str());
	myMaxRadix = atoi(getValue("maxRadix").c_str());
	myWaveSpeed = atoi(getValue("waveSpeed").c_str());
	myRadixAmplitudeRatio = static_cast<float>(atof(getValue("radixAmplitudeRatio").c_str()));
	myThreadCount = atoi(getValue("threadCount").c_str());
}

Settings::~Settings(void)
{
}

const Settings&
Settings::instance()
{
	static Settings s;

	return s;
}

int	
Settings::getMaxRadix() const 
{
	return myMaxRadix;
}

int	
Settings::getMinRadix() const 
{
	return myMinRadix;
}

int 
Settings::getWaveSpeed() const
{
	return myWaveSpeed;
}

float 
Settings::getRadixAmplitudeRatio() const
{
	return myRadixAmplitudeRatio;
}

int 
Settings::getThreadCount() const
{
	return myThreadCount;
}

std::string 
Settings::getValue(const char* aKey)
{
	char buffer[128];
	GetPrivateProfileStringA("shockwave", aKey, "", buffer, 128, "..\\SamsungTest.ini");

	return std::string(buffer);
}
