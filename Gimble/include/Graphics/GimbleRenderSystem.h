
#ifndef _GIMBLE_RENDERSYSTEM_
#define _GIMBLE_RENDERSYSTEM_

#include "GimbleReqs.h"
#include "Graphics/GimbleBuffer.h"
#include "Graphics/GimbleTexture.h"
#include "Graphics/GimbleSampler.h"
#include "GimbleSingleton.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API RenderSystem : public Singleton<RenderSystem>
		{
		public:
			virtual RenderWindow *init(const string title, uint width, uint height, bool fullscreen) = 0;
			virtual void shutdown() = 0;

			virtual uint getWidth() const = 0;
			virtual uint getHeight() const = 0;

			virtual const RenderWindow *getRenderWindow() const = 0;

			virtual Buffer *createBuffer(Buffer::BufferType type, Buffer::BufferUsage usage, uint size, uint stride, void *data) = 0;
			virtual RenderTarget *createRenderTarget(uint width, uint height, PixelFormat format) = 0;
			virtual DepthStencil *createDepthStencil(uint width, uint height, PixelFormat textureFormat, PixelFormat depthFormat, PixelFormat resourceFormat) = 0;
			virtual Texture *createTexture(uint width, uint height, uint mipLevels, PixelFormat format, Texture::TextureAccess access) = 0;

			virtual DepthStencilState *createDepthStencilState() = 0;
			virtual void setDepthStencilState(DepthStencilState &state) = 0;

			virtual RasterizerState *createRasterizerState() = 0;
			virtual void setRasterizerState(RasterizerState &state) = 0;

			virtual BlendState *createBlendState() = 0;
			virtual void setBlendState(BlendState &state) = 0;

			virtual Sampler *createSampler(Sampler::Filter filter, Sampler::AddressMode addressU, Sampler::AddressMode addressV, Sampler::AddressMode addressW, float mipLODBias, uint maxAnistropy, ComparisonFunc comparisonFunc, float minLOD, float maxLOD, float borderR, float borderG, float borderB, float borderA) = 0;
			virtual void setVertexSamplers(uint startSlot, uint numSamplers, Sampler **samplers) = 0;
			virtual void setPixelSamplers(uint startSlot, uint numSamplers, Sampler **samplers) = 0;
			virtual void setGeometrySamplers(uint startSlot, uint numSamplers, Sampler **samplers) = 0;

			virtual void setVertexConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers) = 0;
			virtual void setPixelConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers) = 0;
			virtual void setGeometryConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers) = 0;
			virtual void setHullConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers) = 0;
			virtual void setDomainConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers) = 0;

			virtual ShaderResourceGroup *createShaderResourceGroup(unsigned char initialSize) = 0;
			virtual void setVertexShaderResources(ShaderResourceGroup &group) = 0;
			virtual void setPixelShaderResources(ShaderResourceGroup &group) = 0;
			virtual void setGeometryShaderResources(ShaderResourceGroup &group) = 0;
			virtual void setHullShaderResources(ShaderResourceGroup &group) = 0;
			virtual void setDomainShaderResources(ShaderResourceGroup &group) = 0;

			virtual InputLayout *createInputLayout() = 0;
			virtual void setInputLayout(InputLayout &decl) = 0;

			virtual void setVertexShader(VertexShader *shader) = 0;
			virtual void setPixelShader(PixelShader *shader) = 0;
			virtual void setGeometryShader(GeometryShader *shader) = 0;
			virtual void setHullShader(HullShader *shader) = 0;
			virtual void setDomainShader(DomainShader *shader) = 0;

			virtual void setPrimitiveTopology(PrimitiveTopology topology) = 0;

			virtual void clear(const Color &color, bool clearDepth, float depth, unsigned char stencil) = 0;
			virtual void clearRenderTarget(const RenderTarget &target, const Color &color) = 0;

			virtual void clearDepthStencil(float depth = 1.0f, unsigned char stencilVal = 0) = 0;
			virtual void clearDepthStencil(const DepthStencil &stencil, float depth = 1.0f, unsigned char stencilVal = 0) = 0;

			virtual void setRenderTarget(const RenderTarget &target, const DepthStencil *stencil = 0) = 0;
			virtual void setMultiRenderTarget(const MultiRenderTarget &target, const DepthStencil *stencil = 0) = 0;
			virtual void resetRenderTarget() = 0;

			virtual void setVertexBuffer(uint slot, const Buffer &buffer) = 0;
			virtual void setVertexBuffers(uint startSlot, const MultiBuffer &buffers) = 0;
			virtual void setIndexBuffer(const Buffer &buffer) = 0;

			virtual void draw(uint vertexCount, uint startVertex) = 0;
			virtual void drawIndexed(uint indexCount, uint startIndex, uint startVertex) = 0;

			virtual void drawInstanced(uint verticesPerInstance, uint instanceCount, uint startVertex, uint startInstance) = 0;
			virtual void drawIndexedInstanced(uint indicesPerInstance, uint instanceCount, uint startIndex, uint startVertex, uint startInstance) = 0;

			virtual void present() = 0;
		};
	}
}

#endif