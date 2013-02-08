/*
 (c) Niklas K. Voss
*/

#ifndef _IVY_ACCESS_H_
#define _IVY_ACCESS_H_

#include <boost/thread.hpp>

namespace ivy
{
	template <typename item_t, typename mutex_t>
	class ReaderProxy 
	{
	public:
  		ReaderProxy(item_t& i, mutex_t& m): lock(m), item(i) {}
  		item_t* operator->() 
  		{ 
  			return &item; 
  		}
	private:
  		boost::shared_lock<mutex_t> lock;
  		item_t& item;
	};

	template <typename item_t, typename mutex_t>
	class WriterProxy 
	{
	public:
		WriterProxy(item_t& i, mutex_t& m): lock_upgrade(m), lock(lock_upgrade), item(i) {}
  		item_t* operator->() 
  		{ 
  			return &item; 
  		}
	private:
  		boost::upgrade_lock<mutex_t> lock_upgrade;
  		boost::upgrade_to_unique_lock<mutex_t> lock;
  		item_t& item;
	};
}

#endif