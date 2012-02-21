
#ifndef _GIMBLE_MODELLOADER_
#define _GIMBLE_MODELLOADER_

#include "GimbleReqs.h"
#include "GimbleModel.h"
#include "GimbleResourceLoader.h"
#include "GimbleVector2.h"
#include "GimbleVector3.h"

namespace Gimble
{
	namespace Graphics
	{
		class ModelLoader : public ResourceLoader
		{
		private:
			InputLayout *createInputLayout(string line);
			Buffer *readVertices(std::stringstream *stream, InputLayout *layout, uint num);

		public:
			Model *load(ResourceStream &stream);

			inline const std::type_info &getType() { return typeid(Model); }
		};
	}
}

#endif