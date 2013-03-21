#pragma once

#include <string>

class Settings
{
public:
	static Settings& instance();

	int				getMaxRadius() const;
	void			setMaxRadius(int aValue);
	int				getMinRadius() const;
	void			setMinRadius(int aValue);
	int				getWaveSpeed() const;
	void			setWaveSpeed(int aValue);
	float			getRadiusAmplitudeRatio() const;
	void			setRadiusAmplitudeRatio(float aValue);
	int				getThreadCount() const;
	void			setThreadCount(int aValue);
	bool			isBlastsParallel() const;
	void			setIsBlastsParallel(bool aValue);
	bool			isMultithreaded() const;
	void			setIsMultithreaded(bool aValue);

	void			saveSettings();
private:
	int			myMaxRadius;
	int			myMinRadius;
	int			myWaveSpeed;
	float		myRadiusAmplitudeRatio;
	int			myThreadCount;
	bool		myIsBlastsParallel;
	bool		myIsMultithreaded;

						Settings();
						~Settings();

	std::string			getValue(const char* aKey);
	void				saveValue(const char* aKey, const char* aValue);
};
