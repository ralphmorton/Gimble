
#include "Graphics/D3D11/GimbleD3D11ComputeShaderLoader.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "GimbleResourceStream.h"
#include "GimbleResource.h"
#include "GimbleLog.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11ComputeShaderLoader::D3D11ComputeShaderLoader() {}

		Resource *D3D11ComputeShaderLoader::load(ResourceStream &stream)
		{
			void *data = (void *)new char[stream.getSize()];
			uint size = stream.getSize();
			stream.read(data, 0, size);

			ID3DBlob* errorBlob = NULL;
			ID3DBlob* shaderBlob = NULL;
			ID3D11ComputeShader *shader = 0;

			HRESULT hrc = D3DX11CompileFromMemory((LPCSTR)data, size, NULL, NULL, NULL, "Main", "cs_5_0", 0, 0, NULL, &shaderBlob, &errorBlob, NULL);
			SAFE_RELEASE(errorBlob);

			if (FAILED(hrc))
			{
				SAFE_RELEASE(shaderBlob);
				Log::getPtr()->error("D3D11ComputeShaderLoader->D3DX11CompileFromMemory failed");
				return 0;
			}

			HRESULT hr = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getDevice()->CreateComputeShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
			SAFE_RELEASE(shaderBlob);

			delete [] data;

			if (FAILED(hr))
			{
				Log::getPtr()->error("D3D11ComputeShaderLoader->CreateComputeShader failed");
				return 0;
			}

			return new D3D11ComputeShader(shader);
		}
	}
}