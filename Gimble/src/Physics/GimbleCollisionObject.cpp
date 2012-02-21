
#include "Physics/GimbleCollisionObject.h"
#include "GimbleVector3.h"

namespace Gimble
{
	namespace Physics
	{
		ushort CollisionObject::mIDAlloc = 0;

		CollisionObject::CollisionObject(const Transform &trans, CollisionShape *shape) : mBody(0), mTransform(trans), mShape(shape), mBroadphaseHandle(0) , mNeedsBroadphaseUpdate(false), mID(mIDAlloc++), mContacts(new CollisionObject*[3]), mNumContacts(3)
		{
			assert(shape&&"Shape cannot be null");

			mContacts[0] = 0;
			mContacts[1] = 0;
			mContacts[2] = 0;

			update();
		}

		CollisionObject::~CollisionObject()
		{
			delete [] mContacts;
		}

		Vector3 CollisionObject::supportMap(const Vector3 &direction)
		{
			Matrix imat = mTransform.getInverseMat();
			imat.m[3][0] = 0;
			imat.m[3][1] = 0;
			imat.m[3][2] = 0;

			Vector3 dir = direction * imat;

			Vector3 p = mShape->supportMap(dir);

			return p * mTransform.getMat();
		}

		void CollisionObject::update()
		{
			mBounds.min = Vector3(
                supportMap(-Vector3::UNIT_X).x * 1.05f,
                supportMap(-Vector3::UNIT_Y).y * 1.05f,
                supportMap(-Vector3::UNIT_Z).z * 1.05f
			);
            mBounds.max = Vector3(
                supportMap(Vector3::UNIT_X).x * 1.05f,
                supportMap(Vector3::UNIT_Y).y * 1.05f,
                supportMap(Vector3::UNIT_Z).z * 1.05f
            );
		}
	}
}