
#ifndef _GIMBLE_CONTACTMANIFOLD_
#define _GIMBLE_CONTACTMANIFOLD_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Physics
	{
		class ContactManifold
		{
		private:
			Simulator *mSimulator;
			CollisionObject *mA;
			CollisionObject *mB;
			Contact **mContacts;

		public:
			ContactManifold(CollisionObject *a, CollisionObject *b);
			ContactManifold(CollisionObject *a, CollisionObject *b, Contact *c);
			~ContactManifold();

			void add(Contact *c);
			void clear();
			void update(float dt);

			inline CollisionObject *getA() { return mA; }
			inline CollisionObject *getB() { return mB; }

			inline uint numContacts() { return 5; }
			inline Contact *getContact(uint index) { return mContacts[index]; }
		};
	}
}

#endif