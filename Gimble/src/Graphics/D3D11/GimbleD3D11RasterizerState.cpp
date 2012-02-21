
#include "Graphics/D3D11/GimbleD3D11RasterizerState.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11RasterizerState::D3D11RasterizerState() :
			mState(0),
			mDirty(true),
			mFillMode(FILLMODE_SOLID),
			mCullMode(CULLMODE_BACK),
			mFrontCCW(true),
			mDepthBias(0),
			mDepthBiasClamp(0),
			mSlopeScaledDepthBias(0),
			mDepthClipEnabled(true),
			mScissorEnabled(false),
			mMultisampleEnabled(false),
			mAntialiasedLineEnabled(true)
		{

		}

		D3D11RasterizerState::~D3D11RasterizerState()
		{
			SAFE_RELEASE(mState);
		}

		D3D11RasterizerState::FillMode D3D11RasterizerState::getFillMode()
		{
			return mFillMode;
		}

		D3D11RasterizerState::CullMode D3D11RasterizerState::getCullMode()
		{
			return mCullMode;
		}

		bool D3D11RasterizerState::getFrontCounterClockwise()
		{
			return mFrontCCW;
		}

		int D3D11RasterizerState::getDepthBias()
		{
			return mDepthBias;
		}

		float D3D11RasterizerState::getDepthBiasClamp()
		{
			return mDepthBiasClamp;
		}

		float D3D11RasterizerState::getSlopeScaledDepthBias()
		{
			return mSlopeScaledDepthBias;
		}

		bool D3D11RasterizerState::getDepthClipEnabled()
		{
			return mDepthClipEnabled;
		}

		bool D3D11RasterizerState::getScissorEnabled()
		{
			return mScissorEnabled;
		}

		bool D3D11RasterizerState::getMultisampleEnabled()
		{
			return mMultisampleEnabled;
		}

		bool D3D11RasterizerState::getAntialiasedLineEnabled()
		{
			return mAntialiasedLineEnabled;
		}

		void D3D11RasterizerState::setFillMode(FillMode mode)
		{
			mFillMode = mode;
		}

		void D3D11RasterizerState::setCullMode(CullMode mode)
		{
			mCullMode = mode;
			mDirty = true;
		}

		void D3D11RasterizerState::setFrontCounterClockwise(bool y)
		{
			mFrontCCW = y;
			mDirty = true;
		}

		void D3D11RasterizerState::setDepthBias(int bias)
		{
			mDepthBias = bias;
			mDirty = true;
		}

		void D3D11RasterizerState::setDepthBiasClamp(float clamp)
		{
			mDepthBiasClamp = clamp;
			mDirty = true;
		}

		void D3D11RasterizerState::setSlopeScaledDepthBias(float bias)
		{
			mSlopeScaledDepthBias = bias;
			mDirty = true;
		}

		void D3D11RasterizerState::setDepthClipEnabled(bool enabled)
		{
			mDepthClipEnabled = enabled;
			mDirty = true;
		}

		void D3D11RasterizerState::setScissorEnabled(bool enabled)
		{
			mScissorEnabled = enabled;
			mDirty = true;
		}

		void D3D11RasterizerState::setMultisampleEnabled(bool enabled)
		{
			mMultisampleEnabled = enabled;
			mDirty = true;
		}

		void D3D11RasterizerState::setAntialiasedLineEnabled(bool enabled)
		{
			mAntialiasedLineEnabled = enabled;
			mDirty = true;
		}

		void D3D11RasterizerState::update()
		{
			if (!mDirty)
				return;

			SAFE_RELEASE(mState);

			mDesc.FillMode = D3D11Utils::getFillMode(mFillMode);
			mDesc.CullMode = D3D11Utils::getCullMode(mCullMode);
			mDesc.FrontCounterClockwise = mFrontCCW;
			mDesc.DepthBias = mDepthBias;
			mDesc.DepthBiasClamp = mDepthBiasClamp;
			mDesc.SlopeScaledDepthBias = mSlopeScaledDepthBias;
			mDesc.DepthClipEnable = mDepthClipEnabled;
			mDesc.ScissorEnable = mScissorEnabled;
			mDesc.MultisampleEnable = mMultisampleEnabled;
			mDesc.AntialiasedLineEnable = mAntialiasedLineEnabled;

			ID3D11Device *device = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getDevice();

			HRESULT hr = device->CreateRasterizerState(&mDesc, &mState);

			mDirty = false;
		}

		void D3D11RasterizerState::set()
		{
			update();

			assert(mState);

			ID3D11DeviceContext *ctx = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getContext();
			ctx->RSSetState(mState);
		}
	}
}