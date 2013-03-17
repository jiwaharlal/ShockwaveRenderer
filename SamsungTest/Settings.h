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
private:
	int			myMaxRadix;
	int			myMinRadix;
	int			myWaveSpeed;
	float		myRadixAmplitudeRatio;

						Settings();
						~Settings();

	std::string			getValue(const char* aKey);
};
