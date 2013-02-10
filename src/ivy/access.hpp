/*
 (c) Niklas K. Voss
*/

#ifndef _IVY_ACCESS_H_
#define _IVY_ACCESS_H_

#include <boost/thread.hpp>

namespace ivy
{
  template <typename Item, typename Mutex>
  class ReaderProxy {
  public:
	ReaderProxy(Item& i, Mutex& m): lock(m), item(i) {}
  
	Item* operator->() { return &item; }
  
  private:
	boost::shared_lock<Mutex> lock;
	Item& item;
  };
  
  template <typename Item, typename Mutex>
  class WriterProxy {
  public:
	WriterProxy(Item& i, Mutex& m): uplock(m), lock(uplock), item(i) {}
  
	Item* operator->() { return &item; }
  
  private:
	boost::upgrade_lock<Mutex> uplock;
	boost::upgrade_to_unique_lock<Mutex> lock;
	Item& item;
  };
}

#endif