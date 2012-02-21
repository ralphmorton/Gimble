#include "Graphics/GimbleInputLayout.h"

namespace Gimble
{
	namespace Graphics
	{
		uint InputLayout::getElementCount()
		{
			return mElements.size();
		}

		VertexElement &InputLayout::getElement(uint index)
		{
			assert(index < mElements.size());

			return mElements[index];
		}

		void InputLayout::add(VertexElementType type, VertexElementSemantic semantic, ushort inputSlot, ushort semanticIndex, ushort elementOffset)
		{
			mElements.push_back(VertexElement(type, semantic, inputSlot, semanticIndex, elementOffset));
		}

		void InputLayout::insert(VertexElementType type, VertexElementSemantic semantic, ushort inputSlot, ushort semanticIndex, ushort elementOffset, uint elementIndex)
		{
			if (elementIndex >= mElements.size())
			{
				mElements.push_back(VertexElement(type, semantic, inputSlot, semanticIndex, elementOffset));
				return;
			}

			std::vector<VertexElement>::iterator i = mElements.begin();
        	for (unsigned short n = 0; n < elementIndex; n++) i++;

			mElements.insert(i, VertexElement(type, semantic, inputSlot, semanticIndex, elementOffset));
		}

		void InputLayout::remove(uint elementIndex)
		{
			assert(elementIndex < mElements.size());

			std::vector<VertexElement>::iterator i = mElements.begin();
        	for (unsigned short n = 0; n < elementIndex; n++) i++;

			mElements.erase(i);
		}

		void InputLayout::clear()
		{
			mElements.clear();
		}
	}
}