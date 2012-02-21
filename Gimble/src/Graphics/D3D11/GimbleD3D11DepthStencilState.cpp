
#include "Graphics/D3D11/GimbleD3D11DepthStencilState.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "Graphics/D3D11/GimbleD3D11Utils.h"

namespace Gimble
{
	namespace Graphics
	{
		D3D11DepthStencilState::D3D11DepthStencilState() : 
			mState(0),
			mDirty(true),
			mDepthEnabled(true), 
			mStencilEnabled(true), 
			mDepthWriteMask(DEPTHWRITEMASK_ALL),
			mDepthComparisonFunc(COMP_LESS),
			mStencilReadMask(0xFF),
			mStencilWriteMask(0xFF),
			mStencilRef(0)
		{
			mFrontFace.StencilDepthFailOp = STENCILOP_INCR;
			mFrontFace.StencilFailOp = STENCILOP_KEEP;
			mFrontFace.StencilFunc = COMP_ALWAYS;
			mFrontFace.StencilPassOp = STENCILOP_KEEP;

			mBackFace.StencilDepthFailOp = STENCILOP_DECR;
			mBackFace.StencilFailOp = STENCILOP_KEEP;
			mBackFace.StencilFunc = COMP_ALWAYS;
			mBackFace.StencilPassOp = STENCILOP_KEEP;
		}

		D3D11DepthStencilState::~D3D11DepthStencilState()
		{
			SAFE_RELEASE(mState);
		}

		bool D3D11DepthStencilState::getDepthEnabled()
		{
			return mDepthEnabled;
		}

		DepthStencilState::DepthWriteMask D3D11DepthStencilState::getDepthWriteMask()
		{
			return mDepthWriteMask;
		}

		ComparisonFunc D3D11DepthStencilState::getDepthFunc()
		{
			return mDepthComparisonFunc;
		}

		bool D3D11DepthStencilState::getStencilEnabled()
		{
			return mStencilEnabled;
		}

		unsigned char D3D11DepthStencilState::getStencilReadMask()
		{
			return mStencilReadMask;
		}

		unsigned char D3D11DepthStencilState::getStencilWritemask()
		{
			return mStencilWriteMask;
		}

		DepthStencilState::StencilInfo D3D11DepthStencilState::getFrontFace()
		{
			return mFrontFace;
		}

		DepthStencilState::StencilInfo D3D11DepthStencilState::getBackFace()
		{
			return mBackFace;
		}

		uint D3D11DepthStencilState::getStencilRef()
		{
			return mStencilRef;
		}

		void D3D11DepthStencilState::setDepthEnabled(bool enabled)
		{
			mDepthEnabled = enabled;
			mDirty = true;
		}

		void D3D11DepthStencilState::setDepthWriteMask(DepthStencilState::DepthWriteMask mask)
		{
			mDepthWriteMask = mask;
			mDirty = true;
		}

		void D3D11DepthStencilState::setDepthFunc(ComparisonFunc func)
		{
			mDepthComparisonFunc = func;
			mDirty = true;
		}

		void D3D11DepthStencilState::setStencilEnabled(bool enabled)
		{
			mStencilEnabled = true;
			mDirty = true;
		}

		void D3D11DepthStencilState::setStencilReadMask(unsigned char mask)
		{
			mStencilReadMask = mask;
			mDirty = true;
		}

		void D3D11DepthStencilState::setStencilWriteMask(unsigned char mask)
		{
			mStencilWriteMask = mask;
			mDirty = true;
		}

		void D3D11DepthStencilState::setFrontFace(DepthStencilState::StencilInfo face)
		{
			mFrontFace = face;
			mDirty = true;
		}

		void D3D11DepthStencilState::setBackFace(DepthStencilState::StencilInfo face)
		{
			mBackFace = face;
			mDirty = true;
		}

		void D3D11DepthStencilState::setStencilRef(uint val)
		{
			mStencilRef = val;
			mDirty = true;
		}

		void D3D11DepthStencilState::update()
		{
			if (!mDirty)
				return;

			SAFE_RELEASE(mState);

			D3D11_DEPTH_STENCILOP_DESC fdesc;
			fdesc.StencilDepthFailOp = D3D11Utils::getStencilOp(mFrontFace.StencilDepthFailOp);
			fdesc.StencilFailOp = D3D11Utils::getStencilOp(mFrontFace.StencilFailOp);
			fdesc.StencilFunc = D3D11Utils::getComparisonFunc(mFrontFace.StencilFunc);
			fdesc.StencilPassOp = D3D11Utils::getStencilOp(mFrontFace.StencilPassOp);

			D3D11_DEPTH_STENCILOP_DESC bdesc;
			bdesc.StencilDepthFailOp = D3D11Utils::getStencilOp(mBackFace.StencilDepthFailOp);
			bdesc.StencilFailOp = D3D11Utils::getStencilOp(mBackFace.StencilFailOp);
			bdesc.StencilFunc = D3D11Utils::getComparisonFunc(mBackFace.StencilFunc);
			bdesc.StencilPassOp = D3D11Utils::getStencilOp(mBackFace.StencilPassOp);

			mDesc.DepthEnable = mDepthEnabled;
			mDesc.DepthWriteMask = D3D11Utils::getDepthWriteMask(mDepthWriteMask);
			mDesc.DepthFunc = D3D11Utils::getComparisonFunc(mDepthComparisonFunc);
			mDesc.StencilEnable = mStencilEnabled;
			mDesc.StencilReadMask = mStencilReadMask;
			mDesc.StencilWriteMask = mStencilWriteMask;
			mDesc.FrontFace = fdesc;
			mDesc.BackFace = bdesc;

			ID3D11Device *device = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getDevice();

			device->CreateDepthStencilState(&mDesc, &mState);

			mDirty = false;
		}

		void D3D11DepthStencilState::set()
		{
			update();

			assert(mState);

			ID3D11DeviceContext *ctx = static_cast<D3D11RenderSystem*>(D3D11RenderSystem::getPtr())->getContext();
			ctx->OMSetDepthStencilState(mState, mStencilRef);
		}
	}
}