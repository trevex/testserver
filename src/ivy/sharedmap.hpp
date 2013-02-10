/*
 (c) Niklas K. Voss
*/

#ifndef _IVY_SHAREDMAP_H_
#define _IVY_SHAREDMAP_H_

#include "access.hpp"
#include <map>

namespace ivy
{
    template<typename First, typename Second>
    class SharedMap
    {
    public:
        typedef std::map<First, Second> MapType;
        typedef ReaderProxy<MapType, boost::shared_mutex> Reader;
        typedef WriterProxy<MapType, boost::shared_mutex> Writer;


        bool find(First key) const
        {
            Reader r(m_map, m_mutex);
            auto it = r->find(key);
            if (it == r->end()) return false;
            return true;
        }

        Second get(First key) const
        {
            Reader r(m_map, m_mutex);
            auto it = r->find(key);
            if (it == r->end()) return Second();
            return it->second;
        }

        Second* getPtr(First key) const
        {
            Reader r(m_map, m_mutex);
            auto it = r->find(key);
            if (it == r->end()) return NULL;
            return &it->second;
        }

        void set(First key, Second value)
        {
            Writer w(m_map, m_mutex);
            w->insert(std::make_pair(key, value));
        }

    private:
        boost::shared_mutex m_mutex;
        MapType m_map;
    };

#endif