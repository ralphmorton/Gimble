
#ifndef _GIMBLE_SIMULATOR_
#define _GIMBLE_SIMULATOR_

#include "GimbleReqs.h"
#include "GimbleSingleton.h"
#include "GimbleVector3.h"
#include "Physics/GimbleCollisionWorld.h"
#include "Physics/GimbleCollisionSolver.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API Simulator : public Singleton<Simulator>
		{
		protected:
			friend class CollisionObject;
			friend class RigidBody;

		private:
			Vector3 mGravity;
			float mMaxContactSeparation;
			float mAllowedPenetration;
			float mCollisionBiasFactor;
			float mRestingVelocityLimit;
			uint mIterations;

			CollisionWorld mCollisionWorld;
			CollisionSolver mCollisionSolver;

			std::vector<RigidBody*> mBodies;
			std::vector<Joint*> mJoints;

		public:
			Simulator(Broadphase *broadphase);
			~Simulator();

			void addBody(RigidBody *body);
			void addCollisionObject(CollisionObject *obj);
			void addJoint(Joint *joint);

			void removeBody(RigidBody *body);
			void removeCollisionObject(CollisionObject *obj);
			void removeJoint(Joint *joint);

			void step(float dt);

			void clear();

			inline Vector3 getGravity() const { return mGravity; }
			inline float getMaxContactSeparation() const { return mMaxContactSeparation; }
			inline float getAllowedPenetration() const { return mAllowedPenetration; }
			inline float getCollisionBiasFactor() const { return mCollisionBiasFactor; }
			inline float getRestingVelocityLimit() const { return mRestingVelocityLimit; }
			inline uint getIterations() const { return mIterations; }

			inline void setGravity(const Vector3 &gravity) { mGravity = gravity; }
			inline void setMaxContactSeparation(float val) { mMaxContactSeparation = val; }
			inline void setAllowedPenetration(float val) { mAllowedPenetration = val; }
			inline void setCollisionBiasFactor(float val) { mCollisionBiasFactor = val; }
			inline void setRestingVelocityLimit(float val) { mRestingVelocityLimit = val; }
			inline void setIterations(uint val) { mIterations = val; }
		};
	}
}

#endif