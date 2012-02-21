
#ifndef _GIMBLE_COLLISIONWORLD_
#define _GIMBLE_COLLISIONWORLD_

#include "GimbleReqs.h"
#include "GimbleLinkedList.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API CollisionWorld
		{
		private:
			friend class CollisionObject;

		private:
			std::vector<CollisionObject*> mObjects;
			std::vector<ContactManifold*> mManifolds;
			LinkedList<ContactManifold*> *mManifoldLookup;
			uint mManifoldBuckets;

			Simulator *mSimulator;
			Broadphase *mBroadphase;
			MPR *mMpr;

		public:
			CollisionWorld(Broadphase *broadphase);
			~CollisionWorld();

			void addObject(CollisionObject *obj);
			void removeObject(CollisionObject *obj);

			void clear();

			void addContact(CollisionObject *a, CollisionObject *b, Contact *c);

			void buildContacts(float dt);

			const std::vector<ContactManifold*> &getManifolds();
			void clearManifolds();

		private:
			ContactManifold *findManifold(CollisionObject *a, CollisionObject *b);
			ContactManifold *getManifold(CollisionObject *a, CollisionObject *b);
			void removeManifoldLookup(CollisionObject *a, CollisionObject *b);
			void registerContact(CollisionObject *a, CollisionObject *b);
			void deregisterContact(CollisionObject *a, CollisionObject *b);
			bool islandActive(CollisionObject *obj, float threshold, ushort islandID);
			void deactivateIsland(CollisionObject *obj, ushort islandID);

			inline int hash(int key)
			{
				key += ~(key << 15);
				key ^=  (key >> 10);
				key +=  (key << 3);
				key ^=  (key >> 6);
				key += ~(key << 11);
				key ^=  (key >> 16);
				return key;
			}

			inline int hash(int id1, int id2)
			{
				return hash((id1&0xffff)|(id2<<16));
			}
		};
	}
}

#endif