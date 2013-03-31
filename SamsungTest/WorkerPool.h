#pragma once

#include <list>

#include "BasicDefs.h"

class WorkerPool
{
public:
					WorkerPool(void);
					~WorkerPool(void);

	void			setWorkerCount(int aCount);
	int				getWorkerCount();

	void			addTask(SHARED_PTR(CalculationTask) aTask);
private:
	std::list<SHARED_PTR(CalculationTask) >		myTaskList;
	std::vector< std::thread >					myWorkers;
};
