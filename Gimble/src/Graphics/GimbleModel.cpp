
#include "Graphics/GimbleModel.h"
#include "Graphics/GimbleMesh.h"
#include "GimbleResourceStream.h"
#include "Graphics/GimbleBuffer.h"
#include "Graphics/GimbleInputLayout.h"
#include "Graphics/GimbleRenderSystem.h"
#include "GimbleLog.h"
#include "GimbleVector2.h"
#include "GimbleVector3.h"

namespace Gimble
{
	namespace Graphics
	{
		string Model::mTypeName = "Model";

		Model::Model(InputLayout *inputLayout, Buffer *vertexBuffer, std::vector<Mesh *> meshes) : mInputLayout(inputLayout), mVertexBuffer(vertexBuffer), mMeshes(meshes)
		{
			
		}

		Model::~Model()
		{
			delete mInputLayout;
			delete mVertexBuffer;

			for (uint i = 0; i < mMeshes.size(); i++)
				delete mMeshes[i];

			mMeshes.clear();
		}
	}
}