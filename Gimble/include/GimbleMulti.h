
#ifndef _GIMBLE_MULTI_
#define _GIMBLE_MULTI_

namespace Gimble
{
	template <typename T> class Multi
	{
	protected:
		uint mCount;
		T **mElements;

	public:
		Multi() : mCount(0), mElements(0)
		{
			
		}

		~Multi()
		{
			if (mElements) 
				delete[] mElements;
			mElements = 0;
		}

		inline uint count() const { return mCount; }
		inline T** data() const { return mElements; }

		inline T *get(uint index) const
		{
			assert(index < mCount);

			if (index >= mCount)
				return 0;

			return mElements[index];
		}

		inline void add(T *element)
		{
			if (mCount == 0)
			{
				T **tElements = new T*[1];
				tElements[0] = element;

				mElements = tElements;
				mCount = 1;

				return;
			}

			T **tElements = new T*[mCount + 1];

			for (uint i = 0; i < mCount; i++)
				tElements[i] = mElements[i];

			tElements[mCount] = element;

			delete[] mElements;
			mElements = tElements;

			mCount++;
		}

		inline void clear()
		{
			if (mCount == 0)
				return;

			delete[] mElements;
			mElements = 0;
			mCount = 0;
		}
	};
}

#endif