/*
 (c) Niklas K. Voss
*/

#ifndef _IVY_GLOBAL_H_
#define _IVY_GLOBAL_H_

#include <mutex>

namespace ivy
{
	namespace global
	{
		extern std::mutex cout_mutex;
	}
}

#endif