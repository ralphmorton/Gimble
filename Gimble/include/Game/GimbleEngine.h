
#ifndef _GIMBLE_ENGINE_
#define _GIMBLE_ENGINE_

#include "GimbleReqs.h"
#include "GimbleSingleton.h"

namespace Gimble
{
	namespace Game
	{
		class GIMBLE_API Engine : public Singleton<Engine>
		{
		private:
			Config *mConfig;
			Graphics::RenderSystem *mRenderSystem;
			Input::InputSystem *mInputSystem;
			Physics::Simulator *mSimulator;
			Script::ScriptEngine *mScriptEngine;
			ResourceManager *mResourceManager;
			RenderWindow *mRenderWindow;

			App *mApp;

			bool mShutdownRequested;

		public:
			Engine(Graphics::RenderSystem *renderSys = 0, Script::ScriptEngine *scriptEngine = 0);
			~Engine();

			void run(string cfgPath, App *app);
			bool shutdown();

			string getConfigValue(const string &key);
			bool setConfigValue(const string &key, const string &value);
		
			inline Graphics::RenderSystem *getRenderSystem() { return mRenderSystem; }
			inline Input::InputSystem *getInputSystem() { return mInputSystem; }
			inline Physics::Simulator *getSimulator() { return mSimulator; }
			inline Script::ScriptEngine *getScriptEngine() { return mScriptEngine; }
			inline App *getApp() { return mApp; }

		private:
			ResourceManager *initResourceManager();
			RenderWindow *initRenderSystem();
			Physics::Simulator *initSimulator();
			Input::InputSystem *initInputSystem();

			Graphics::RenderSystem *createRenderSystem();

			void cleanup();

		private:
			static bool windowCloseCallback();
		};
	}
}

#endif