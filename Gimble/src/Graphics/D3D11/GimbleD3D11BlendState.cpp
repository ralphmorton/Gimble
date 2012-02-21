
#include "Graphics/D3D11/GimbleD3D11BlendState.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11BlendState::D3D11BlendState() : 
			mState(0),
			mDirty(true),
			mAlphaToCoverageEnabled(false), 
			mIndependentBlendEnabled(false), 
			mSampleMask(0xffffffff)
		{
			mBlendFactor[0] = 0;
			mBlendFactor[1] = 0;
			mBlendFactor[2] = 0;
			mBlendFactor[3] = 0;

			mBlendInfo[0].BlendEnable = false;
			mBlendInfo[0].SrcBlend = BLEND_ONE;
			mBlendInfo[0].DestBlend = BLEND_ZERO;
			mBlendInfo[0].BlendOp = BLENDOP_ADD;
			mBlendInfo[0].SrcBlendAlpha = BLEND_ONE;
			mBlendInfo[0].DestBlendAlpha = BLEND_ZERO;
			mBlendInfo[0].BlendOpAlpha = BLENDOP_ADD;
			mBlendInfo[0].RenderTargetWriteMask = BLENDWRITEMASK_ENABLE_ALL;

			mBlendInfo[1] = mBlendInfo[0];
			mBlendInfo[2] = mBlendInfo[0];
			mBlendInfo[3] = mBlendInfo[0];
			mBlendInfo[4] = mBlendInfo[0];
			mBlendInfo[5] = mBlendInfo[0];
			mBlendInfo[6] = mBlendInfo[0];
			mBlendInfo[7] = mBlendInfo[0];
		}

		D3D11BlendState::~D3D11BlendState()
		{
			SAFE_RELEASE(mState);
		}

		bool D3D11BlendState::getAlphaToCoverageEnabled()
		{
			return mAlphaToCoverageEnabled;
		}

		bool D3D11BlendState::getIndependentBlendEnabled()
		{
			return mIndependentBlendEnabled;
		}

		uint D3D11BlendState::getMaxRenderTargets()
		{
			return 8;
		}

		D3D11BlendState::RenderTargetBlendInfo D3D11BlendState::getRenderTargetBlendInfo(uint index)
		{
			return mBlendInfo[index];
		}

		void D3D11BlendState::getBlendFactor(float &r, float &g, float &b, float &a)
		{
			r = mBlendFactor[0];
			g = mBlendFactor[1];
			b = mBlendFactor[2];
			a = mBlendFactor[3];
		}

		uint D3D11BlendState::getSampleMask()
		{
			return mSampleMask;
		}
			
		void D3D11BlendState::setAlphaToCoverageEnabled(bool enabled)
		{
			mAlphaToCoverageEnabled = enabled;
			mDirty = true;
		}

		void D3D11BlendState::setIndependentBlendEnabled(bool enabled)
		{
			mIndependentBlendEnabled = enabled;
			mDirty = true;
		}

		void D3D11BlendState::setRenderTargetBlendInfo(uint index, RenderTargetBlendInfo info)
		{
			mBlendInfo[index] = info;
			mDirty = true;
		}

		void D3D11BlendState::setBlendFactor(float r, float g, float b, float a)
		{
			mBlendFactor[0] = r;
			mBlendFactor[1] = g;
			mBlendFactor[2] = b;
			mBlendFactor[3] = a;
			mDirty = true;
		}

		void D3D11BlendState::setSampleMask(uint mask)
		{
			mSampleMask = mask;
			mDirty = true;
		}

		void D3D11BlendState::update()
		{
			if (!mDirty)
				return;

			SAFE_RELEASE(mState);

			mDesc.AlphaToCoverageEnable = mAlphaToCoverageEnabled;
			mDesc.IndependentBlendEnable = mIndependentBlendEnabled;
			
			int max = mIndependentBlendEnabled ? 8 : 1;

			for (int i = -1; ++i < 8;)
			{
				mDesc.RenderTarget[i].BlendEnable = mBlendInfo[i].BlendEnable;
				mDesc.RenderTarget[i].RenderTargetWriteMask = mBlendInfo[i].RenderTargetWriteMask;
				mDesc.RenderTarget[i].BlendOp = D3D11Utils::getBlendOp(mBlendInfo[i].BlendOp);
				mDesc.RenderTarget[i].BlendOpAlpha = D3D11Utils::getBlendOp(mBlendInfo[i].BlendOpAlpha);
				mDesc.RenderTarget[i].DestBlend = D3D11Utils::getBlend(mBlendInfo[i].DestBlend);
				mDesc.RenderTarget[i].DestBlendAlpha = D3D11Utils::getBlend(mBlendInfo[i].DestBlendAlpha);
				mDesc.RenderTarget[i].SrcBlend = D3D11Utils::getBlend(mBlendInfo[i].SrcBlend);
				mDesc.RenderTarget[i].SrcBlendAlpha = D3D11Utils::getBlend(mBlendInfo[i].SrcBlendAlpha);
			}

			ID3D11Device *device = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getDevice();

			HRESULT hr = device->CreateBlendState(&mDesc, &mState);

			mDirty = false;
		}

		void D3D11BlendState::set()
		{
			update();

			assert(mState);

			ID3D11DeviceContext *ctx = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getContext();
			ctx->OMSetBlendState(mState, mBlendFactor, mSampleMask);
		}
	}
}