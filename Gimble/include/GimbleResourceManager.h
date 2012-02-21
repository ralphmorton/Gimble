
#ifndef _GIMBLE_RESOURCEMANAGER_
#define _GIMBLE_RESOURCEMANAGER_

#include "GimbleReqs.h"
#include "GimbleSingleton.h"

namespace Gimble
{
	class GIMBLE_API ResourceManager : public Singleton<ResourceManager>
	{
	private:
		typedef std::vector<ResourceLocator *> ResourceLocatorList;
		typedef std::vector<ResourceLocator *>::iterator ResourceLocatorListIterator;

		typedef std::vector<ResourceLoader *> ResourceLoaderList;
		typedef std::vector<ResourceLoader *>::iterator ResourceLoaderListIterator;

		typedef std::map<string, Resource *> ResourceMap;
		typedef std::map<string, Resource *>::iterator ResourceMapIterator;

	private:
		ResourceLocatorList mLocators;
		ResourceLoaderList mLoaders;
		ResourceMap mLocatedResources;

	public:
		ResourceManager();
		~ResourceManager();

		bool exists(const string &name);
		bool isType(const string &name, const std::type_info &type);

		Resource *get(const string &name, const std::type_info &type);
		template <class T> T *get(const string &name)
		{
			return (T*)get(name, typeid(T));
		}

		bool load(const string &name, const std::type_info &type);
		void unload(const string &name);
		void clear();

		void addLoader(ResourceLoader *loader);
		void removeLoader(ResourceLoader *loader);

		void addLocator(ResourceLocator *locator);
		void insertLocator(ResourceLocator *locator, uint index);
		ResourceLocator *getLocator(uint index);
		void removeLocator(ResourceLocator *locator);

		inline uint getNumLocators() { return mLocators.size(); }
	};
}

#endif