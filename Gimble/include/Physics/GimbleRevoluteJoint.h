
#ifndef _GIMBLE_REVOLUTEJOINT_
#define _GIMBLE_REVOLUTEJOINT_

#include "GimbleReqs.h"
#include "Physics/GimbleJoint.h"
#include "GimbleVector3.h"
#include "GimbleMatrix.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API RevoluteJoint : public Joint
		{
		private:
			RigidBody *mA, *mB;
			Vector3 mAnchorA, mAnchorB;
			float mSoftness, mBiasFactor;

			Vector3 mImpulse, mBias;
			Vector3 mRpA, mRpB;
			Matrix mMass;

		public:
			RevoluteJoint(RigidBody *a, RigidBody *b, const Vector3 &anchor, float softness, float biasFactor);
			~RevoluteJoint();

			void prep(float dt);
			void solve(float dt);
		};
	}
}

#endif