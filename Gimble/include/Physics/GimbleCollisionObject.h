
#ifndef _GIMBLE_COLLISIONOBJECT_
#define _GIMBLE_COLLISIONOBJECT_

#include "GimbleReqs.h"
#include "Physics/GimbleCollisionShape.h"
#include "GimbleMatrix.h"
#include "GimbleTransform.h"
#include "GimbleAABB.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API CollisionObject
		{
		private:
			friend class RigidBody;
			friend class CollisionWorld;
			friend class Simulator;

		private:
			static ushort mIDAlloc;

		private:
			RigidBody *mBody;
			CollisionShape *mShape;
			Transform mTransform;
			AABB mBounds;

			GIMBLE_HANDLE mBroadphaseHandle;
			bool mNeedsBroadphaseUpdate;

			ushort mID;
			CollisionObject **mContacts;
			ushort mNumContacts;

		public:
			CollisionObject(const Transform &trans, CollisionShape *shape);
			~CollisionObject();

			Vector3 supportMap(const Vector3 &direction);

			void update();

			inline bool isStatic() { return !mBody; }
			inline RigidBody *getBody() { return mBody; }
			inline Transform *getTransform() { return &mTransform; }
			inline const AABB &getBounds() const { return mBounds; }
		};
	}
}

#endif 