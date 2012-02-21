
#include "GimbleResourceManager.h"
#include "GimbleResourceLocator.h"
#include "GimbleResourceLoader.h"
#include "GimbleResource.h"
#include "GimbleLog.h"

namespace Gimble
{
	template<> ResourceManager* Singleton<ResourceManager>::ms_Singleton = 0;

	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{
		for (unsigned int i = 0; i < mLocators.size(); i++)
			delete (mLocators[i]);

		for (unsigned int i = 0; i < mLoaders.size(); i++)
			delete (mLoaders[i]);

		for (ResourceMapIterator iter = mLocatedResources.begin(); iter != mLocatedResources.end(); iter++)
			delete (iter->second);

		mLocatedResources.clear();
	}

	bool ResourceManager::exists(const string &name)
	{
		if (mLocatedResources.find(name) != mLocatedResources.end())
			return true;

		for (unsigned int i = 0; i < mLocators.size(); i++)
			if (mLocators[i]->exists(name)) return true;

		return false;
	}

	bool ResourceManager::isType(const string &name, const std::type_info &type)
	{
		if (mLocatedResources.find(name) != mLocatedResources.end())
			return mLocatedResources.find(name)->second->getType() == type;

		return false;
	}

	Resource *ResourceManager::get(const string &name, const std::type_info &type)
	{
		ResourceLoader *loader = 0;
		for (unsigned int i = 0; i < mLoaders.size(); i++)
		{
			if (mLoaders[i]->getType() == type)
			{
				loader = mLoaders[i];
				break;
			}
		}

		if (!loader)
		{
			Log::getPtr()->warning("Could not load resource [" + name + "]. No ResourceLoader handling the specified type is known by the ResourceManager");
			return 0;
		}

		if (mLocatedResources.find(name) != mLocatedResources.end())
		{
			Resource *res = mLocatedResources.find(name)->second;

			assert(res->getType() == type);

			if (res->getType() != type)
			{
				Log::getPtr()->warning("Could not load resource [" + name + "]. Attempting to load as wrong type, type is actually [" + res->getTypeName() + "]");
				return 0;
			}

			return res;
		}

		for (unsigned int i = 0; i < mLocators.size(); i++)
		{
			if (mLocators[i]->exists(name))
			{
				ResourceStream *stream = mLocators[i]->locate(name);

				assert(stream && "Resource stream could not be loaded");
				if (!stream)
					return 0;

				Resource *res = loader->load(*stream);

				delete stream;

				//assert(res && "Resource could not be loaded");
				if (!res)
					return 0;

				mLocatedResources[name] = res;

				assert(res->getType() == type);

				if (res->getType() != type)
				{
					Log::getPtr()->warning("Could not load resource [" + name + "]. Attempting to load as wrong type, type is actually [" + res->getTypeName() + "]");
					return 0;
				}

				return res;
			}
		}

		Log::getPtr()->warning("Resource [" + name + "] could not be found");

		return 0;
	}

	bool ResourceManager::load(const string &name, const std::type_info &type)
	{
		return get(name, type) != 0;
	}

	void ResourceManager::unload(const string &name)
	{
		if (mLocatedResources.find(name) != mLocatedResources.end())
		{
			Resource *res = mLocatedResources.find(name)->second;
			delete res;

			mLocatedResources.erase(name);
		}
	}

	void ResourceManager::clear()
	{
		for (ResourceMapIterator iter = mLocatedResources.begin(); iter != mLocatedResources.end(); iter++)
			delete iter->second;

		mLocatedResources.clear();
	}

	void ResourceManager::addLoader(ResourceLoader *loader)
	{
		assert(loader);

		for (ResourceLoaderListIterator iter = mLoaders.begin(); iter != mLoaders.end(); iter++)
		{
			if ((*iter)->getType() == loader->getType())
			{
				Log::getPtr()->warning("ResourceLoader was not added, a loader for the specified type exists already");
				return;
			}
		}
		
		mLoaders.push_back(loader);
	}

	void ResourceManager::removeLoader(ResourceLoader *loader)
	{
		assert(loader);

		for (ResourceLoaderListIterator iter = mLoaders.begin(); iter != mLoaders.end(); iter++)
		{
			if ((*iter) == loader)
			{
				mLoaders.erase(iter);
				return;
			}
		}
	}

	void ResourceManager::addLocator(ResourceLocator *locator)
	{
		assert(locator);

		mLocators.push_back(locator);
	}

	void ResourceManager::insertLocator(ResourceLocator *locator, uint index)
	{
		assert(locator);

		if (index >= mLocators.size())
		{
			mLocators.push_back(locator);
			return;
		}

		ResourceLocatorListIterator iter = mLocators.begin();
		for (unsigned int i = 0; i < index; i++) iter++;

		mLocators.insert(iter, locator);
	}

	ResourceLocator *ResourceManager::getLocator(uint index)
	{
		assert(index < mLocators.size());

		return mLocators[index];
	}

	void ResourceManager::removeLocator(ResourceLocator *locator)
	{
		for (ResourceLocatorListIterator iter = mLocators.begin(); iter != mLocators.end(); iter++)
		{
			if ((*iter) == locator)
			{
				delete (*iter);
				mLocators.erase(iter);
				return;
			}
		}
	}
}