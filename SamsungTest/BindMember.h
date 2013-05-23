#pragma once

#include <memory>

#include "rl_ptr.h"

template <class ReturnType, class ValueType> class Functor;

template <class ReturnType, class ValueType>
class AFunctor
{
	friend class Functor<ReturnType, ValueType>;
public:
	virtual ~AFunctor() {};
	ReturnType operator () (ValueType aValue) {
		return Call(aValue);
	}
protected:
	virtual ReturnType Call(ValueType) = 0;
};

template <class ClassType, class ReturnType, class ValueType>
class MemberFunctorImpl: public AFunctor<ReturnType, ValueType>
{
	typedef ReturnType (ClassType::*FuncType)(ValueType);
public:
	MemberFunctorImpl(ClassType* aObject, FuncType aFunc)
		: myObject(aObject), myFunc(aFunc)
	{}
protected:
	virtual ReturnType Call(ValueType value)
	{
		return (myObject->*myFunc)(value);
	}
private:
	ClassType* myObject;
	FuncType myFunc;
};

template <class ReturnType, class ValueType>
class Functor {
public:
	Functor(AFunctor<ReturnType, ValueType>* aFunctorImpl) : myFunctorImpl(aFunctorImpl) {}
	~Functor() {}
	ReturnType operator () (ValueType value) {
		return myFunctorImpl->Call(value);
	}
private:
	koki::rl_ptr<AFunctor<ReturnType, ValueType> > myFunctorImpl;
};

template <class ClassType, class ReturnType, class ValueType>
Functor<ReturnType, ValueType> bindMember(ClassType* aObject, ReturnType (ClassType::*aFunc)(ValueType)) {
	return Functor<ReturnType, ValueType>(new MemberFunctorImpl<ClassType, ReturnType, ValueType>(aObject, aFunc));
}