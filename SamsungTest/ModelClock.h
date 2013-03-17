#ifndef MODEL_CLOCK_H
#define MODEL_CLOCK_H

// class defines interface for clock object, used by ModelEnvironment
// ModelClock is abstract, you may use SystemTimerClock or implement you own class
// with this interface
class ModelClock
{
public:
	ModelClock(void);
	virtual ~ModelClock(void);

	virtual double getTime() const = 0;
private:
	ModelClock(const ModelClock &other) {}
	const ModelClock &operator =(const ModelClock &other) {}
};

class SystemTimerClock : public ModelClock 
{
public:
	SystemTimerClock();
	virtual double getTime() const;
private:
	double timerFreq;
};

#endif // MODEL_CLOCK_H