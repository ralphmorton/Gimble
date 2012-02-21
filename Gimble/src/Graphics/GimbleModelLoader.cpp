
#include "Graphics/GimbleModelLoader.h"
#include "Graphics/GimbleMesh.h"
#include "Graphics/GimbleBuffer.h"
#include "Graphics/GimbleInputLayout.h"
#include "Graphics/GimbleRenderSystem.h"
#include "GimbleResourceStream.h"
#include "GimbleStringUtils.h"
#include "GimbleLog.h"

namespace Gimble
{
	namespace Graphics
	{
		InputLayout *ModelLoader::createInputLayout(string line)
		{
			InputLayout *ilayout = RenderSystem::getPtr()->createInputLayout();

			ushort slot = 0;
			ushort offset = 0;

			ushort npos = 0;
			ushort nnorm = 0;
			ushort ntan = 0;
			ushort nuv = 0;
			ushort ncol = 0;

			std::vector<string> *parts = StringUtils::split(line, " ", true);
			for (int i = -1; ++i < parts->size();)
			{
				string semantic = (*parts)[i];

				if (semantic == "pos")
				{
					ilayout->add(Graphics::VERTEXELEMENT_FLOAT3, Graphics::VERTEXELEMENTSEMANTIC_POSITION, 0, npos++, offset);
					offset += sizeof(float) * 3;
				}
				else if (semantic == "norm")
				{
					ilayout->add(Graphics::VERTEXELEMENT_FLOAT3, Graphics::VERTEXELEMENTSEMANTIC_NORMAL, 0, nnorm++, offset);
					offset += sizeof(float) * 3;
				}
				else if (semantic == "tan")
				{
					ilayout->add(Graphics::VERTEXELEMENT_FLOAT4, Graphics::VERTEXELEMENTSEMANTIC_TANGENT, 0, ntan++, offset);
					offset += sizeof(float) * 4;
				}
				else if (semantic == "uv")
				{
					ilayout->add(Graphics::VERTEXELEMENT_FLOAT2, Graphics::VERTEXELEMENTSEMANTIC_TEXTURECOORDS, 0, nuv++, offset);
					offset += sizeof(float) * 2;
				}
				else if (semantic == "col")
				{
					ilayout->add(Graphics::VERTEXELEMENT_FLOAT3, Graphics::VERTEXELEMENTSEMANTIC_DIFFUSE, 0, nuv++, offset);
					offset += sizeof(float) * 3;
				}
			}

			delete parts;

			return ilayout;
		}

		Buffer *ModelLoader::readVertices(std::stringstream *stream, InputLayout *layout, uint num)
		{
			uint lbytes = 0;
			for (int i = -1; ++i < layout->getElementCount();)
			{
				VertexElement el = layout->getElement(i);
				switch (el.getType())
				{
				case VERTEXELEMENT_FLOAT1:
					lbytes += sizeof(float);
					break;
				case VERTEXELEMENT_FLOAT2:
					lbytes += sizeof(float) * 2;
					break;
				case VERTEXELEMENT_FLOAT3:
					lbytes += sizeof(float) * 3;
					break;
				case VERTEXELEMENT_FLOAT4:
				case VERTEXELEMENT_COLOR:
					lbytes += sizeof(float) * 4;
					break;
				}
			}

			uint len = num * lbytes;
			uint offset = 0;

			float *data = new float[len];

			string line;
			for (int i = -1; ++i < num;)
			{
				if (stream->eof())
				{
					delete [] data;
					return 0;
				}

				std::getline(*stream, line);

				std::vector<string> *parts = StringUtils::split(line, " ", true);
				for (int j = -1; ++j < parts->size();)
					data[offset++] = StringUtils::toFloat((*parts)[j]);

				delete parts;
			}

			Buffer *vbuf = RenderSystem::getPtr()->createBuffer(Buffer::BUFFERTYPE_VERTEX, Buffer::BUFFERUSAGE_STATIC, len, lbytes, data);

			delete [] data;

			return vbuf;
		}

		Model *ModelLoader::load(ResourceStream &stream)
		{
			int len = stream.getSize();
			char *data = new char[len];
			stream.read(data, 0, len);

			std::stringstream in(data, std::ios::in);
			string line;
			
			if (in.eof())
			{
				delete [] data;
				return 0;
			}
			std::getline(in, line);

			InputLayout *layout = createInputLayout(line);

			if (in.eof())
			{
				delete layout;
				delete [] data;
				return 0;
			}
			std::getline(in, line);

			int nverts = StringUtils::toInt(line);
			Buffer *vbuf = readVertices(&in, layout, nverts);

			if (in.eof())
			{
				delete layout;
				delete vbuf;
				delete [] data;
				return 0;
			}

			std::vector<Mesh *> meshes;

			std::vector<int> *inds = 0;
			string meshname = "";

			while (!in.eof())
			{
				std::getline(in, line);
				if (line[0] == '-') break; //End

				std::vector<string> *parts = StringUtils::split(line, " ", true);
				if (parts->size() < 2)
				{
					delete parts;
					delete layout;
					delete vbuf;
					for (int i = -1; ++i < meshes.size(); delete meshes[i]);;
					delete [] data;
					return 0;
				}

				if ((*parts)[0] == "mat")
				{
					if (inds)
					{
						int *idata = new int[inds->size()];
						for (int i = -1; ++i < inds->size(); idata[i] = (*inds)[i]);;

						Buffer *ibuf = RenderSystem::getPtr()->createBuffer(Buffer::BUFFERTYPE_INDEX, Buffer::BUFFERUSAGE_STATIC, sizeof(int) * inds->size(), sizeof(int), idata);
						meshes.push_back(new Mesh(meshname, ibuf));

						delete [] idata;
						delete inds;
						meshname = "";
					}

					meshname = (*parts)[1];
					inds = new std::vector<int>();
				}
				else
				{
					int i1 = StringUtils::toInt((*parts)[0]);
					int i2 = StringUtils::toInt((*parts)[1]);
					int i3 = StringUtils::toInt((*parts)[2]);

					inds->push_back(i1);
					inds->push_back(i2);
					inds->push_back(i3);
				}

				delete parts;
			}

			if (inds)
			{
				int *idata = new int[inds->size()];
				for (int i = -1; ++i < inds->size(); idata[i] = (*inds)[i]);;

				Buffer *ibuf = RenderSystem::getPtr()->createBuffer(Buffer::BUFFERTYPE_INDEX, Buffer::BUFFERUSAGE_STATIC, sizeof(int) * inds->size(), sizeof(int), idata);
				meshes.push_back(new Mesh(meshname, ibuf));

				delete [] idata;
				delete inds;
				meshname = "";
			}

			delete [] data;

			return new Model(layout, vbuf, meshes);
		}
	}
}