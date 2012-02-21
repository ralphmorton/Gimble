
#include "Game/GimbleEngine.h"
#include "Game/GimbleApp.h"
#include "GimbleSysUtils.h"
#include "GimbleTimer.h"
#include "GimbleConfig.h"
#include "GimbleLog.h"
#include "GimbleFileLogDevice.h"
#include "GimbleRenderWindow.h"
#include "Script/GimbleScriptEngine.h"
#include "Script/GimbleLuaScriptEngine.h"
#include "Graphics/GimbleRenderSystem.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "GimbleResourceManager.h"
#include "GimbleFileResourceLocator.h"
#include "Graphics/GimbleModelLoader.h"
#include "Graphics/GimbleMaterialLoader.h"
#include "Physics/GimbleSimulator.h"
#include "Physics/GimbleSAP.h"
#include "Input/GimbleInputSystem.h"

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

template<> Gimble::Game::Engine* Gimble::Singleton<Gimble::Game::Engine>::ms_Singleton = 0;

namespace Gimble
{
	namespace Game
	{
		Engine::Engine(Graphics::RenderSystem *renderSys, Script::ScriptEngine *scriptEngine) : mConfig(0), mApp(0), mRenderSystem(renderSys), mScriptEngine(scriptEngine), mInputSystem(0), mResourceManager(0), mRenderWindow(0), mShutdownRequested(false)
		{
			RenderWindow::setWindowCloseCallback(Engine::windowCloseCallback);

			if (!Log::getPtr()) new Log(); //Make sure theres a log around
			if (!FileLogDevice::getPtr())
			{
				FileLogDevice *flog = new FileLogDevice("gimble.log");
				Log::getPtr()->addDevice(flog);
			}
			Log::getPtr()->info("Log created");

			mResourceManager = initResourceManager();
			mSimulator = initSimulator();
		}

		Engine::~Engine()
		{
			delete mApp;
			mApp = 0;

			delete mInputSystem;
			mInputSystem = 0;

			delete mRenderSystem;
			mRenderSystem = 0;
			mRenderWindow = 0; //RenderSystem kills RenderWindow

			delete mSimulator;
			mSimulator = 0;

			delete mScriptEngine;

			delete mResourceManager;
			mResourceManager = 0;

			delete Log::getPtr();
		}

		Graphics::RenderSystem *Engine::createRenderSystem()
		{
			return new Graphics::D3D11RenderSystem();
		}

		RenderWindow *Engine::initRenderSystem()
		{
			int width = 640;
			int height = 480;
			bool fullscreen = false;
			string title = "Gimble";

			if (mConfig)
			{
				int cwidth = mConfig->getValueAsInt("RenderWidth");
				int cheight = mConfig->getValueAsInt("RenderHeight");
				bool cfullscreen = mConfig->getValueAsBool("RenderFullscreen");
				string ctitle = mConfig->getValue("WindowTitle");
				if (ctitle != "")
					title = ctitle;

				width = cwidth > width ? cwidth : width;
				height = cheight > height ? cheight : height;
				fullscreen = cfullscreen;
			}

			return mRenderSystem->init(title, width, height, fullscreen);
		}

		ResourceManager *Engine::initResourceManager()
		{
			if (ResourceManager::getPtr() != 0)
				delete ResourceManager::getPtr();

			ResourceManager *m = new ResourceManager();

			FileResourceLocator *frl = new FileResourceLocator();
			m->addLocator(frl);

			Graphics::ModelLoader *mldr = new Graphics::ModelLoader;
			m->addLoader(mldr);

			Graphics::MaterialLoader *mtldr = new Graphics::MaterialLoader;
			m->addLoader(mtldr);

			return m;
		}

		Physics::Simulator *Engine::initSimulator()
		{
			if (Physics::Simulator::getPtr())
				delete Physics::Simulator::getPtr();

			uint maxObjects = 1000;
			uint cmax;

			if (mConfig && (cmax = mConfig->getValueAsInt("PhysicsMaxObjects")))
				maxObjects = cmax;

			Physics::Simulator *sim = new Physics::Simulator(new Physics::SAP(maxObjects));
			return sim;
		}

		void Engine::cleanup()
		{
			mInputSystem->shutdown();
			mRenderSystem->shutdown();
			mSimulator->clear();
			mResourceManager->clear();
			mApp->shutdown();
		}

		void Engine::run(string cfgPath, App *app)
		{
			if (!app) 
			{
				Log::getPtr()->error("No App provided to run");
				return;
			}
			this->mApp = app;

			mConfig = Gimble::Config::load(cfgPath);

			if (mRenderSystem == 0)
				mRenderSystem = createRenderSystem();
			if (mScriptEngine == 0)
				mScriptEngine = new Script::LuaScriptEngine();
			mRenderWindow = initRenderSystem();

			if (Input::InputSystem::getPtr())
				delete Input::InputSystem::getPtr();
			mInputSystem = new Input::InputSystem();
			mInputSystem->init();

			if (!mApp->init())
			{
				Log::getPtr()->error("App init function failed");
				cleanup();
				return;
			}

			Timer timer;
			timer.reset();
			unsigned long micros = 0;

			Log::getPtr()->info("Engine main loop commencing");

			while (!mShutdownRequested)
			{
				MSG  msg;
				while(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				unsigned long nmicros = timer.getMicros();
				unsigned long delta = nmicros - micros;
				micros = nmicros;

				mInputSystem->updateDeviceState();

				bool reqshut = !mApp->update(delta / 1000000.0f);
				if (reqshut) mShutdownRequested = true;

				mApp->draw();
			}

			Log::getPtr()->info("Engine main loop ended");

			//Its over, shutdown time

			cleanup();

			if (mConfig) mConfig->save();
			delete mConfig;
			mConfig = 0;
		}

		bool Engine::shutdown()
		{
			Log::getPtr()->info("Engine shutdown requested, granted");

			mShutdownRequested = true;
			return true;
		}

		string Engine::getConfigValue(const string &key)
		{
			if (mConfig == 0) return 0;
			return mConfig->getValue(key);
		}

		bool Engine::setConfigValue(const string &key, const string &value)
		{
			if (mConfig == 0) return false;
			mConfig->setValue(key, value);
			return true;
		}

		bool Engine::windowCloseCallback()
		{
			return Engine::getPtr()->shutdown();
		}
	}
}
