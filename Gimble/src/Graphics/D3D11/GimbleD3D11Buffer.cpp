
#include "Graphics/D3D11/GimbleD3D11Buffer.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"
#include "GimbleLog.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11Buffer::D3D11Buffer(ID3D11Device *device, ID3D11Buffer *buffer, D3D11_BUFFER_DESC desc, BufferType type, BufferUsage usage, uint size, uint stride) : Buffer(type, usage, size, stride), mDevice(device), mBuffer(buffer), mTempStagingBuffer(0), mSRV(0), mUAV(0), mDesc(desc)
		{
			
		}

		D3D11Buffer::~D3D11Buffer()
		{
			SAFE_RELEASE(mSRV);
			SAFE_RELEASE(mUAV);
			SAFE_RELEASE(mBuffer);
		}

		void *D3D11Buffer::map(MapType mapType)
		{
			//assert(mUsage & BUFFERUSAGE_DYNAMIC);

			ID3D11DeviceContext *ctx;
			mDevice->GetImmediateContext(&ctx);

			D3D11_MAP mt = mapType == BUFFERMAPTYPE_READ ? D3D11_MAP_READ : D3D11_MAP_WRITE_DISCARD;

			if (mt == D3D11_MAP_READ)
			{
				ID3D11Buffer* tbuf = NULL;

				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				mBuffer->GetDesc(&desc);
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				desc.Usage = D3D11_USAGE_STAGING;
				desc.BindFlags = 0;
				desc.MiscFlags = 0;

				if (FAILED(mDevice->CreateBuffer(&desc, NULL, &tbuf)))
				{
					SAFE_RELEASE(tbuf);
					Log::getPtr()->error("ID3D11Device::CreateBuffer failed; was trying to create staging buffer for data read.");
					return 0;
				}

				ctx->CopyResource(tbuf, mBuffer);
				mTempStagingBuffer = tbuf;
			}

			void* ret = 0;
			D3D11_MAPPED_SUBRESOURCE msr;
			ZeroMemory(&msr, sizeof(msr));

			HRESULT hr = mTempStagingBuffer ? ctx->Map(mTempStagingBuffer, 0, mt, 0, &msr) : ctx->Map(mBuffer, 0, mt, 0, &msr);

			if (FAILED(hr))
			{
				Log::getPtr()->error("ID3D11Buffer->Map failed, ensure that the resource was created with DYNAMIC usage");
				return 0;
			}

			return msr.pData;
		}

		void D3D11Buffer::unmap()
		{
			ID3D11DeviceContext *ctx;
			mDevice->GetImmediateContext(&ctx);

			if (mTempStagingBuffer)
			{
				ctx->Unmap(mTempStagingBuffer, 0);
				SAFE_RELEASE(mTempStagingBuffer);
				mTempStagingBuffer = 0;
			}
			else
				ctx->Unmap(mBuffer, 0);
		}

		void D3D11Buffer::setData(uint start, uint length, const void *source)
		{
			assert(start + length < mSize);

			void *dest = map(BUFFERMAPTYPE_WRITE);
			dest = static_cast<void*>(static_cast<char*>(dest) + length);

			memcpy(dest, source, length);

			unmap();
		}

		ID3D11ShaderResourceView *D3D11Buffer::getSRV()
		{
			if (mSRV) return mSRV;

			D3D11_BUFFER_DESC bdesc;
			ZeroMemory(&bdesc, sizeof(bdesc));
			mBuffer->GetDesc(&bdesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			desc.BufferEx.FirstElement = 0;

			if (bdesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
			{
				desc.Format = DXGI_FORMAT_R32_TYPELESS;
				desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
				desc.BufferEx.NumElements = bdesc.ByteWidth / 4;
			} 
			else if (bdesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
			{
				desc.Format = DXGI_FORMAT_UNKNOWN;
				desc.BufferEx.NumElements = bdesc.ByteWidth / bdesc.StructureByteStride;
			} 
			else
				return 0;

			mDevice->CreateShaderResourceView(mBuffer, &desc, &mSRV);

			return mSRV;
		}

		ID3D11UnorderedAccessView *D3D11Buffer::getUAV()
		{
			if (mUAV) return mUAV;

			D3D11_BUFFER_DESC bdesc;
			ZeroMemory(&bdesc, sizeof(bdesc));
			mBuffer->GetDesc(&bdesc);
        
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = 0;

			if (bdesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
			{
				desc.Format = DXGI_FORMAT_R32_TYPELESS; // Format must be DXGI_FORMAT_R32_TYPELESS for Raw Unordered Access View
				desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
				desc.Buffer.NumElements = bdesc.ByteWidth / 4; 
			} 
			else if (bdesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
			{
				desc.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN when creating a View of a Structured Buffer
				desc.Buffer.NumElements = bdesc.ByteWidth / bdesc.StructureByteStride; 
			} 
			else
				return 0;
    
			mDevice->CreateUnorderedAccessView(mBuffer, &desc, &mUAV);

			return mUAV;
		}

		D3D11Buffer *D3D11Buffer::create(BufferType type, BufferUsage usage, uint size, uint stride, void *data, ID3D11Device *device)
		{
			ID3D11Buffer *buffer;
			D3D11_BUFFER_DESC desc = D3D11Utils::getDesc(type, usage, size, stride);

			HRESULT hr;

			if (data)
			{
				D3D11_SUBRESOURCE_DATA initData;
				initData.pSysMem = data;
				hr = device->CreateBuffer(&desc, &initData, &buffer);
			}
			else
				hr = device->CreateBuffer(&desc, NULL, &buffer);

			if (FAILED(hr))
			{
				Log::getPtr()->error("ID3D11Device->CreateBuffer failed");
				return 0;
			}

			return new D3D11Buffer(device, buffer, desc, type, usage, size, stride);
		}
	}
}