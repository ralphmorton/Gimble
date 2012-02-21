
#ifndef _GIMBLE_CONTACT_
#define _GIMBLE_CONTACT_

#include "GimbleReqs.h"
#include "GimbleVector3.h"

namespace Gimble
{
	namespace Physics
	{
		class Contact
		{
		public:
			Vector3 mRcp1;
			Vector3 mRcp2;
			Vector3 mNormal;
			float mPenetration;

			Vector3 mTangent;
			float mMassNormal;
			float mMassTangent;
			float mBias;
			float mPN;
			float mPT;
			float mPNB;
			float mDV;

		public:
			Contact(Vector3 rcp1, Vector3 rcp2, Vector3 normal, float penetration) : mRcp1(rcp1), mRcp2(rcp2), mNormal(normal), mPenetration(penetration), mMassNormal(0), mBias(0), mPN(0), mPNB(0), mMassTangent(0), mTangent(Vector3::ZERO), mDV(0), mPT(0)
			{ 
				mNormal.normalise();
			}
		};
	}
}

#endif