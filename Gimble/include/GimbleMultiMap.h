
#ifndef _GIMBLE_MULTIMAP_
#define _GIMBLE_MULTIMAP_

#include "GimbleReqs.h"

namespace Gimble
{
	template <typename K1, typename K2, typename V> class MultiMap2
	{
	private:
		typedef std::map<K2, V> InnerMap;
		typedef std::map<K1, InnerMap> Map;

		typedef std::pair<K2, V> InnerPair;
		typedef std::pair<K1, InnerMap> Pair;

	private:
		Map mMap;

	public:
		MultiMap2() {}
		~MultiMap2() {}

		void set(K1 k1, K2 k2, V v)
		{
			Map::iterator i1 = mMap.find(k1);

			if (i1 == mMap.end())
			{
				mMap.insert(Pair(k1, InnerMap()));
				mMap[k1].insert(InnerPair(k2, v));
				return;
			}

			InnerMap::iterator i2 = i1->second.find(k2);
			
			if (i2 == i1->second.end())
			{
				i1->second.insert(InnerPair(k2, v));
				return;
			}

			i1->second[k2] = v;
		}

		bool contains(K1 k1, K2 k2)
		{
			Map::iterator i1 = mMap.find(k1);

			if (i1 == mMap.end())
				return false;

			InnerMap::iterator i2 = i1->second.find(k2);
			
			if (i2 == i1->second.end())
				return false;
			
			return true;
		}

		V get(K1 k1, K2 k2)
		{
			return mMap[k1][k2];
		}

		void erase(K1 k1)
		{
			Map::iterator i1 = mMap.find(k1);
			if (i1 == mMap.end()) return;
			mMap.erase(i1);
		}

		void erase(K1 k1, K2 k2)
		{
			Map::iterator i1 = mMap.find(k1);
			if (i1 == mMap.end()) return;
			
			InnerMap::iterator i2 = i1->second.find(k2);
			if (i2 == i1->second.end()) return;

			i1->second.erase(i2);
		}

		void clear()
		{
			mMap.clear();
		}
	};

	template <typename K1, typename K2, typename K3, typename V> class MultiMap3
	{
	private:
		typedef std::map<K3, V> K3Map;
		typedef std::map<K2, K3Map> K2Map;
		typedef std::map<K1, K2Map> Map;

		typedef std::pair<K3, V> K3Pair;
		typedef std::pair<K2, K3Map> K2Pair;
		typedef std::pair<K1, K2Map> Pair;

	private:
		Map mMap;

	public:
		MultiMap3() {}
		~MultiMap3() {}

		void set(K1 k1, K2 k2, K3 k3, V v)
		{
			Map::iterator i1 = mMap.find(k1);

			if (i1 == mMap.end())
			{
				mMap.insert(Pair(k1, K2Map()));
				mMap[k1].insert(K2Pair(k2, K3Map()));
				mMap[k1][k2].insert(K3Pair(k3, v));
				return;
			}

			K2Map::iterator i2 = i1->second.find(k2);

			if (i2 == i1->second.end())
			{
				i1->second.insert(K2Pair(k2, K3Map()));
				mMap[k1][k2][k3] = v;
				return;
			}

			K3Map::iterator i3 = i2->second.find(k3);

			if (i3 == i2->second.end())
			{
				i2->second.insert(K3Pair(k3, v));
				return;
			}

			mMap[k1][k2][k3] = v;
		}

		bool contains(K1 k1, K2 k2, K3 k3)
		{
			Map::iterator i1 = mMap.find(k1);

			if (i1 == mMap.end())
				return false;

			K2Map::iterator i2 = i1->second.find(k2);

			if (i2 == i1->second.end())
				return false;

			K3Map::iterator i3 = i2->second.find(k3);

			if (i3 == i2->second.end())
				return false;

			return true;
		}

		V get(K1 k1, K2 k2, K3 k3)
		{
			return mMap[k1][k2][k3];
		}

		void erase(K1 k1)
		{
			Map::iterator i1 = mMap.find(k1);
			if (i1 == mMap.end()) return;
			mMap.erase(i1);
		}

		void erase(K1 k1, K2 k2)
		{
			Map::iterator i1 = mMap.find(k1);
			if (i1 == mMap.end()) return;
			
			K2Map::iterator i2 = i1->second.find(k2);
			if (i2 == i1->second.end()) return;

			i1->second.erase(i2);
		}

		void erase(K1 k1, K2 k2, K3 k3)
		{
			Map::iterator i1 = mMap.find(k1);
			if (i1 == mMap.end()) return;
			
			K2Map::iterator i2 = i1->second.find(k2);
			if (i2 == i1->second.end()) return;

			K3Map::iterator i3 = i2->second.find(k3);
			if (i3 == i2->second.end()) return;

			i2->erase(i3);
		}

		void clear()
		{
			mMap.clear();
		}
	};
}

#endif