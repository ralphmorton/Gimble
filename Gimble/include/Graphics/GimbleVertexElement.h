
#ifndef _GIMBLE_VERTEXELEMENT_
#define _GIMBLE_VERTEXELEMENT_

#include "GimbleReqs.h"

namespace Gimble
{
	namespace Graphics
	{
		enum VertexElementType
		{
			VERTEXELEMENT_FLOAT1,
			VERTEXELEMENT_FLOAT2,
			VERTEXELEMENT_FLOAT3,
			VERTEXELEMENT_FLOAT4,
			VERTEXELEMENT_COLOR
		};

		enum VertexElementSemantic
		{
			VERTEXELEMENTSEMANTIC_POSITION,
			VERTEXELEMENTSEMANTIC_BLENDWEIGHTS,
	        VERTEXELEMENTSEMANTIC_BLENDINDICES,
			VERTEXELEMENTSEMANTIC_NORMAL,
			VERTEXELEMENTSEMANTIC_DIFFUSE,
			VERTEXELEMENTSEMANTIC_SPECULAR,
			VERTEXELEMENTSEMANTIC_TEXTURECOORDS,
	        VERTEXELEMENTSEMANTIC_BINORMAL,
	        VERTEXELEMENTSEMANTIC_TANGENT
		};

		class GIMBLE_API VertexElement
		{
		private:
			VertexElementType mType;
			VertexElementSemantic mSemantic;
			ushort mInputSlot;
			ushort mIndex;
			ushort mOffset;
		public:
			VertexElement(VertexElementType type, VertexElementSemantic semantic, ushort inputSlot, ushort index, ushort offset) : mType(type), mSemantic(semantic), mInputSlot(inputSlot), mIndex(index), mOffset(offset) 
			{

			}

			inline VertexElementType getType() const { return mType; }
        	inline VertexElementSemantic getSemantic() const { return mSemantic; }
			inline ushort getInputSlot() const { return mInputSlot; }
			inline ushort getIndex() const { return mIndex; }
			inline ushort getOffset() const { return mOffset; }
		};
	}
}

#endif