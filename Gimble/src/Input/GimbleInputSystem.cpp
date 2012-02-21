
#include "Input/GimbleInputSystem.h"
#include "Graphics/GimbleRenderSystem.h"
#include "GimbleRenderWindow.h"
#include "GimbleLog.h"

template<> Gimble::Input::InputSystem* Gimble::Singleton<Gimble::Input::InputSystem>::ms_Singleton = 0;

namespace Gimble
{
	namespace Input
	{
		InputSystem::InputSystem() : mInput(0), mKeyboard(0), mMouse(0), mX(0), mY(0), mPX(0), mPY(0), mDX(0), mDY(0), mMouseLocked(false)
		{
			
		}

		InputSystem::~InputSystem()
		{
			shutdown();
		}

		void InputSystem::init()
		{
			mRenderSys = Graphics::RenderSystem::getPtr();
			if (!mRenderSys)
			{
				Log::getPtr()->error("InputSystem initialisation failed - No rendersystem exists");
				return;
			}

			mHwnd = mRenderSys->getRenderWindow()->getHwnd();

			HRESULT hr = DirectInput8Create(mRenderSys->getRenderWindow()->getInst(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mInput, NULL);
			if (FAILED(hr))
			{
				Log::getPtr()->error("InputSystem initialisation failed - DirectInput8Create failed");
				return;
			}

			hr = mInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
			if (FAILED(hr))
			{
				mKeyboard = 0;
				Log::getPtr()->warning("InputSystem keyboard initialisation failed - CreateDevice failed");
			}
			else
			{
				hr = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
				if (FAILED(hr))
				{
					if (mKeyboard)
					{
						mKeyboard->Release();
						mKeyboard = 0;
					}
					Log::getPtr()->warning("InputSystem keyboard initialisation failed - SetDataFormat failed");
				}
				else
				{
					hr = mKeyboard->SetCooperativeLevel(mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); //TODO: may need to make this configurable
					if (FAILED(hr))
					{
						if (mKeyboard)
						{
							mKeyboard->Release();
							mKeyboard = 0;
						}
						Log::getPtr()->warning("InputSystem keyboard initialisation failed - SetCooperativeLevel failed");
					}
				}
			}

			hr = mInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
			if (FAILED(hr))
			{
				mMouse = 0;
				Log::getPtr()->warning("InputSystem mouse initialisation failed - CreateDevice failed");
			}
			else
			{
				hr = mMouse->SetDataFormat(&c_dfDIMouse);
				if (FAILED(hr))
				{
					if (mMouse)
					{
						mMouse->Release();
						mMouse = 0;
					}
					Log::getPtr()->warning("InputSystem mouse initialisation failed - SetDataFormat failed");
				}
				else
				{
					hr = mMouse->SetCooperativeLevel(mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); //TODO: may need to make this configurable
					if (FAILED(hr))
					{
						if (mMouse)
						{
							mMouse->Release();
							mMouse = 0;
						}
						Log::getPtr()->warning("InputSystem mouse initialisation failed - SetCooperativeLevel failed");
					}
				}
			}

			updateDeviceState();
		}

		void InputSystem::shutdown()
		{
			if (mKeyboard)
			{
				mKeyboard->Release();
				mKeyboard = 0;
			}

			if (mMouse)
			{
				mMouse->Release();
				mMouse = 0;
			}

			if (mInput)
			{
				mInput->Release();
				mInput = 0;
			}
		}

		void InputSystem::updateDeviceState()
		{
			if (!mInput) return;

			ZeroMemory(mKeys, sizeof(mKeys));
			mMouseState.lX = 0;
			mMouseState.lY = 0;
			mMouseState.lZ = 0;
			mMouseState.rgbButtons[0] = 0;
			mMouseState.rgbButtons[1] = 0;
			mMouseState.rgbButtons[2] = 0;
			mMouseState.rgbButtons[3] = 0;

			if (mKeyboard)
			{
				HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeys), mKeys);
				if (FAILED(hr))
				{
					hr = mKeyboard->Acquire();
					if (!FAILED(hr)) 
						mKeyboard->GetDeviceState(sizeof(mKeys), mKeys);
				}
			}

			if (mMouse)
			{
				HRESULT hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);
				if (FAILED(hr))
				{
					hr = mMouse->Acquire();
					if (!FAILED(hr)) 
						mMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);
				}
			}

			int hwidth = ((int)mRenderSys->getWidth() / 2);
			int hheight = ((int)mRenderSys->getHeight() / 2);

			POINT pt, wpt; 
			GetCursorPos(&pt); 
			wpt = pt;
			ScreenToClient(mHwnd, &wpt); 
			mPX = mMouseLocked ? hwidth : mX;
			mPY = mMouseLocked ? hheight : mY;
			mX = wpt.x; 
			mY = wpt.y;
			mDX = mX - mPX;
			mDY = mY - mPY;

			if (mMouseLocked)
			{
				long dx = mX - hwidth;
				long dy = mY - hheight;

				SetCursorPos(pt.x - dx, pt.y - dy);
			}
		}
	}
}