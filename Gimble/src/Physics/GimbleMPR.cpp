
#include "Physics/GimbleMPR.h"
#include "Physics/GimbleContact.h"
#include "Physics/GimbleCollisionObject.h"
#include "Physics/GimbleCollisionWorld.h"
#include "GimbleVector3.h"
#include "Physics/GimbleBoxShape.h"

namespace Gimble
{
	namespace Physics
	{
		MPR::MPR(CollisionWorld *world, int maxIterations, float tolerance) : mWorld(world), mMaxIterations(maxIterations), mTolerance(tolerance)
		{
			
		}

		bool MPR::collide(CollisionObject *a, CollisionObject *b)
		{
			//Return vectors
            Vector3 rp1 = Vector3::ZERO, rp2 = Vector3::ZERO;

            //Get the centre of the minkowski sum
			Vector3 v01 = a->getTransform()->getTranslation();
            Vector3 v02 = b->getTransform()->getTranslation();
            Vector3 v0 = v02 - v01;

            //Avoid centres overlapping and causing issues
            if (v0 == Vector3::ZERO) v0.x = 0.00001f;

            //Start search toward origin
            Vector3 n = -v0;
            Vector3 mn = v0;
			Vector3 v11 = a->supportMap(mn);
            Vector3 v12 = b->supportMap(n);
            Vector3 v1 = v12 - v11;
			if (v1.dot(n) <= 0.0f) return false; //No collision

			//Search perp to the line, in the direction of the origin
			n = v1.cross(v0);
            mn = -n;
            if (n == Vector3::ZERO)
            {
                n = v1 - v0;
				n.normalise();
                mn = -n;
                Vector3 pp1 = v11 - a->getTransform()->getTranslation();
                Vector3 pp2 = v12 - b->getTransform()->getTranslation();
                float pen = v1.dot(n);

				mWorld->addContact(a, b, new Contact(pp1, pp2, mn, pen));
                return true;
            }
            
			Vector3 v21 = a->supportMap(mn);
			Vector3 v22 = b->supportMap(n);
            Vector3 v2 = v22 - v21;
            if (v2.dot(n) <= 0.0f) return false;

			//Find out which side of the plane the origin is on, and swap the normal if necessary
			n = (v1 - v0).cross(v2 - v0);
            mn = -n;
			float dist = n.dot(v0);
            
            if (dist > 0.0f)
            {
                Vector3 v1t = v1;
                v1 = v2;
                v2 = v1t;

                Vector3 v11t = v11;
                v11 = v21;
                v21 = v11t;

                Vector3 v12t = v12;
                v12 = v22;
                v22 = v12t;

                mn = n;
                n = -n;
            }

			int iterations = 0;
            //Find a portal
            while (true)
            {
                iterations++;
                if (iterations > mMaxIterations) return false; //Failed

				Vector3 v31 = a->supportMap(mn);
				Vector3 v32 = b->supportMap(n);
                Vector3 v3 = v32 - v31;
                if (v3.dot(n) <= 0.0f) return false; //No collision

                //If the origin is outside (v1,v0,v3), get rid of v2
				if (v1.cross(v3).dot(v0) < 0.0f)
                {
                    v2 = v3;
                    v21 = v31;
                    v22 = v32;
					n = (v1 - v0).cross(v3 - v0);
                    mn = -n;
                    continue;
                }

                //If the origin is outside (v3,v0,v2), get rid of v1
				if (v3.cross(v2).dot(v0) < 0.0f)
                {
                    v1 = v3;
                    v11 = v31;
                    v12 = v32;
                    n = (v3 - v0).cross(v2 - v0);
                    mn = -n;
                    continue;
                }
                
                //Refine portal
                bool hit = false;
                while (true)
                {
					n = (v2 - v1).cross(v3 - v1);
                    n.normalise();
                    mn = -n;
                    float d = n.dot(v1);
                    
                    //We have a hit
                    if (d >= 0.0f && !hit)
                    {
						float b0 = v1.cross(v2).dot(v3);
                        float b1 = v3.cross(v2).dot(v0);
                        float b2 = v0.cross(v1).dot(v3);
                        float b3 = v2.cross(v1).dot(v0);
                        float sum = b0 + b1 + b2 + b3;

                        if (sum <= 0)
                        {
                            b0 = 0;
                            b1 = v2.cross(v3).dot(n);
                            b2 = v3.cross(v1).dot(n);
                            b3 = v1.cross(v2).dot(n);
                            sum = b1 + b2 + b3;
                        }
                        float inv = 1.0f / sum;

                        rp1 = ((b0 * v01) + (b1 * v11) + (b2 * v21) + (b3 * v31)) * inv;
                        rp2 = ((b0 * v02) + (b1 * v12) + (b2 * v22) + (b3 * v32)) * inv;

                        hit = true;
                    }
                    
					Vector3 v41 = a->supportMap(mn);
					Vector3 v42 = b->supportMap(n);
                    Vector3 v4 = v42 - v41;
                    float delta = (v4 - v3).dot(n);
                    float separation = -v4.dot(n);

                    if (delta <= mTolerance || separation >= 0)
                    {
                        if (hit)
                        {
                            //Hit
							Vector3 pp1 = rp1 - a->getTransform()->getTranslation();
                            Vector3 pp2 = rp2 - b->getTransform()->getTranslation();

							mWorld->addContact(a, b, new Contact(pp1, pp2, mn, -separation));
                            return true;
                        }
                        return false;
                    }

                    float d1 = v4.cross(v1).dot(v0);
                    float d2 = v4.cross(v2).dot(v0);
                    float d3 = v4.cross(v3).dot(v0);

                    if (d1 < 0)
                    {
                        if (d2 < 0)
                        {
                            v1 = v4;
                            v11 = v41;
                            v12 = v42;
                        }
                        else
                        {
                            v3 = v4;
                            v31 = v41;
                            v32 = v42;
                        }
                    }
                    else
                    {
                        if (d3 < 0)
                        {
                            v2 = v4;
                            v21 = v41;
                            v22 = v42;
                        }
                        else
                        {
                            v1 = v4;
                            v11 = v41;
                            v12 = v42;
                        }
                    }
                }
            }
		}
	}
}