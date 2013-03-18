#pragma once

#include <memory>

#include "rl_ptr.h"

template <class T> class Functor;

template <class ValueType>
class AFunctor
{
	friend class Functor<ValueType>;
public:
	virtual ~AFunctor() {};
	void operator () (ValueType aValue) {
		Call(aValue);
	}
protected:
	virtual void Call(ValueType) = 0;
};

template <class ClassType, class ValueType>
class MemberFunctorImpl: public AFunctor<ValueType>
{
	typedef void (ClassType::*FuncType)(ValueType);
public:
	MemberFunctorImpl(ClassType* aObject, FuncType aFunc)
		: myObject(aObject), myFunc(aFunc)
	{}
protected:
	virtual void Call(ValueType value)
	{
		(myObject->*myFunc)(value);
	}
private:
	ClassType* myObject;
	FuncType myFunc;
};

template <class ValueType>
class Functor {
public:
	Functor(AFunctor<ValueType>* aFunctorImpl) : myFunctorImpl(aFunctorImpl) {}
	~Functor() {}
	void operator () (ValueType value) {
		myFunctorImpl->Call(value);
	}
private:
	koki::rl_ptr<AFunctor<ValueType> > myFunctorImpl;
};

template <class ClassType, class ValueType>
Functor<ValueType> bindMember(ClassType* aObject, void (ClassType::*aFunc)(ValueType)) {
	return Functor<ValueType>(new MemberFunctorImpl<ClassType, ValueType>(aObject, aFunc));
}