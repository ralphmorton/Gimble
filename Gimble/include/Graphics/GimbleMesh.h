
#ifndef _GIMBLE_MESH_
#define _GIMBLE_MESH_

#include "GImbleReqs.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API Mesh
		{
		private:
			static string mTypeName;

		protected:
			string mName;
			Buffer *mIndexBuffer;
			uint mIndexCount;

		public:
			Mesh(string name, Buffer *indexBuffer);
			~Mesh();

			inline const string &getName() { return mName; }
			inline Buffer *getIndexBuffer() { return mIndexBuffer; }
			inline uint getIndexCount() { return mIndexCount; }
		};
	}
}

#endif