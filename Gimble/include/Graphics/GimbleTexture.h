
#ifndef _GIMBLE_TEXTURE_
#define _GIMBLE_TEXTURE_

#include "GimbleReqs.h"
#include "GimbleResource.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API MappedTexture
		{
		public:
			void *data;
			unsigned int rowPitch;

		public:
			MappedTexture(void *fdata, unsigned int frowPitch) : data(fdata), rowPitch(frowPitch) {}
		};

		class GIMBLE_API Texture : public Resource
		{
		public:
			enum TextureAccess
			{
				TEXTUREACCESS_READ,
				TEXTUREACCESS_WRITE,
				TEXTUREACCESS_READWRITE
			};

		private:
			static string mTypeName;

		protected:
			Texture() { }

		public:
			virtual ~Texture() { }

			virtual bool isMappable() const = 0;
			virtual MappedTexture map() = 0;
			virtual void unmap() = 0;

			virtual uint getWidth() const = 0;
			virtual uint getHeight() const = 0;
			virtual uint getDepth() const = 0;

			inline const std::type_info &getType() { return typeid(Texture); }
			inline const string &getTypeName() { return mTypeName; }
		};
	}
}

#endif