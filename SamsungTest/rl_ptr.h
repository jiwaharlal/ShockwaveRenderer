// rl_ptr.h
// 15.01.2004
// Author: Maxim Bondarenko
// Description: reference-linked pointer


#ifndef KOKI_RL_PTR_H_INCLUDED
#define KOKI_RL_PTR_H_INCLUDED


namespace koki
{

template < class T >
class rl_ptr
{
public:
	//rl_ptr() : pointee( NULL ), prevPtr( this ) {}
	rl_ptr( T* inPtr = 0 ) : pointee( inPtr ) { prevPtr = this; }
	
	rl_ptr( const rl_ptr< T >& rhs )
		: pointee( rhs.pointee ) 
		, prevPtr( &rhs )
	{	
		nextPtr( prevPtr )->prevPtr = this;
	}

	template < class U > rl_ptr( const rl_ptr< U >& rhs )
		: pointee( rhs.pointee ) 
		, prevPtr( reinterpret_cast< const rl_ptr<T>* >( &rhs ) )
	{	
		nextPtr( prevPtr )->prevPtr = this;
	}
	
	rl_ptr< T >& operator =( rl_ptr< T >& rhs )
	{
		if ( this != &rhs ) 
		{
			reset();
			pointee = rhs.pointee;
			prevPtr = &rhs;
			nextPtr( prevPtr )->prevPtr = this;
		}
		return *this;
	}
	
	template < class U > 
	rl_ptr< T >& operator =( rl_ptr< U >& rhs )
	{
		reset();
		pointee = rhs.pointee;
		prevPtr = reinterpret_cast< const rl_ptr< T >* >( &rhs );
		nextPtr( prevPtr )->prevPtr = this;

		return *this;
	}
	
	~rl_ptr()
	{
		reset();
	}		
			
	T& operator*() const { return *pointee; }
	T* operator->() const { return pointee; }
	T* get() const { return pointee; }
	
private: // functions
	const rl_ptr< T >* nextPtr( const rl_ptr< T >* iPtr)
	{
		const rl_ptr< T >* nextPtr = iPtr;
		while ( nextPtr->prevPtr != iPtr )
			nextPtr = nextPtr->prevPtr;
		return nextPtr;
	}
	
	void reset()
	{
		// if it's only pointer, remove pointed structure
		if ( prevPtr == this )
		{
			delete pointee;
		}
		else
		{
			// delete self from ring
			nextPtr( this )->prevPtr = prevPtr;
		}
	}
private: // data members
	T* pointee;
	mutable const rl_ptr< T >* prevPtr;
	
	//template < class U > friend class rl_ptr< U >;
};

} //namespace koki


#endif //  KOKI_RL_PTR_H_INCLUDED