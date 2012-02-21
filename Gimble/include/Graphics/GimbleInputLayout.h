
#ifndef _GIMBLE_INPUTLAYOUT_
#define _GIMBLE_INPUTLAYOUT_

#include "GimbleReqs.h"
#include "Graphics/GimbleVertexElement.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API InputLayout
		{
		protected:
			std::vector<VertexElement> mElements;

		protected:
			InputLayout() { }

		public:
			virtual ~InputLayout() { }

			uint getElementCount();
			VertexElement &getElement(uint index);

			virtual void add(VertexElementType type, VertexElementSemantic semantic, ushort inputSlot, ushort semanticIndex, ushort elementOffset);
			virtual void insert(VertexElementType type, VertexElementSemantic semantic, ushort inputSlot, ushort semanticIndex, ushort elementOffset, uint elementIndex);
			virtual void remove(uint elementIndex);
			virtual void clear();
		};
	}
}

#endif