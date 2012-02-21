
#include "GimbleMutex.h"
#include "GimbleThread.h"
#include "GimbleResourceManager.h"

#include <vector>
#include <map>

using namespace Gimble;

template <class T> class ResLoader : public Thread
{
private:
	std::vector<string> mPaths;
	std::map<string, Resource*> mResources;
	uint mProgress;

public:
	ResLoader() : mProgress(0) { }

	void add(const string &name)
	{
		mPaths.push_back(name);
	}

	T *get(const string &name)
	{
		std::map<string, Resource>::iterator iter = mResources.find(name);
		if (iter == mResources.end())
			return 0;

		return (T*)iter->second;
	}

	float progress()
	{
		return (float)mProgress / (float)mPaths.size();
	}

	DWORD run()
	{
		for (int i = -1; ++i < mPaths.size();)
			mResources.insert(std::pair<string, Resource*>(mPaths[i], ResourceManager::getPtr()->get(mPaths[i], typeid(T))));

		return 1;
	}
};
