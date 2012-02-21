
#ifndef _GIMBLE_D3D11BUFFER_
#define _GIMBLE_D3D11BUFFER_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleBuffer.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11Buffer : public Buffer
		{
		private:
			ID3D11Device *mDevice;
			ID3D11Buffer *mBuffer;
			ID3D11Buffer *mTempStagingBuffer;
			ID3D11ShaderResourceView *mSRV;
			ID3D11UnorderedAccessView *mUAV;
			D3D11_BUFFER_DESC mDesc;

		protected:
			D3D11Buffer(ID3D11Device *device, ID3D11Buffer *buffer, D3D11_BUFFER_DESC desc, BufferType type, BufferUsage usage, uint size, uint stride);

		public:
			~D3D11Buffer();

			void *map(MapType mapType);
			void unmap();

			void setData(uint start, uint length, const void *source);

			ID3D11ShaderResourceView *getSRV();
			ID3D11UnorderedAccessView *getUAV();

			static D3D11Buffer *create(BufferType type, BufferUsage usage, uint size, uint stride, void *data, ID3D11Device *device);

			inline ID3D11Buffer *getD3D11Buffer() const { return mBuffer; }
		};
	}
}

#endif