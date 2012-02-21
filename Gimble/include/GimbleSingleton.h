
#ifndef _GIMBLE_SINGLETON_
#define _GIMBLE_SINGLETON_

#include "GimbleReqs.h"

#pragma warning (disable : 4311)
#pragma warning (disable : 4312)
#pragma warning (disable : 4661)

namespace Gimble
{
	template <typename T> class Singleton
    {
    protected:
        static T* ms_Singleton;

	private:
		Singleton(const Singleton<T> &);
		Singleton& operator=(const Singleton<T> &);

    public:
        Singleton()
        {
            assert(!ms_Singleton);
#if defined(_MSC_VER) && _MSC_VER < 1200	 
            int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
            ms_Singleton = (T*)((int)this + offset);
#else
			ms_Singleton = static_cast<T*>( this );
#endif
        }

        ~Singleton()
        {  
			assert(ms_Singleton);  
			ms_Singleton = 0;  
		}

        static T& get()
		{	
			assert(ms_Singleton);  
			return (*ms_Singleton); 
		}

        static T* getPtr()
		{ 
			return ms_Singleton; 
		}
    };
}

#endif