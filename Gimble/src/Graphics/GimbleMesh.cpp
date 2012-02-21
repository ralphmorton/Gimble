
#include "GimbleResourceStream.h"
#include "Graphics/GimbleMesh.h"
#include "Graphics/GimbleBuffer.h"

namespace Gimble
{
	namespace Graphics
	{
		Mesh::Mesh(string name, Buffer *indexBuffer) : mName(name), mIndexBuffer(indexBuffer), mIndexCount(indexBuffer->getSize() / indexBuffer->getStride())
		{

		}

		Mesh::~Mesh()
		{
			delete mIndexBuffer;
		}
	}
}