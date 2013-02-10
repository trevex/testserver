/*
 (c) Niklas K. Voss
*/

#include <iostream>
#include <atomic>
#include <thread>
#include <memory>
#include <map>

#include <boost/lockfree/queue.hpp>
#include <boost/coroutine/all.hpp>


#include "routine.hpp"
#include "global.hpp"
#include "access.hpp"

#ifndef IVY_QUEUE_BLOCK_SIZE
#define IVY_QUEUE_BLOCK_SIZE 128
#endif

namespace ivy
{
	typedef boost::coroutines::coroutine<int(routine_func)> routine;
	typedef boost::lockfree::queue<routine_func*> routine_queue;
	
	std::atomic<bool> bRun(true);
	
	routine_queue queue(IVY_QUEUE_BLOCK_SIZE);
	
	namespace mt
 	{
 		bool isLockFreeQueue(void)
 		{
 			return queue.is_lock_free();
 		}
 		void stop(void)
 		{
 			bRun.store(false);
 		}
 		int flush(void)
 		{
 			int count = 0;
 			routine_func* value;
 			while (queue.pop(value)) count++;
 			return count;
 		}
 	}


 	void QueueRoutine(routine_func &f)
 	{
 		//routine* r = new routine(f);
 		while (!queue.push(&f)) ;
 	}

 	void CoFunction(routine::caller_type& caller)
 	{
 		routine_func f = caller.get();
 		caller(f());
 	}

 	int RoutineThread(void)
 	{
 		while(bRun.load())
 		{
 			routine_func* f = NULL;
 			while(!queue.pop(f))
 			{
 				if (!bRun.load()) break;
 			}
 			if (f != NULL)
 			{
 				routine r(CoFunction, *f);
 				int result = r.get();
 				if (result < 0) 
 				{
 					std::lock_guard<std::mutex> lock(global::cout_mutex);
 					std::cout << "Warning: " << std::this_thread::get_id() << " returned " << result << "." << std::endl;
 				}
 				delete f;
 			}
 		}
 		return 0;
 	}
}