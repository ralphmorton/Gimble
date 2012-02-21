
#include "Physics/GimbleRigidBody.h"
#include "Physics/GimbleCollisionShape.h"
#include "Physics/GimbleCollisionObject.h"
#include "GimbleTransform.h"

namespace Gimble
{
	namespace Physics
	{
		RigidBody::RigidBody(CollisionShape *shape, const Transform &transform, float mass, float restitution, float friction, float linearDamping, float angularDamping) : mRestitution(Math::Clamp(restitution, 0.0f, 1.0f)), mFriction(Math::Clamp(friction, 0.0f, 1.0f)), mLinearDamping(linearDamping), mAngularDamping(angularDamping), mForce(Vector3::ZERO), mTorque(Vector3::ZERO), mLinearVelocity(Vector3::ZERO), mAngularVelocity(Vector3::ZERO), mBiasLinearVelocity(Vector3::ZERO), mBiasAngularVelocity(Vector3::ZERO), mInverseMass(1.f/mass), mActive(true), mMotion(0.5f)
		{
			assert(mass>0.f&&"Mass must be positive");

			mInverseInertiaTensor = shape->createInertiaTensor(transform.getScale(), mass);
			mInverseInertiaTensor = mInverseInertiaTensor.inverse();

			mObject = new CollisionObject(transform, shape);
			mObject->mBody = this;

			update();
		}

		void RigidBody::applyImpulse(const Vector3 &impulse, const Vector3 &relativePosition)
		{
			float imass = mInverseMass;
			
			mLinearVelocity.x += (impulse.x * imass);
            mLinearVelocity.y += (impulse.y * imass);
            mLinearVelocity.z += (impulse.z * imass);

			Vector3 irp = relativePosition.cross(impulse) * mInverseInertiaTensorWorld;
			mAngularVelocity += irp;
		}

		void RigidBody::applyBiasImpulse(const Vector3 &impulse, const Vector3 &relativePosition)
		{
			float imass = mInverseMass;
			
			mBiasLinearVelocity.x += (impulse.x * imass);
            mBiasLinearVelocity.y += (impulse.y * imass);
            mBiasLinearVelocity.z += (impulse.z * imass);

			Vector3 irp = relativePosition.cross(impulse) * mInverseInertiaTensorWorld;
			mBiasAngularVelocity += irp;
		}

		Vector3 RigidBody::getVelocity(const Vector3 &relativePosition) const
		{
			return mLinearVelocity + mAngularVelocity.cross(relativePosition);
		}

		Vector3 RigidBody::getBiasVelocity(const Vector3 &relativePosition) const
		{
			return mBiasLinearVelocity + mBiasAngularVelocity.cross(relativePosition);
		}

		float RigidBody::getImpulseDenominator(const Vector3 &relativePosition, const Vector3 &normal) const
		{
			Vector3 co = relativePosition.cross(normal);
			Vector3 v = (co * mInverseInertiaTensorWorld).cross(relativePosition);
			return mInverseMass + normal.dot(v);
		}

		Transform *RigidBody::getTransform()
		{
			return mObject->getTransform();
		}

		CollisionShape *RigidBody::getShape()
		{
			return mObject->mShape;
		}

		void RigidBody::update()
		{
			Matrix transform(mObject->getTransform()->getOrientation());
			mInverseInertiaTensorWorld = transform * mInverseInertiaTensor * transform.transpose();
			mInverseInertiaTensorWorld.m[3][0] = 0;
			mInverseInertiaTensorWorld.m[3][1] = 0;
			mInverseInertiaTensorWorld.m[3][2] = 0;

			mObject->update();
		}
	}
}