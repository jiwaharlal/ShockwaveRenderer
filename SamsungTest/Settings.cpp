#include "Settings.h"

#include <windows.h>
#include <stdlib.h>

#include "BasicDefs.h"

Settings::Settings(void)
{
	myMinRadius = atoi(getValue("minRadius").c_str());
	myMaxRadius = atoi(getValue("maxRadius").c_str());
	myWaveSpeed = atoi(getValue("waveSpeed").c_str());
	myRadiusAmplitudeRatio = static_cast<float>(atof(getValue("radiusAmplitudeRatio").c_str()));
	myThreadCount = atoi(getValue("threadCount").c_str());
	myIsBlastsParallel = getValue("parallelBlasts") == "true";
	myIsMultithreaded = getValue("multithreaded") == "true";
}

Settings::~Settings(void)
{
}

Settings&
Settings::instance()
{
	static Settings s;

	return s;
}

int	
Settings::getMaxRadius() const 
{
	return myMaxRadius;
}

void
Settings::setMaxRadius(int aValue)
{
	myMaxRadius = aValue;
	saveValue("maxRadius", toStr(aValue).c_str());
}

int	
Settings::getMinRadius() const 
{
	return myMinRadius;
}

void
Settings::setMinRadius(int aValue)
{
	myMinRadius = aValue;
	saveValue("minRadius", toStr(aValue).c_str());
}

int 
Settings::getWaveSpeed() const
{
	return myWaveSpeed;
}

void
Settings::setWaveSpeed(int aValue)
{
	myWaveSpeed = aValue;
	saveValue("waveSpeed", toStr(aValue).c_str());
}

float 
Settings::getRadiusAmplitudeRatio() const
{
	return myRadiusAmplitudeRatio;
}

void
Settings::setRadiusAmplitudeRatio(float aValue)
{
	myRadiusAmplitudeRatio = aValue;
	saveValue("radiusAmplitudeRatio", toStr(aValue).c_str());
}

int 
Settings::getThreadCount() const
{
	return myThreadCount;
}

void
Settings::setThreadCount(int aValue)
{
	myThreadCount = aValue;
	saveValue("threadCount", toStr(aValue).c_str());
}

bool
Settings::isBlastsParallel() const
{
	return myIsBlastsParallel;
}

void
Settings::setIsBlastsParallel(bool aValue)
{
	myIsBlastsParallel = aValue;
	saveValue("parallelBlasts", aValue ? "true" : "false");
}

bool
Settings::isMultithreaded() const
{
	return myIsMultithreaded;
}

void
Settings::setIsMultithreaded(bool aValue)
{
	myIsMultithreaded = aValue;
	saveValue("multithreaded", aValue ? "true" : "false");
}

std::string 
Settings::getValue(const char* aKey)
{
	char buffer[128];
	GetPrivateProfileStringA("shockwave", aKey, "", buffer, 128, "..\\SamsungTest.ini");

	return std::string(buffer);
}

void
Settings::saveValue(const char* aKey, const char* aValue)
{
	WritePrivateProfileStringA("shockwave", aKey, aValue, "..\\SamsungTest.ini");
}
