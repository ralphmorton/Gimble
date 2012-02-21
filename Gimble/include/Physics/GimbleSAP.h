
#ifndef _GIMBLE_SAP_
#define _GIMBLE_SAP_

#include "GimbleReqs.h"
#include "Physics/GimbleBroadphase.h"
#include "GimbleLinkedList.h"
#include "GimbleVector3.h"

namespace Gimble
{
	namespace Physics
	{
		class GIMBLE_API SAP : public Broadphase
		{
		public:
			enum Axis
			{
				AXIS_X,
				AXIS_Y,
				AXIS_Z
			};
			
			class SAPObject;

			struct SAPEntry
			{
				SAPObject *Obj;
				float Value;
				bool Minima;
				uint Index;

				SAPEntry(SAPObject *obj, float value, bool minima) : Obj(obj), Value(value), Minima(minima) { }
			};

			class SAPObject
			{
			public:
				CollisionObject *mObj;
				uint mIndex;
				SAPEntry *mMinX, *mMaxX;
				SAPEntry *mMinY, *mMaxY;
				SAPEntry *mMinZ, *mMaxZ;

				SAPObject(CollisionObject *obj) : mObj(obj), mIndex(0), mMinX(0), mMaxX(0), mMinY(0), mMaxY(0), mMinZ(0), mMaxZ(0) { }
			};

		private:
			PairCache *mCache;

			SAPObject **mObjects;
			uint mNumObjects;
			uint mFirstFreeObjectIndex;

			uint mMaxEntries;

			SAPEntry **mX;
			SAPEntry **mY;
			SAPEntry **mZ;

		public:
			SAP(uint maxObjects);
			~SAP();

			GIMBLE_HANDLE add(CollisionObject *o);
			void remove(GIMBLE_HANDLE o);
			void update(GIMBLE_HANDLE o);

			const BroadphasePair *getOverlappingPairs();
			uint getNumOverlappingPairs();

		private:
			void insertEntries(SAPObject *obj);
			void removeEntries(SAPObject *obj);
			void updateEntries(SAPObject *obj);

			void sortUp(Axis axis, uint index, bool updateOverlap);
			void sortDown(Axis axis, uint index, bool updateOverlap);
		};
	}
}

#endif