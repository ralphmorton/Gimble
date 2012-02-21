
#ifndef _GIMBLE_RIGIDBODY_
#define _GIMBLE_RIGIDBODY_

#include "GimbleReqs.h"
#include "GimbleVector3.h"
#include "GimbleMatrix.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API RigidBody
		{
		private:
			friend class CollisionObject;
			friend class Simulator;

		private:
			CollisionObject *mObject;
			float mRestitution;
			float mFriction;
			float mLinearDamping;
			float mAngularDamping;

			float mInverseMass;
			Matrix mInverseInertiaTensor;
			Matrix mInverseInertiaTensorWorld;
			Vector3 mForce;
			Vector3 mTorque;
			Vector3 mLinearVelocity;
			Vector3 mAngularVelocity;
			Vector3 mBiasLinearVelocity;
			Vector3 mBiasAngularVelocity;

			bool mActive;
			ushort mIslandID;
			float mMotion;

		public:
			RigidBody(CollisionShape *shape, const Transform &transform, float mass, float restitution, float friction, float linearDamping, float angularDamping);

			void applyImpulse(const Vector3 &impulse, const Vector3 &relativePosition);
			void applyBiasImpulse(const Vector3 &impulse, const Vector3 &relativePosition);

			Vector3 getVelocity(const Vector3 &relativePosition) const;
			Vector3 getBiasVelocity(const Vector3 &relativePosition) const;

			float getImpulseDenominator(const Vector3 &relativePosition, const Vector3 &normal) const;

			Transform *getTransform();
			CollisionShape *getShape();

			void update();

			inline float getRestitution() const { return mRestitution; }
			inline float getFriction() const { return mFriction; }

			inline float getInverseMass() { return mInverseMass; }
			inline Matrix getInverseWorldSpaceInertiaTensor() { return mInverseInertiaTensorWorld; }
			inline Vector3 getLinearVelocity() { return mLinearVelocity; }
			inline Vector3 getAngularVelocity() { return mAngularVelocity; }

			inline bool active() { return mActive; }
			inline void setActive(bool active)
			{
				mActive = active;
				if (!active) 
					mMotion = 0;
			}

			inline float motion() { return mMotion; }

			inline ushort islandID() { return mIslandID; }
			inline void setIslandID(ushort id)
			{
				mIslandID = id;
			}
		};
	};
}

#endif