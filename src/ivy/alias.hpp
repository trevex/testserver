/*
 (c) Niklas Voss
*/

#ifndef _IVY_ALIAS_H_
#define _IVY_ALIAS_H_

#define IVY_ALIAS(g, f) \
	template <typename... Args>	\
	auto g(Args&&... args) -> decltype(f(std::forward<Args>(args)...))	\
	{	\
  		return f(std::forward<Args>(args)...);	\
	}


#endif