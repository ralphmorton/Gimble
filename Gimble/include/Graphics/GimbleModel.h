
#ifndef _GIMBLE_MODEL_
#define _GIMBLE_MODEL_

#include "GimbleReqs.h"
#include "GimbleResource.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API Model : public Resource
		{
		private:
			typedef std::vector<Mesh*> MeshList;

		private:
			static string mTypeName;

		private:
			InputLayout *mInputLayout;
			Buffer *mVertexBuffer;
			MeshList mMeshes;

		public:
			Model(InputLayout *inputLayout, Buffer *vertexBuffer, std::vector<Mesh*> meshes);
			~Model();

			inline const std::type_info &getType() { return typeid(Model); }
			inline const string &getTypeName() { return mTypeName; }

			inline InputLayout *getInputLayout() { return mInputLayout; }
			inline Buffer *getVertexBuffer() { return mVertexBuffer; }

			inline uint getMeshCount() { return mMeshes.size(); }
			inline Mesh *getMesh(uint index) { assert(index<mMeshes.size()); return mMeshes[index]; }
		};
	}
}

#endif