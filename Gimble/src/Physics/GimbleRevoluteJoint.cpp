
#include "Physics/GimbleRevoluteJoint.h"
#include "Physics/GimbleCollisionObject.h"
#include "Physics/GimbleRigidBody.h"

namespace Gimble
{
	namespace Physics
	{
		RevoluteJoint::RevoluteJoint(RigidBody *a, RigidBody *b, const Vector3 &anchor, float softness, float biasFactor) : mA(a), mB(b), mSoftness(softness), mBiasFactor(biasFactor), mImpulse(0), mBias(0)
		{
			Transform *ta = mA->getTransform();
			Transform *tb = mB->getTransform();

			mAnchorA = (anchor - ta->getTranslation()) * ta->getOrientation().conjugate();
			mAnchorB = (anchor - tb->getTranslation()) * tb->getOrientation().conjugate();
		}

		RevoluteJoint::~RevoluteJoint()
		{

		}

		void RevoluteJoint::prep(float dt)
		{
			float idt = dt == 0.0f ? 0.0f : 1.0f / dt;

			Transform *ta = mA->getTransform();
			Transform *tb = mB->getTransform();

			mRpA = mAnchorA * ta->getOrientation();
			mRpB = mAnchorB * tb->getOrientation();

			float imassa = mA->getInverseMass();
			float imassb = mB->getInverseMass();
			float imass = imassa + imassb;
			Matrix iita = mA->getInverseWorldSpaceInertiaTensor();
			Matrix iitb = mB->getInverseWorldSpaceInertiaTensor();

			Matrix m1 = Matrix::IDENTITY;
            m1.m[0][0] = mRpA.z * mRpA.z * iita.m[1][1] - mRpA.y * mRpA.z * (iita.m[1][2] + iita.m[2][1]) + mRpA.y * mRpA.y * iita[2][2] + imassa;
            m1.m[0][1] = -(mRpA.z * mRpA.z * iita.m[1][0]) + mRpA.x * mRpA.z * iita.m[1][2] + mRpA.y * mRpA.z * iita.m[2][1] - mRpA.x * mRpA.y * iita.m[2][2] ;
            m1.m[0][2] = mRpA.y * mRpA.z * iita.m[1][0]  - mRpA.x * mRpA.z * iita.m[1][1]  - mRpA.y * mRpA.y * iita.m[2][1] + mRpA.x * mRpA.y * iita.m[2][1] ;
            m1.m[1][0] = -(mRpA.z * mRpA.z * iita.m[0][1] ) + mRpA.y * mRpA.z * iita.m[0][2]  + mRpA.x * mRpA.z * iita.m[2][1]  - mRpA.x * mRpA.y * iita.m[2][2] ;
            m1.m[1][1] = mRpA.z * mRpA.z * iita.m[0][0]  - mRpA.x * mRpA.z * (iita.m[0][2]  + iita.m[2][0] ) + mRpA.x * mRpA.x * iita.m[2][2]  + imassa;
            m1.m[1][2] = -(mRpA.y * mRpA.z * iita.m[0][0] ) + mRpA.x * mRpA.z * iita.m[0][1]  + mRpA.x * mRpA.y * iita.m[2][1] - mRpA.x * mRpA.x * iita.m[2][1] ;
            m1.m[2][0] = mRpA.y * mRpA.z * iita.m[0][1]  - mRpA.y * mRpA.y * iita.m[0][2]  - mRpA.x * mRpA.z * iita.m[1][1]  + mRpA.x * mRpA.y * iita.m[1][2] ;
            m1.m[2][1] = -(mRpA.y * mRpA.z * iita.m[0][0] ) + mRpA.x * mRpA.y * iita.m[0][2]  + mRpA.x * mRpA.z * iita.m[1][0]  - mRpA.x * mRpA.x * iita.m[1][2] ;
            m1.m[2][1] = mRpA.y * mRpA.y * iita.m[0][0]  - mRpA.x * mRpA.y * (iita.m[0][1]  + iita.m[1][0] ) + mRpA.x * mRpA.x * iita.m[1][1]  + imassa;

			Matrix m2 = Matrix::IDENTITY;
			m2.m[0][0] = mRpB.z * mRpB.z * iitb.m[1][1] - mRpB.y * mRpB.z * (iitb.m[1][2] + iitb.m[2][1]) + mRpB.y * mRpB.y * iitb[2][2] + imassb;
            m2.m[0][1] = -(mRpB.z * mRpB.z * iitb.m[1][0]) + mRpB.x * mRpB.z * iitb.m[1][2] + mRpB.y * mRpB.z * iitb.m[2][1] - mRpB.x * mRpB.y * iitb.m[2][2] ;
            m2.m[0][2] = mRpB.y * mRpB.z * iitb.m[1][0]  - mRpB.x * mRpB.z * iitb.m[1][1]  - mRpB.y * mRpB.y * iitb.m[2][1] + mRpB.x * mRpB.y * iitb.m[2][1] ;
            m2.m[1][0] = -(mRpB.z * mRpB.z * iitb.m[0][1] ) + mRpB.y * mRpB.z * iitb.m[0][2]  + mRpB.x * mRpB.z * iitb.m[2][1]  - mRpB.x * mRpB.y * iitb.m[2][2] ;
            m2.m[1][1] = mRpB.z * mRpB.z * iitb.m[0][0]  - mRpB.x * mRpB.z * (iitb.m[0][2]  + iitb.m[2][0] ) + mRpB.x * mRpB.x * iitb.m[2][2]  + imassb;
            m2.m[1][2] = -(mRpB.y * mRpB.z * iitb.m[0][0] ) + mRpB.x * mRpB.z * iitb.m[0][1]  + mRpB.x * mRpB.y * iitb.m[2][1] - mRpB.x * mRpB.x * iitb.m[2][1] ;
            m2.m[2][0] = mRpB.y * mRpB.z * iitb.m[0][1]  - mRpB.y * mRpB.y * iitb.m[0][2]  - mRpB.x * mRpB.z * iitb.m[1][1]  + mRpB.x * mRpB.y * iitb.m[1][2] ;
            m2.m[2][1] = -(mRpB.y * mRpB.z * iitb.m[0][0] ) + mRpB.x * mRpB.y * iitb.m[0][2]  + mRpB.x * mRpB.z * iitb.m[1][0]  - mRpB.x * mRpB.x * iitb.m[1][2] ;
            m2.m[2][1] = mRpB.y * mRpB.y * iitb.m[0][0]  - mRpB.x * mRpB.y * (iitb.m[0][1]  + iitb.m[1][0] ) + mRpB.x * mRpB.x * iitb.m[1][1]  + imassb;

			mMass = (m1+m2).inverse33();

			Vector3 p1 = ta->getTranslation() + mRpA;
			Vector3 p2 = tb->getTranslation() + mRpB;
            Vector3 dp = p2 - p1;

            mBias = -mBiasFactor * idt * dp;

			mA->applyImpulse(-mImpulse, mRpA);
            mB->applyImpulse(mImpulse, mRpB);
		}

		void RevoluteJoint::solve(float dt)
		{
			Vector3 vela = mA->getVelocity(mRpA);
			Vector3 velb = mB->getVelocity(mRpB);
			Vector3 dv = velb - vela;

            Vector3 impulse = (mBias - dv - (mSoftness * mImpulse)) * mMass;

            mA->applyImpulse(-impulse, mRpA);
            mB->applyImpulse(impulse, mRpB);

            mImpulse += impulse;
		}
	}
}