
#ifndef _GIMBLE_D3D11VERTEXDECLARATION_
#define _GIMBLE_D3D11VERTEXDECLARATION_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "Graphics/GimbleInputLayout.h"
#include "Graphics/D3D11/GimbleD3D11Shader.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11InputLayout : public InputLayout
		{
		public:
			typedef std::map<const D3D11VertexShader*, ID3D11InputLayout*> LayoutMap;
			typedef std::map<const D3D11VertexShader*, ID3D11InputLayout*>::iterator LayoutMapIterator;
			typedef std::pair<const D3D11VertexShader*, ID3D11InputLayout*> LayoutMapPair;

		private:
			D3D11RenderSystem *mRenderSystem;
			LayoutMap mLayoutMap;
			bool mDirty;

			D3D11_INPUT_ELEMENT_DESC *mDesc;

		private:
			void update();

		public: 
			D3D11InputLayout() : mDirty(false), mDesc(0), InputLayout() 
			{
				mRenderSystem = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr());
			}

			~D3D11InputLayout();

			inline void add(VertexElementType type, VertexElementSemantic semantic, ushort inputSlot, ushort semanticIndex, ushort elementOffset)
			{
				mDirty = true;
				InputLayout::add(type, semantic, inputSlot, semanticIndex, elementOffset);
			}

			inline void insert(VertexElementType type, VertexElementSemantic semantic, ushort inputSlot, ushort semanticIndex, ushort elementOffset, uint elementIndex)
			{
				mDirty = true;
				InputLayout::insert(type, semantic, inputSlot, semanticIndex, elementOffset, elementIndex);
			}

			inline void remove(uint elementIndex)
			{
				mDirty = true;
				InputLayout::remove(elementIndex);
			}

			inline void clear()
			{
				mDirty = true;
				InputLayout::clear();
			}

			ID3D11InputLayout *getLayout(const D3D11VertexShader *shader);
		};
	}
}

#endif