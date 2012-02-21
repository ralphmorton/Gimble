
#ifndef _GIMBLE_COMPUTESHADER_
#define _GIMBLE_COMPUTESHADER_

#include "GimbleReqs.h"
#include "GimbleResource.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API ComputeShader : public Resource
		{
		private:
			static string mTypeName;

		public:
			virtual void run(const MultiBuffer &shaderResourceViews, const MultiBuffer &unorderedAccessViews, const MultiBuffer &constantBuffers, uint x, uint y, uint z) = 0;

			inline const std::type_info &getType() { return typeid(ComputeShader); }
			inline const string &getTypeName() { return mTypeName; }
		};
	}
}

#endif