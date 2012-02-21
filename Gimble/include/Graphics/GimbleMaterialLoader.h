
#ifndef _GIMBLE_MATERIALLOADER_
#define _GIMBLE_MATERIALLOADER_

#include "GimbleReqs.h"
#include "GimbleMaterial.h"
#include "GimbleResourceLoader.h"

namespace Gimble
{
	namespace Graphics
	{
		class MaterialLoader : public ResourceLoader
		{
		private:
			void read(string resid, string type, string name, string val, Material *mat);

		public:
			Material *load(ResourceStream &stream);

			inline const std::type_info &getType() { return typeid(Material); }
		};
	}
}

#endif