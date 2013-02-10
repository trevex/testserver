/*
 (c) Niklas K. Voss
*/
#ifndef _IVY_ROUTINE_H_
#define _IVY_ROUTINE_H_

#include <functional>

#include "alias.hpp"

namespace ivy
{
	namespace mt
 	{
 		bool isLockFreeQueue(void);
 		void stop(void);
 		int flush(void);
 		int getMapSize(void);
 	}

 	// functor type
 	typedef std::function<int(void)> routine_func;


 	/**
	[Q]ueue [R]outine
 	*/
 	void QueueRoutine(routine_func &f);
 	IVY_ALIAS(QR, QueueRoutine);

 	/**
 	[C]reate [R]outine
 	*/
	template <typename Func>
	void CreateRoutine(Func f)
	{
		routine_func* r = new routine_func(f);
		QR(*r);
	}
	IVY_ALIAS(CR, CreateRoutine);

	/**
	[R]outine [T]hread ("thread-main"-function)
	*/
 	int RoutineThread(void);
 	inline int RT(void) { return RoutineThread(); }
}

#endif