
#ifndef _GIMBLE_D3D11RENDERSYSTEM_
#define _GIMBLE_D3D11RENDERSYSTEM_

#include "GimbleReqs.h"
#include "GimbleSingleton.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleRenderSystem.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11RenderSystem : public RenderSystem
		{
		private:
			RenderWindow *mRenderWindow;

			HWND mHWnd;

			ID3D11Device* mDevice;
			ID3D11DeviceContext* mContext;
			IDXGISwapChain* mSwapChain;
			D3D_FEATURE_LEVEL mFeatureLevel;

			ID3D11RasterizerState* mRasterState;
			D3D11_RASTERIZER_DESC mRasterStateDesc;

			ID3D11DepthStencilState *mDepthState;
			D3D11_DEPTH_STENCIL_DESC mDepthStateDesc;

			ID3D11RenderTargetView* mRenderTargetView;
			ID3D11DepthStencilView* mDepthStencilView;

			D3D11InputLayout *mInputLayout;
			D3D11VertexShader *mVertexShader;

			int mWidth, mHeight;

			D3D11TextureLoader *mTexLoader;
			D3D11VertexShaderLoader *mVertexShaderLoader;
			D3D11PixelShaderLoader *mPixelShaderLoader;
			D3D11GeometryShaderLoader *mGeometryShaderLoader;
			D3D11HullShaderLoader *mHullShaderLoader;
			D3D11DomainShaderLoader *mDomainShaderLoader;
			D3D11ComputeShaderLoader *mComputeShaderLoader;

		private:
			bool initDevice(bool fullscreen);

		public:
			D3D11RenderSystem();
			~D3D11RenderSystem();

			RenderWindow *init(const string title, uint width, uint height, bool fullscreen);
			void shutdown();

			inline uint getWidth() const { return mWidth; }
			inline uint getHeight() const { return mHeight; }
			const RenderWindow *getRenderWindow() const { return mRenderWindow; }

			Buffer *createBuffer(Buffer::BufferType type, Buffer::BufferUsage usage, uint size, uint stride, void *data);
			RenderTarget *createRenderTarget(uint width, uint height, PixelFormat format);
			DepthStencil *createDepthStencil(uint width, uint height, PixelFormat textureFormat, PixelFormat depthFormat, PixelFormat resourceFormat);
			Texture *createTexture(uint width, uint height, uint mipLevels, PixelFormat format, Texture::TextureAccess access);

			DepthStencilState *createDepthStencilState();
			void setDepthStencilState(DepthStencilState &state);

			RasterizerState *createRasterizerState();
			void setRasterizerState(RasterizerState &state);

			BlendState *createBlendState();
			void setBlendState(BlendState &state);

			Sampler *createSampler(Sampler::Filter filter, Sampler::AddressMode addressU, Sampler::AddressMode addressV, Sampler::AddressMode addressW, float mipLODBias, uint maxAnistropy, ComparisonFunc comparisonFunc, float minLOD, float maxLOD, float borderR, float borderG, float borderB, float borderA);
			void setVertexSamplers(uint startSlot, uint numSamplers, Sampler **samplers);
			void setPixelSamplers(uint startSlot, uint numSamplers, Sampler **samplers);
			void setGeometrySamplers(uint startSlot, uint numSamplers, Sampler **samplers);

			void setVertexConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers);
			void setPixelConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers);
			void setGeometryConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers);
			void setHullConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers);
			void setDomainConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers);

			ShaderResourceGroup *createShaderResourceGroup(unsigned char initialSize);
			void setVertexShaderResources(ShaderResourceGroup &group);
			void setPixelShaderResources(ShaderResourceGroup &group);
			void setGeometryShaderResources(ShaderResourceGroup &group);
			void setHullShaderResources(ShaderResourceGroup &group);
			void setDomainShaderResources(ShaderResourceGroup &group);

			InputLayout *createInputLayout();
			void setInputLayout(InputLayout &decl);

			void setVertexShader(VertexShader *shader);
			void setPixelShader(PixelShader *shader);
			void setGeometryShader(GeometryShader *shader);
			void setHullShader(HullShader *shader);
			void setDomainShader(DomainShader *shader);

			void setPrimitiveTopology(PrimitiveTopology topology);

			void clear(const Color &color, bool clearDepth, float depth, unsigned char stencil);
			void clearRenderTarget(const RenderTarget &target, const Color &color);

			void clearDepthStencil(float depth = 1.0f, unsigned char stencilVal = 0);
			void clearDepthStencil(const DepthStencil &stencil, float depth = 1.0f, unsigned char stencilVal = 0);

			void setRenderTarget(const RenderTarget &target, const DepthStencil *stencil = 0);
			void setMultiRenderTarget(const MultiRenderTarget &target, const DepthStencil *stencil = 0);
			void resetRenderTarget();

			void setVertexBuffer(uint slot, const Buffer &buffer);
			void setVertexBuffers(uint startSlot, const MultiBuffer &buffers);
			void setIndexBuffer(const Buffer &buffer);

			void draw(uint vertexCount, uint startVertex);
			void drawIndexed(uint indexCount, uint startIndex, uint startVertex);

			void drawInstanced(uint verticesPerInstance, uint instanceCount, uint startVertex, uint startInstance);
			void drawIndexedInstanced(uint indicesPerInstance, uint instanceCount, uint startIndex, uint startVertex, uint startInstance);

			void present();

			inline ID3D11Device *getDevice() { return mDevice; }
			inline ID3D11DeviceContext *getContext() { return mContext; }
		};
	}
}

#endif