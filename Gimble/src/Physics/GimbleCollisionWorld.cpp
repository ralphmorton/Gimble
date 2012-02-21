
#include "Physics/GimbleCollisionWorld.h"
#include "Physics/GimbleCollisionObject.h"
#include "Physics/GimbleRigidBody.h"
#include "Physics/GimbleContact.h"
#include "Physics/GimbleContactManifold.h"
#include "Physics/GimbleSimulator.h"
#include "Physics/GimbleBroadphase.h"
#include "Physics/GimbleSAP.h"
#include "Physics/GimbleMPR.h"
#include "GimbleQuaternion.h"

namespace Gimble
{
	namespace Physics
	{
		CollisionWorld::CollisionWorld(Broadphase *broadphase) : mManifoldBuckets(100), mManifoldLookup(new LinkedList<ContactManifold*>[100])
		{
			mBroadphase = broadphase;
			mMpr = new MPR(this, 128, 1e-3f);

			mSimulator = Simulator::getPtr();
		}

		CollisionWorld::~CollisionWorld()
		{
			for (int i = -1; ++i < mManifolds.size();)
				delete mManifolds[i];

			delete mMpr;
			delete mBroadphase;
			delete [] mManifoldLookup;
		}

		void CollisionWorld::addContact(CollisionObject *a, CollisionObject *b, Contact *c)
		{
			assert(a != b);

			ContactManifold *m = getManifold(a, b);

			if (m->getA() == b)
			{
				c->mNormal = -c->mNormal;
				Vector3 r1 = c->mRcp1;
				c->mRcp1 = c->mRcp2;
				c->mRcp2 = r1;
			}
			m->add(c);
		}

		void CollisionWorld::addObject(CollisionObject *obj)
		{
			GIMBLE_HANDLE handle = mBroadphase->add(obj);
			obj->mBroadphaseHandle = handle;

			mObjects.push_back(obj);
		}

		void CollisionWorld::removeObject(CollisionObject *obj)
		{
			for (std::vector<CollisionObject*>::iterator iter = mObjects.begin(); iter < mObjects.end(); iter++)
			{
				if ((*iter) == obj)
				{
					mBroadphase->remove(obj->mBroadphaseHandle);

					mObjects.erase(iter);
					return;
				}
			}

			ContactManifold *manifold;
			std::vector<ContactManifold*>::iterator manifoldIterator = mManifolds.begin();
			while (manifoldIterator != mManifolds.end())
			{
				manifold = (*manifoldIterator);
				if (manifold->getA() == obj || manifold->getB() == obj)
				{
					deregisterContact(manifold->getA(), manifold->getB());
					removeManifoldLookup(manifold->getA(), manifold->getB());

					manifoldIterator = mManifolds.erase(manifoldIterator);
					delete manifold;
					continue;
				}

				manifoldIterator++;
			}
		}

		void CollisionWorld::clear()
		{
			for (int i = mObjects.size(); --i >= 0;)
				removeObject(mObjects[i]);
		}

		void CollisionWorld::buildContacts(float dt)
		{
			CollisionObject *a, *b;
			ContactManifold *manifold;
			
			std::vector<ContactManifold*>::iterator manifoldIterator = mManifolds.begin();
			while (manifoldIterator != mManifolds.end())
			{
				manifold = (*manifoldIterator);
				a = manifold->getA();
				b = manifold->getB();

				//TODO: Monitor whether removing manifolds being removed on deactivation has any stability ramifications worth considering, 
				//given that doing it has a big positive influence on performance
				if (((!a->mBody || !a->mBody->active()) && (!b->mBody || !b->mBody->active())) || !a->getBounds().intersects(b->getBounds())) 
				{
					deregisterContact(a, b);
					removeManifoldLookup(a, b);

					manifoldIterator = mManifolds.erase(manifoldIterator);
					delete manifold;
					continue;
				}

				manifold->update(dt);
				manifoldIterator++;
			}

			/*
			for (int i = -1; ++i < mObjects.size();)
			{
				a = mObjects[i];
				for (int j = i; ++j < mObjects.size();)
				{
					b = mObjects[j];
					if (!a->getBounds().intersects(b->getBounds())) 
						continue;
					mMpr->collide(a, b);
				}
			}*/

			float threshold = mSimulator->getRestingVelocityLimit();

			ushort islandIDGen = 0;
			for (int i = -1; ++i < mObjects.size();)
			{
				a = mObjects[i];

				if (!a->mBody || !a->mBody->active())
					continue;

				if (a->mBody && !a->mBody->islandID())
				{
					islandIDGen++;
					if (!islandActive(a, threshold, islandIDGen))
						deactivateIsland(a, islandIDGen);
				}

				if (a->mNeedsBroadphaseUpdate)
				{
					mBroadphase->update(a->mBroadphaseHandle);
					a->mNeedsBroadphaseUpdate = false;
				}
			}

			const Broadphase::BroadphasePair *pairs = mBroadphase->getOverlappingPairs();
			uint numpairs = mBroadphase->getNumOverlappingPairs();

			for (int i = -1; ++i < numpairs;)
			{
				a = pairs[i].A;
				b = pairs[i].B;

				if ((!a->mBody || !a->getBody()->active()) && (!b->mBody || !b->getBody()->active()))
					continue;

				mMpr->collide(pairs[i].A, pairs[i].B);
			}
		}

		const std::vector<ContactManifold*> &CollisionWorld::getManifolds()
		{
			return mManifolds;
		}

		void CollisionWorld::clearManifolds()
		{
			for (int i = -1; ++i < mManifolds.size();)
				delete mManifolds[i];
			mManifolds.clear();
		}

		ContactManifold *CollisionWorld::findManifold(CollisionObject *a, CollisionObject *b)
		{
			GIMBLE_HANDLE ia = a->mID;
			GIMBLE_HANDLE ib = b->mID;

			if (ib<ia)
			{
				GIMBLE_HANDLE iat = ia;
				ia = ib;
				ib = iat;
			}

			int hval = hash(ia, ib);

			if (!mManifoldLookup[hval%mManifoldBuckets].first())
				return 0;

			LinkedListNode<ContactManifold*> *node = mManifoldLookup[hval%mManifoldBuckets].first();
			ContactManifold *m = 0;
			while (node)
			{
				m = node->item();
				if ((m->getA() == a && m->getB() == b) || (m->getA() == b && m->getB() == a))
					return m;

				node = node->next();
			}

			return 0;
		}

		ContactManifold *CollisionWorld::getManifold(CollisionObject *a, CollisionObject *b)
		{
			ContactManifold *m = findManifold(a, b);
			if (m) return m;

			GIMBLE_HANDLE ia = a->mID;
			GIMBLE_HANDLE ib = b->mID;

			if (ib<ia)
			{
				GIMBLE_HANDLE iat = ia;
				ia = ib;
				ib = iat;
			}

			int hval = hash(ia, ib);

			m = new ContactManifold(a, b);

			registerContact(a, b);
			mManifolds.push_back(m);
			mManifoldLookup[hval%mManifoldBuckets].insertFirst(m);

			return m;
		}

		void CollisionWorld::removeManifoldLookup(CollisionObject *a, CollisionObject *b)
		{
			GIMBLE_HANDLE ia = a->mID;
			GIMBLE_HANDLE ib = b->mID;

			if (ib<ia)
			{
				GIMBLE_HANDLE iat = ia;
				ia = ib;
				ib = iat;
			}

			int hval = hash(ia, ib);

			if (!mManifoldLookup[hval%mManifoldBuckets].first())
				return;

			LinkedListNode<ContactManifold*> *node = mManifoldLookup[hval%mManifoldBuckets].first();
			ContactManifold *m = 0;
			while (node)
			{
				m = node->item();
				if ((m->getA() == a && m->getB() == b) || (m->getA() == b && m->getB() == a))
				{
					mManifoldLookup[hval%mManifoldBuckets].remove(node);
					return;
				}

				node = node->next();
			}
		}

		void CollisionWorld::registerContact(CollisionObject *a, CollisionObject *b)
		{
			bool upd = true;
			bool done = false;

			for (int i = -1; ++i < a->mNumContacts;)
				if (a->mContacts[i] == b) { upd = false; break; }

			if (upd)
			{
				for (int i = -1; ++i < a->mNumContacts;)
				{
					if (!a->mContacts[i]) { a->mContacts[i] = b; done = true; break; }
				}
				if (!done)
				{
					CollisionObject **contacts = new CollisionObject*[++a->mNumContacts];
					contacts[a->mNumContacts-1] = b;
					
					//memcpy(contacts, a->mContacts, a->mNumContacts-1);
					for (int i = -1; ++i < a->mNumContacts-1;)
						contacts[i] = a->mContacts[i];

					delete [] a->mContacts;
					a->mContacts = contacts;
				}
			}

			upd = true;
			done = false;

			for (int i = -1; ++i < b->mNumContacts;)
				if (b->mContacts[i] == a) { upd = false; break; }

			if (upd)
			{
				for (int i = -1; ++i < b->mNumContacts;)
				{
					if (!b->mContacts[i]) { b->mContacts[i] = a; done = true; break; }
				}
				if (!done)
				{
					CollisionObject **contacts = new CollisionObject*[++b->mNumContacts];
					contacts[b->mNumContacts-1] = a;
					
					//memcpy(contacts, b->mContacts, b->mNumContacts-1);
					for (int i = -1; ++i < b->mNumContacts-1;)
						contacts[i] = b->mContacts[i];
					
					delete [] b->mContacts;
					b->mContacts = contacts;
				}
			}
		}

		void CollisionWorld::deregisterContact(CollisionObject *a, CollisionObject *b)
		{
			for (int i = -1; ++i < a->mNumContacts;)
				if (a->mContacts[i] == b) { a->mContacts[i] = 0; break; }

			for (int i = -1; ++i < b->mNumContacts;)
				if (b->mContacts[i] == a) { b->mContacts[i] = 0; break; }
		}

		bool CollisionWorld::islandActive(CollisionObject *obj, float threshold, ushort islandID)
		{
			if (obj->mBody->motion() > threshold) 
				return true;

			obj->mBody->setIslandID(islandID);

			for (int i = -1; ++i < obj->mNumContacts;)
			{
				CollisionObject *obj2 = obj->mContacts[i];
				if (obj2 && obj2->mBody && !obj2->mBody->islandID())
				{
					if (obj2->mBody->motion() > threshold)
						return true;

					if (islandActive(obj2, threshold, islandID))
						return true;
				}
			}

			return false;
		}

		void CollisionWorld::deactivateIsland(CollisionObject *obj, ushort islandID)
		{
			obj->mBody->setActive(false);

			for (int i = -1; ++i < obj->mNumContacts;)
			{
				CollisionObject *obj2 = obj->mContacts[i];
				if (obj2 && obj2->mBody && obj2->mBody->islandID() == islandID && obj2->mBody->active())
					deactivateIsland(obj2, islandID);
			}
		}
	}
}