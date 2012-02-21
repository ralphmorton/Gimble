
#include "Graphics/D3D11/GimbleD3D11InputLayout.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"
#include "GimbleLog.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11InputLayout::~D3D11InputLayout()
		{
			for (LayoutMapIterator iter = mLayoutMap.begin(); iter != mLayoutMap.end(); iter++)
				SAFE_RELEASE(iter->second);

			mLayoutMap.clear();
		}

		void D3D11InputLayout::update()
		{
			for (LayoutMapIterator iter = mLayoutMap.begin(); iter != mLayoutMap.end(); iter++)
				SAFE_RELEASE(iter->second);
			mLayoutMap.clear();

			if (mDesc)
				delete mDesc;

			uint count = mElements.size();
			mDesc = new D3D11_INPUT_ELEMENT_DESC[count];
			for (uint i = 0; i < count; i++)
			{
				VertexElement element = mElements[i];

				mDesc[i].SemanticName = D3D11Utils::getSemantic(element.getSemantic());
				mDesc[i].SemanticIndex = element.getIndex();
				mDesc[i].Format = D3D11Utils::getVertexElementType(element.getType());
				mDesc[i].InputSlot = element.getInputSlot();
				mDesc[i].AlignedByteOffset = static_cast<WORD>(element.getOffset());
				mDesc[i].InputSlotClass	= D3D11_INPUT_PER_VERTEX_DATA;
				mDesc[i].InstanceDataStepRate = 0;
			}

			mDirty = false;
		}

		ID3D11InputLayout *D3D11InputLayout::getLayout(const D3D11VertexShader *shader)
		{
			if (mDirty)
				update();

			LayoutMapIterator iter = mLayoutMap.find(shader);

			if (iter != mLayoutMap.end())
				return iter->second;

			ID3D10Blob *bin = shader->getBinary();

			ID3D11InputLayout *iLayout;
			HRESULT hr = mRenderSystem->getDevice()->CreateInputLayout(mDesc, mElements.size(), bin->GetBufferPointer(), bin->GetBufferSize(), &iLayout);

			if (FAILED(hr))
			{
				Log::getPtr()->error("Could not create D3D11 Input Layout");
				return 0;
			}

			mLayoutMap.insert(LayoutMapPair(shader, iLayout));

			return iLayout;
		}
	}
}