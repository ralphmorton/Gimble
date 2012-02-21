
#include "Physics/GimbleCollisionSolver.h"
#include "Physics/GimbleCollisionObject.h"
#include "Physics/GimbleRigidBody.h"
#include "Physics/GimbleSimulator.h"
#include "Physics/GimbleContact.h"
#include "Physics/GimbleContactManifold.h"

namespace Gimble
{
	namespace Physics
	{
		CollisionSolver::CollisionSolver()
		{
			mSimulator = Simulator::getPtr();
		}

		void CollisionSolver::prep(float dt, const std::vector<ContactManifold*> &manifolds)
		{
			float idt = dt == 0.0f ? 0.0f : 1.0f / dt;

			for (int h = -1; ++h < manifolds.size();)
			{
				ContactManifold *manifold = manifolds[h];
				CollisionObject *a = manifold->getA();
				CollisionObject *b = manifold->getB();
				RigidBody *ra = a->getBody();
				RigidBody *rb = b->getBody();

				for (int i = -1; ++i < manifold->numContacts();)
				{
					Contact *c = manifold->getContact(i);
					if (!c) continue;

					float ida = ra ? ra->getImpulseDenominator(c->mRcp1, c->mNormal) : 0.f;
					float idb = rb ? rb->getImpulseDenominator(c->mRcp2, c->mNormal) : 0.f;
					c->mMassNormal = 1.0f / (ida + idb);

					Vector3 vela = ra ? ra->getVelocity(c->mRcp1) : Vector3::ZERO;
					Vector3 velb = rb ? rb->getVelocity(c->mRcp2) : Vector3::ZERO;
					Vector3 rvel = velb - vela;
					float vn = rvel.dot(c->mNormal);
					Vector3 tangent = c->mNormal * vn;
                    tangent = rvel - tangent;
					if (tangent != Vector3::ZERO)
					{
						tangent.normalise();
						float idta = ra ? ra->getImpulseDenominator(c->mRcp1, tangent) : 0.f;
						float idtb = rb ? rb->getImpulseDenominator(c->mRcp2, tangent) : 0.f;
						float mtangent = idta + idtb;
						c->mMassTangent = 1.0f / mtangent;
						c->mTangent = tangent;
					}
					else
					{
						c->mMassTangent = 0.0f;
						c->mTangent = Vector3::ZERO;
					}

					c->mBias = -mSimulator->getCollisionBiasFactor() * idt * Math::Min(0.0f, -c->mPenetration + mSimulator->getAllowedPenetration());

					float veln = -rvel.dot(c->mNormal);

					float resta = ra ? ra->getRestitution() : 1.0f;
					float restb = rb ? rb->getRestitution() : 1.0f;
					float restitution = Math::Min(resta, restb);

					c->mDV = veln > (c->mMassNormal * mSimulator->getRestingVelocityLimit() * 2.0f) ? (1.0f + restitution) * (c->mMassNormal * veln) : 0.0f; //TODO: Constant here, make it configurable?

					Vector3 impulse = (c->mPN * c->mNormal) + (c->mPT * c->mTangent);

					if (ra) ra->applyImpulse(-impulse, c->mRcp1);
					if (rb) rb->applyImpulse(impulse, c->mRcp2);
				}
			}
		}

		void CollisionSolver::step(const std::vector<ContactManifold*> &manifolds)
		{
			for (int h = -1; ++h < manifolds.size();)
			{
				ContactManifold *manifold = manifolds[h];
				CollisionObject *a = manifold->getA();
				CollisionObject *b = manifold->getB();
				RigidBody *ra = a->getBody();
				RigidBody *rb = b->getBody();

				for (int i = -1; ++i < manifold->numContacts();)
				{
					Contact *c = manifold->getContact(i);
					if (!c) continue;

					Vector3 vela = ra ? ra->getVelocity(c->mRcp1) : Vector3::ZERO;
					Vector3 velb = rb ? rb->getVelocity(c->mRcp2) : Vector3::ZERO;
					Vector3 rvel = velb - vela;
					float veln = rvel.dot(c->mNormal);

					float deltaImpulse = c->mMassNormal * -veln;
					float pn0 = c->mPN;
					c->mPN = Math::Max(pn0 + deltaImpulse, c->mDV);
					deltaImpulse = c->mPN - pn0;

					Vector3 impulse = c->mNormal * deltaImpulse;

					if (ra) ra->applyImpulse(-impulse, c->mRcp1);
					if (rb) rb->applyImpulse(impulse, c->mRcp2);

					vela = ra ? ra->getBiasVelocity(c->mRcp1) : Vector3::ZERO;
					velb = rb ? rb->getBiasVelocity(c->mRcp2) : Vector3::ZERO;
					rvel = velb - vela;
					float velnb = rvel.dot(c->mNormal);
					float dPnb = c->mMassNormal * (-velnb + c->mBias);
                    float pnb0 = c->mPNB;
                    c->mPNB = Math::Max(pnb0 + dPnb, c->mDV);
                    dPnb = c->mPNB - pnb0;

					Vector3 impulseb = c->mNormal * dPnb;

					if (ra) ra->applyBiasImpulse(-impulseb, c->mRcp1);
					if (rb) rb->applyBiasImpulse(impulseb, c->mRcp2);

					if (c->mTangent != Vector3::ZERO)
					{
						vela = ra ? ra->getVelocity(c->mRcp1) : Vector3::ZERO;
						velb = rb ? rb->getVelocity(c->mRcp2) : Vector3::ZERO;
						rvel = velb - vela;

						float velt = c->mTangent.dot(rvel);
						float deltaImpulseTangent = c->mMassTangent * -velt;

						float fa = ra ? ra->getFriction() : 0.0f;
						float fb = rb ? rb->getFriction() : 0.0f;

						float maxPt = Math::Max(fa, fb) * c->mPN;
						float oldTangentImpulse = c->mPT;
						c->mPT = Math::Clamp(oldTangentImpulse + deltaImpulseTangent, -maxPt, maxPt);
						deltaImpulseTangent = c->mPT - oldTangentImpulse;

						Vector3 frictionImpulse = c->mTangent * deltaImpulseTangent;

						if (ra) ra->applyImpulse(-frictionImpulse, c->mRcp1);
						if (rb) rb->applyImpulse(frictionImpulse, c->mRcp2);
					}
				}
			}
		}
	}
}