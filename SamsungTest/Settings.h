#pragma once

#include <string>

class Settings
{
public:
	static const Settings& instance();

	int				getMaxRadix() const;
	int				getMinRadix() const;
	int				getWaveSpeed() const;
	float			getRadixAmplitudeRatio() const;
	int				getThreadCount() const;
private:
	int			myMaxRadix;
	int			myMinRadix;
	int			myWaveSpeed;
	float		myRadixAmplitudeRatio;
	int			myThreadCount;

						Settings();
						~Settings();

	std::string			getValue(const char* aKey);
};
