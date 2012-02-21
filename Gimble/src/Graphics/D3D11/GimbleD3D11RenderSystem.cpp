
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "GimbleRenderWindow.h"
#include "GimbleLog.h"
#include "GimbleResourceManager.h"
#include "Graphics/GimbleColor.h"
#include "Graphics/D3D11/GimbleD3D11TextureLoader.h"
#include "Graphics/D3D11/GimbleD3D11ShaderLoader.h"
#include "Graphics/D3D11/GimbleD3D11ComputeShaderLoader.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"
#include "Graphics/D3D11/GimbleD3D11Buffer.h"
#include "Graphics/D3D11/GimbleD3D11InputLayout.h"
#include "Graphics/D3D11/GimbleD3D11RenderTarget.h"
#include "Graphics/D3D11/GimbleD3D11DepthStencil.h"
#include "Graphics/D3D11/GimbleD3D11DepthStencilState.h"
#include "Graphics/D3D11/GimbleD3D11RasterizerState.h"
#include "Graphics/D3D11/GimbleD3D11BlendState.h"
#include "Graphics/D3D11/GimbleD3D11Sampler.h"
#include "Graphics/D3D11/GimbleD3D11Shader.h"
#include "Graphics/D3D11/GimbleD3D11ShaderResourceGroup.h"
#include "Graphics/GimbleMultiBuffer.h"
#include "Graphics/GimbleMultiRenderTarget.h"
#include "GimbleMath.h"

template<> Gimble::Graphics::D3D11RenderSystem* Gimble::Singleton<Gimble::Graphics::D3D11RenderSystem>::ms_Singleton = 0;

namespace Gimble
{
	namespace Graphics
	{
		D3D11RenderSystem::D3D11RenderSystem() : mHWnd(0), mDevice(0), mSwapChain(0), mRenderTargetView(0), mInputLayout(0), mVertexShader(0), mTexLoader(0), mVertexShaderLoader(0), mPixelShaderLoader(0), mGeometryShaderLoader(0), mHullShaderLoader(0), mDomainShaderLoader(0), mComputeShaderLoader(0)
		{
			Log::getPtr()->info("D3D11 RenderSystem created");
		}

		D3D11RenderSystem::~D3D11RenderSystem()
		{
			if (mRenderWindow && !mRenderWindow->isClosed()) mRenderWindow->destroy();
			delete mRenderWindow;
		}

		bool D3D11RenderSystem::initDevice(bool fullscreen)
		{
			RECT rc;
		    GetClientRect(mHWnd, &rc);
		    UINT width = rc.right - rc.left;
		    UINT height = rc.bottom - rc.top;

			HRESULT hr = S_OK;

			//Create device + swap chain

		    DXGI_SWAP_CHAIN_DESC sd;
		    ZeroMemory(&sd, sizeof(sd));
		    sd.BufferCount = 1;
		    sd.BufferDesc.Width = width;
		    sd.BufferDesc.Height = height;
		    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		    sd.BufferDesc.RefreshRate.Numerator = 60;
		    sd.BufferDesc.RefreshRate.Denominator = 1;
		    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		    sd.OutputWindow = mHWnd;
		    sd.SampleDesc.Count = 1;
		    sd.SampleDesc.Quality = 0;
		    sd.Windowed = TRUE;

			D3D_FEATURE_LEVEL featureLevels[] = 
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};
			
			if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 6, D3D11_SDK_VERSION, &sd, &mSwapChain, &mDevice, &mFeatureLevel, &mContext)))
			{
				Log::getPtr()->error("D3D11CreateDeviceAndSwapChain failed");
				return false;
			}

			ID3D11Texture2D* pBackBuffer;
		    if (FAILED(hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
			{
				Log::getPtr()->error("SwapChain->GetBuffer failed");
				return false;
			}

		    if (FAILED(hr = mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mRenderTargetView)))
			{
				pBackBuffer->Release();
				Log::getPtr()->error("Device->CreateRenderTargetView failed");
				return false;
			}
		    
		    pBackBuffer->Release();

			//Depth State

			mDepthStateDesc.DepthEnable = true;
			mDepthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			mDepthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

			mDepthStateDesc.StencilEnable = true;
			mDepthStateDesc.StencilReadMask = 0xFF;
			mDepthStateDesc.StencilWriteMask = 0xFF;

			mDepthStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			mDepthStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			mDepthStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			mDepthStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			mDepthStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			mDepthStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			mDepthStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			mDepthStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			
			mDevice->CreateDepthStencilState(&mDepthStateDesc, &mDepthState);
			mContext->OMSetDepthStencilState(mDepthState, 1);

			//Depth Stencil

			ID3D11Texture2D* pDepthStencil;

			D3D11_TEXTURE2D_DESC ddesc;
		    ddesc.Width = width;
		    ddesc.Height = height;
		    ddesc.MipLevels = 1;
		    ddesc.ArraySize = 1;
		    ddesc.Format = DXGI_FORMAT_D32_FLOAT;
		    ddesc.SampleDesc.Count = 1;
		    ddesc.SampleDesc.Quality = 0;
		    ddesc.Usage = D3D11_USAGE_DEFAULT;
		    ddesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		    ddesc.CPUAccessFlags = 0;
		    ddesc.MiscFlags = 0;

		    if(FAILED(mDevice->CreateTexture2D(&ddesc, NULL, &pDepthStencil)))
			{
				Log::getPtr()->error("Device->CreateTexture2D (Depth Stencil) failed");
				return false;
			}

		    D3D11_DEPTH_STENCIL_VIEW_DESC dsdesc;
			ZeroMemory( &dsdesc, sizeof(dsdesc) );
		    dsdesc.Format = ddesc.Format;
		    dsdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		    dsdesc.Texture2D.MipSlice = 0;
			
		    if(FAILED(mDevice->CreateDepthStencilView(pDepthStencil, &dsdesc, &mDepthStencilView)))
			{
				Log::getPtr()->error("Device->CreateDepthStencilView failed");
				return false;
			}

		    mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

		    D3D11_VIEWPORT vp;
		    vp.Width = (float)width;
		    vp.Height = (float)height;
		    vp.MinDepth = 0.0f;
		    vp.MaxDepth = 1.0f;
		    vp.TopLeftX = 0;
		    vp.TopLeftY = 0;
		    mContext->RSSetViewports(1, &vp);

			//Rasterizer state

			mRasterStateDesc.CullMode = D3D11_CULL_BACK;
			mRasterStateDesc.FillMode = D3D11_FILL_SOLID;
			mRasterStateDesc.FrontCounterClockwise = true;
			mRasterStateDesc.DepthBias = false;
			mRasterStateDesc.DepthBiasClamp = 0;
			mRasterStateDesc.SlopeScaledDepthBias = 0;
			mRasterStateDesc.DepthClipEnable = true;
			mRasterStateDesc.ScissorEnable = false;
			mRasterStateDesc.MultisampleEnable = false;
			mRasterStateDesc.AntialiasedLineEnable = true;
			
			mDevice->CreateRasterizerState(&mRasterStateDesc, &mRasterState);
			mContext->RSSetState(mRasterState);

			mTexLoader = new D3D11TextureLoader();
			mVertexShaderLoader = new D3D11VertexShaderLoader();
			mPixelShaderLoader = new D3D11PixelShaderLoader();
			mGeometryShaderLoader = new D3D11GeometryShaderLoader();
			mHullShaderLoader = new D3D11HullShaderLoader();
			mDomainShaderLoader = new D3D11DomainShaderLoader();
			mComputeShaderLoader = new D3D11ComputeShaderLoader();

			ResourceManager *resman = ResourceManager::getPtr();
			resman->addLoader(mTexLoader);
			resman->addLoader(mVertexShaderLoader);
			resman->addLoader(mPixelShaderLoader);
			resman->addLoader(mGeometryShaderLoader);
			resman->addLoader(mHullShaderLoader);
			resman->addLoader(mDomainShaderLoader);
			resman->addLoader(mComputeShaderLoader);

			setPrimitiveTopology(Graphics::PRIMITIVETOPOLOGY_TRIANGLELIST);

			return true;
		}

		Gimble::RenderWindow *D3D11RenderSystem::init(const string title, uint width, uint height, bool fullscreen)
		{
			Log::getPtr()->info("D3D11 RenderSystem initialization");

			mWidth = width;
			mHeight = height;

			mRenderWindow = new Gimble::RenderWindow(title, width, height, fullscreen);
			mHWnd = mRenderWindow->create();

			if (!initDevice(fullscreen))
			{
				Log::getPtr()->error("D3D11 RenderSystem initialization failed");
				mRenderWindow->destroy();
				delete mRenderWindow;
				return 0;
			};

			Log::getPtr()->info("D3D11 RenderSystem initialization completed");
			
			return mRenderWindow;
		}

		void D3D11RenderSystem::shutdown()
		{
			if (mDevice) mContext->ClearState();

			ResourceManager *resman = ResourceManager::getPtr();
			resman->removeLoader(mTexLoader);
			resman->removeLoader(mVertexShaderLoader);
			resman->removeLoader(mPixelShaderLoader);
			resman->removeLoader(mGeometryShaderLoader);
			resman->removeLoader(mHullShaderLoader);
			resman->removeLoader(mDomainShaderLoader);
			resman->removeLoader(mComputeShaderLoader);

			delete mTexLoader;
			delete mVertexShaderLoader;
			delete mPixelShaderLoader;
			delete mGeometryShaderLoader;
			delete mHullShaderLoader;
			delete mDomainShaderLoader;
			delete mComputeShaderLoader;

			SAFE_RELEASE(mRenderTargetView);
			SAFE_RELEASE(mDepthStencilView);
			SAFE_RELEASE(mSwapChain);
			SAFE_RELEASE(mContext);
			SAFE_RELEASE(mDevice);
		}

		Buffer *D3D11RenderSystem::createBuffer(Buffer::BufferType type, Buffer::BufferUsage usage, unsigned int size, unsigned int stride, void *data)
		{
			return D3D11Buffer::create(type, usage, size, stride, data, mDevice);
		}

		RenderTarget *D3D11RenderSystem::createRenderTarget(unsigned int width, unsigned int height, PixelFormat format)
		{
			return D3D11RenderTarget::create(mDevice, width, height, format);
		}

		DepthStencil *D3D11RenderSystem::createDepthStencil(unsigned int width, unsigned int height, PixelFormat textureFormat, PixelFormat depthFormat, PixelFormat resourceFormat)
		{
			return D3D11DepthStencil::create(mDevice, width, height, textureFormat, depthFormat, resourceFormat);
		}

		Texture *D3D11RenderSystem::createTexture(uint width, uint height, uint mipLevels, PixelFormat format, Texture::TextureAccess access)
		{
			return D3D11Texture::create(mDevice, width, height, mipLevels, format, access);
		}

		DepthStencilState *D3D11RenderSystem::createDepthStencilState()
		{
			return new D3D11DepthStencilState();
		}

		void D3D11RenderSystem::setDepthStencilState(DepthStencilState &state)
		{
			D3D11DepthStencilState *state11 = static_cast<D3D11DepthStencilState*>(&state);
			state11->set();
		}

		RasterizerState *D3D11RenderSystem::createRasterizerState()
		{
			return new D3D11RasterizerState();
		}

		void D3D11RenderSystem::setRasterizerState(RasterizerState &state)
		{
			D3D11RasterizerState *state11 = static_cast<D3D11RasterizerState*>(&state);
			state11->set();
		}

		BlendState *D3D11RenderSystem::createBlendState()
		{
			return new D3D11BlendState();
		}

		void D3D11RenderSystem::setBlendState(BlendState &state)
		{
			D3D11BlendState *state11 = static_cast<D3D11BlendState*>(&state);
			state11->set();
		}

		Sampler *D3D11RenderSystem::createSampler(Sampler::Filter filter, Sampler::AddressMode addressU, Sampler::AddressMode addressV, Sampler::AddressMode addressW, float mipLODBias, uint maxAnistropy, ComparisonFunc comparisonFunc, float minLOD, float maxLOD, float borderR, float borderG, float borderB, float borderA)
		{
			return new D3D11Sampler(
				filter,
				addressU,
				addressV,
				addressW,
				mipLODBias,
				maxAnistropy,
				comparisonFunc,
				minLOD,
				maxLOD,
				borderR,
				borderG,
				borderB,
				borderA);
		}

		void D3D11RenderSystem::setVertexSamplers(uint startSlot, uint numSamplers, Sampler **samplers)
		{
			uint start = Math::Clamp(startSlot, 0u, 15u);
			uint num = Math::Min(16u, numSamplers);

			ID3D11SamplerState **samplers11 = new ID3D11SamplerState*[num];

			for (int i = -1; ++i < num;)
				samplers11[i] = static_cast<D3D11Sampler*>(samplers[i])->getSampler();

			mContext->VSSetSamplers(start, num, samplers11);

			delete [] samplers11;
		}

		void D3D11RenderSystem::setPixelSamplers(uint startSlot, uint numSamplers, Sampler **samplers)
		{
			uint start = Math::Clamp(startSlot, 0u, 15u);
			uint num = Math::Min(16u, numSamplers);

			ID3D11SamplerState **samplers11 = new ID3D11SamplerState*[num];

			for (int i = -1; ++i < num;)
				samplers11[i] = static_cast<D3D11Sampler*>(samplers[i])->getSampler();

			mContext->PSSetSamplers(start, num, samplers11);

			delete [] samplers11;
		}

		void D3D11RenderSystem::setGeometrySamplers(uint startSlot, uint numSamplers, Sampler **samplers)
		{
			uint start = Math::Clamp(startSlot, 0u, 15u);
			uint num = Math::Min(16u, numSamplers);

			ID3D11SamplerState **samplers11 = new ID3D11SamplerState*[num];

			for (int i = -1; ++i < num;)
				samplers11[i] = static_cast<D3D11Sampler*>(samplers[i])->getSampler();

			mContext->GSSetSamplers(start, num, samplers11);

			delete [] samplers11;
		}

		void D3D11RenderSystem::setVertexConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers)
		{
			uint max = (uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1;
			uint start = Math::Clamp(startSlot, 0u, max);
			uint num = Math::Min((uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, numBuffers);

			ID3D11Buffer **buffers11 = new ID3D11Buffer*[num];

			for (int i = -1; ++i < num;)
				buffers11[i] = static_cast<D3D11Buffer*>(buffers[i])->getD3D11Buffer();

			mContext->VSSetConstantBuffers(start, num, buffers11);
			
			delete [] buffers11;
		}

		void D3D11RenderSystem::setPixelConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers)
		{
			uint max = (uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1;
			uint start = Math::Clamp(startSlot, 0u, max);
			uint num = Math::Min((uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, numBuffers);

			ID3D11Buffer **buffers11 = new ID3D11Buffer*[num];

			for (int i = -1; ++i < num;)
				buffers11[i] = static_cast<D3D11Buffer*>(buffers[i])->getD3D11Buffer();

			mContext->PSSetConstantBuffers(start, num, buffers11);
			
			delete [] buffers11;
		}

		void D3D11RenderSystem::setGeometryConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers)
		{
			uint max = (uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1;
			uint start = Math::Clamp(startSlot, 0u, max);
			uint num = Math::Min((uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, numBuffers);

			ID3D11Buffer **buffers11 = new ID3D11Buffer*[num];

			for (int i = -1; ++i < num;)
				buffers11[i] = static_cast<D3D11Buffer*>(buffers[i])->getD3D11Buffer();

			mContext->GSSetConstantBuffers(start, num, buffers11);
			
			delete [] buffers11;
		}

		void D3D11RenderSystem::setHullConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers)
		{
			uint max = (uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1;
			uint start = Math::Clamp(startSlot, 0u, max);
			uint num = Math::Min((uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, numBuffers);

			ID3D11Buffer **buffers11 = new ID3D11Buffer*[num];

			for (int i = -1; ++i < num;)
				buffers11[i] = static_cast<D3D11Buffer*>(buffers[i])->getD3D11Buffer();

			mContext->HSSetConstantBuffers(start, num, buffers11);
			
			delete [] buffers11;
		}

		void D3D11RenderSystem::setDomainConstantBuffers(uint startSlot, uint numBuffers, Buffer **buffers)
		{
			uint max = (uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1;
			uint start = Math::Clamp(startSlot, 0u, max);
			uint num = Math::Min((uint)D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, numBuffers);

			ID3D11Buffer **buffers11 = new ID3D11Buffer*[num];

			for (int i = -1; ++i < num;)
				buffers11[i] = static_cast<D3D11Buffer*>(buffers[i])->getD3D11Buffer();

			mContext->DSSetConstantBuffers(start, num, buffers11);
			
			delete [] buffers11;
		}

		ShaderResourceGroup *D3D11RenderSystem::createShaderResourceGroup(unsigned char initialSize)
		{
			return new D3D11ShaderResourceGroup(initialSize);
		}

		void D3D11RenderSystem::setVertexShaderResources(ShaderResourceGroup &group)
		{
			D3D11ShaderResourceGroup *group11 = static_cast<D3D11ShaderResourceGroup*>(&group);
			mContext->VSSetShaderResources(0, group11->getNumSRVs(), group11->getSRVs());
		}

		void D3D11RenderSystem::setPixelShaderResources(ShaderResourceGroup &group)
		{
			D3D11ShaderResourceGroup *group11 = static_cast<D3D11ShaderResourceGroup*>(&group);
			mContext->PSSetShaderResources(0, group11->getNumSRVs(), group11->getSRVs());
		}

		void D3D11RenderSystem::setGeometryShaderResources(ShaderResourceGroup &group)
		{
			D3D11ShaderResourceGroup *group11 = static_cast<D3D11ShaderResourceGroup*>(&group);
			mContext->GSSetShaderResources(0, group11->getNumSRVs(), group11->getSRVs());
		}

		void D3D11RenderSystem::setHullShaderResources(ShaderResourceGroup &group)
		{
			D3D11ShaderResourceGroup *group11 = static_cast<D3D11ShaderResourceGroup*>(&group);
			mContext->HSSetShaderResources(0, group11->getNumSRVs(), group11->getSRVs());
		}

		void D3D11RenderSystem::setDomainShaderResources(ShaderResourceGroup &group)
		{
			D3D11ShaderResourceGroup *group11 = static_cast<D3D11ShaderResourceGroup*>(&group);
			mContext->DSSetShaderResources(0, group11->getNumSRVs(), group11->getSRVs());
		}

		InputLayout *D3D11RenderSystem::createInputLayout()
		{
			return new D3D11InputLayout;
		}

		void D3D11RenderSystem::setInputLayout(InputLayout &decl)
		{
			mInputLayout = static_cast<D3D11InputLayout*>(&decl);
			if (mVertexShader)
			{
				ID3D11InputLayout *layout = mInputLayout ? mInputLayout->getLayout(mVertexShader) : 0;
				mContext->IASetInputLayout(layout);
			}
		}

		void D3D11RenderSystem::setVertexShader(VertexShader *shader)
		{
			if (!shader)
			{
				mContext->VSSetShader(NULL, NULL, 0);
				return;
			}

			D3D11VertexShader *shader11 = static_cast<D3D11VertexShader*>(shader);
			mVertexShader = shader11;

			mContext->VSSetShader(shader11->getShader(), NULL, 0);

			ID3D11InputLayout *layout = mInputLayout ? mInputLayout->getLayout(mVertexShader) : 0;
			mContext->IASetInputLayout(layout);
		}

		void D3D11RenderSystem::setPixelShader(PixelShader *shader)
		{
			if (!shader)
			{
				mContext->PSSetShader(NULL, NULL, 0);
				return;
			}

			D3D11PixelShader *shader11 = static_cast<D3D11PixelShader*>(shader);
			mContext->PSSetShader(shader11->getShader(), NULL, 0);
		}

		void D3D11RenderSystem::setGeometryShader(GeometryShader *shader)
		{
			if (!shader)
			{
				mContext->GSSetShader(NULL, NULL, 0);
				return;
			}

			D3D11GeometryShader *shader11 = static_cast<D3D11GeometryShader*>(shader);
			mContext->GSSetShader(shader11->getShader(), NULL, 0);
		}

		void D3D11RenderSystem::setHullShader(HullShader *shader)
		{
			if (!shader)
			{
				mContext->HSSetShader(NULL, NULL, 0);
				return;
			}

			D3D11HullShader *shader11 = static_cast<D3D11HullShader*>(shader);
			mContext->HSSetShader(shader11->getShader(), NULL, 0);
		}

		void D3D11RenderSystem::setDomainShader(DomainShader *shader)
		{
			if (!shader)
			{
				mContext->DSSetShader(NULL, NULL, 0);
				return;
			}

			D3D11DomainShader *shader11 = static_cast<D3D11DomainShader*>(shader);
			mContext->DSSetShader(shader11->getShader(), NULL, 0);
		}

		void D3D11RenderSystem::setPrimitiveTopology(PrimitiveTopology topology)
		{
			mContext->IASetPrimitiveTopology(D3D11Utils::getPrimitiveTopology(topology));
		}

		void D3D11RenderSystem::clear(const Color &color, bool clearDepth, float depth, unsigned char stencil)
		{
			float c[4];
			c[0] = color.r;
			c[1] = color.g;
			c[2] = color.b;
			c[3] = color.a;

			mContext->ClearRenderTargetView(mRenderTargetView, c);  
			
			if (clearDepth)
				mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, depth, stencil);
		}

		void D3D11RenderSystem::clearRenderTarget(const RenderTarget &target, const Color &color)
		{
			float c[4];
			c[0] = color.r;
			c[1] = color.g;
			c[2] = color.b;
			c[3] = color.a;

			mContext->ClearRenderTargetView(static_cast<const D3D11RenderTarget*>(&target)->getRenderTarget(), c);
		}

		void D3D11RenderSystem::clearDepthStencil(float depth, unsigned char stencilVal)
		{
			mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, depth, stencilVal);
		}

		void D3D11RenderSystem::clearDepthStencil(const DepthStencil &stencil, float depth, unsigned char stencilVal)
		{
			mContext->ClearDepthStencilView(static_cast<const D3D11DepthStencil*>(&stencil)->getDepthStencil(), D3D11_CLEAR_DEPTH, depth, stencilVal); //TODO: allow clear stencil
		}

		void D3D11RenderSystem::setRenderTarget(const RenderTarget &target, const DepthStencil *stencil)
		{
			ID3D11RenderTargetView *targ = static_cast<const D3D11RenderTarget *>(&target)->getRenderTarget();
			ID3D11DepthStencilView *sten = stencil ? static_cast<const D3D11DepthStencil *>(stencil)->getDepthStencil() : 0;

			mContext->OMSetRenderTargets(1, &targ, sten);
		}

		void D3D11RenderSystem::setMultiRenderTarget(const MultiRenderTarget &target, const DepthStencil *stencil)
		{
			uint count = target.count();

			ID3D11RenderTargetView **views = new ID3D11RenderTargetView *[count];
			for (uint i = 0; i < count; i++)
				views[i] = static_cast<D3D11RenderTarget *>(target.get(i))->getRenderTarget();

			ID3D11DepthStencilView *sten = stencil ? static_cast<const D3D11DepthStencil *>(stencil)->getDepthStencil() : 0;

			mContext->OMSetRenderTargets(count, views, sten);

			delete [] views;
		}

		void D3D11RenderSystem::resetRenderTarget()
		{
			mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
		}

		void D3D11RenderSystem::setVertexBuffer(uint slot, const Buffer &buffer)
		{
			const D3D11Buffer *buf = static_cast<const D3D11Buffer *>(&buffer);

			ID3D11Buffer *dbuf = buf->getD3D11Buffer();
			unsigned int stride = buf->getStride();

			unsigned int *offsets = new unsigned int[1];
			offsets[0] = 0;

			mContext->IASetVertexBuffers(slot, 1, &dbuf, &stride, offsets);

			delete [] offsets;
		}

		void D3D11RenderSystem::setVertexBuffers(uint startSlot, const MultiBuffer &buffers)
		{
			uint count = buffers.count();

			ID3D11Buffer **bufs = new ID3D11Buffer*[count];
			unsigned int *strides = new unsigned int[count];
			unsigned int *offsets = new unsigned int[count];

			for (unsigned int i = 0; i < count; i++)
			{
				D3D11Buffer *dbuf = static_cast<D3D11Buffer *>(buffers.get(i));
				bufs[i] = dbuf->getD3D11Buffer();
				strides[i] = dbuf->getStride();
				offsets[i] = 0;
			}

			mContext->IASetVertexBuffers(startSlot, count, bufs, strides, offsets);

			delete [] strides;
			delete [] offsets;
		}

		void D3D11RenderSystem::setIndexBuffer(const Buffer &buffer)
		{
			unsigned int stride = buffer.getStride();

			DXGI_FORMAT format;
			
			if (stride == 2)
				format = DXGI_FORMAT_R16_UINT;
			else 
				format = DXGI_FORMAT_R32_UINT;

			const D3D11Buffer *buf = static_cast<const D3D11Buffer *>(&buffer);

			ID3D11Buffer *dbuf = buf->getD3D11Buffer();

			mContext->IASetIndexBuffer(dbuf, format, 0);
		}

		void D3D11RenderSystem::draw(uint vertexCount, uint startVertex)
		{
			mContext->Draw(vertexCount, startVertex);
		}

		void D3D11RenderSystem::drawIndexed(uint indexCount, uint startIndex, uint startVertex)
		{
			mContext->DrawIndexed(indexCount, startIndex, startVertex);
		}

		void D3D11RenderSystem::drawInstanced(uint verticesPerInstance, uint instanceCount, uint startVertex, uint startInstance)
		{
			mContext->DrawInstanced(verticesPerInstance, instanceCount, startVertex, startInstance);
		}

		void D3D11RenderSystem::drawIndexedInstanced(uint indicesPerInstance, uint instanceCount, uint startIndex, uint startVertex, uint startInstance)
		{
			mContext->DrawIndexedInstanced(indicesPerInstance, instanceCount, startIndex, startVertex, startInstance);
		}

		void D3D11RenderSystem::present()
		{
			mSwapChain->Present(0, 0);
		}
	}
}