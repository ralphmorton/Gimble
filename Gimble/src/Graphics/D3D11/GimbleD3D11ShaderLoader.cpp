
#include "Graphics/D3D11/GimbleD3D11ShaderLoader.h"
#include "Graphics/D3D11/GimbleD3D11RenderSystem.h"
#include "GimbleResourceStream.h"
#include "GimbleResource.h"
#include "GimbleStringUtils.h"
#include "GimbleLog.h"

#include <algorithm>

namespace Gimble
{
	namespace Graphics
	{
		ID3D10Blob *D3D10ShaderLoader::compile(const string &resid, void *data, uint size, Gimble::string entry, Gimble::string profile)
		{
			ID3D10Blob *shader = 0;
			ID3D10Blob *error = 0;

			HRESULT hr = D3DX11CompileFromMemory((LPCSTR)data, size, NULL, NULL, NULL, entry.c_str(), profile.c_str(), 0, 0, NULL, &shader, &error, NULL);

			if (FAILED(hr))
			{
				char *msg = new char[error->GetBufferSize()+1];
				msg[error->GetBufferSize()] = '\0';
				memcpy(msg, error->GetBufferPointer(), error->GetBufferSize());
				string smsg(msg);
				
				Log::getPtr()->error("Error: Could not compile shader data from resource [" + resid + "] (" + smsg + ")");

				delete [] msg;

				SAFE_RELEASE(shader);
				SAFE_RELEASE(error);

				return false;
			}
			SAFE_RELEASE(error);

			return shader;
		}

		bool D3D10ShaderLoader::read(const string &resid, void *data, uint size, string type, uint &compilelength, string &entrypoint, string &profile)
		{
			compilelength = 0;

			std::stringstream in((char*)data, std::ios::in);
			string line;
			while (!in.eof())
			{
				std::getline(in, line);
				
				if (line.substr(0, 7) != "gimble:")
					compilelength = (uint)in.tellg();
				else
				{
					string s = line.substr(7, std::string::npos);
					std::vector<string> *parts = StringUtils::split(s, " ", true);
					if (parts->size() == 3)
					{
						string itype = (*parts)[0];
						string iprofile = (*parts)[1];
						string entry = (*parts)[2];
						entry.erase(std::remove(entry.begin(), entry.end(), '\r'), entry.end());
						delete parts;

						if (itype == type)
						{
							profile = iprofile;
							entrypoint = entry;
							return true;
						}
						else
						{
							Log::getPtr()->error("Error: Not a vertex shader: [" + line + "] in shader [" + resid + "]");
							return false;
						}
					}
					else
					{
						delete parts;
						Log::getPtr()->error("Error: Unknown shader compilation instruction [" + line + "] in shader [" + resid + "]");
						return false;
					}
				}
			}

			Log::getPtr()->error("Error: No shader compilation instruction found in shader [" + resid + "]");
			return false;

			//compilelength = size;
			//entrypoint = "VSMain";
			//profile = "vs_4_0";
			//return true;
		}

		//VERTEX SHADER
		Resource *D3D11VertexShaderLoader::load(ResourceStream &stream)
		{
			void *data = (void *)new char[stream.getSize()];
			uint size = stream.getSize();
			stream.read(data, 0, size);

			string resid = stream.getResourceID();

			uint compileLength;
			string entrypoint;
			string profile;

			bool success = read(resid, data, size, "Vertex", compileLength, entrypoint, profile);

			if (!success)
			{
				delete [] data;
				return 0;
			}

			ID3D10Blob *shaderBlob = 0;
			ID3D10Blob *errorBlob = 0;
			ID3D11VertexShader *shader = 0;
			
			ID3D11Device *device = static_cast<D3D11RenderSystem*>(RenderSystem::getPtr())->getDevice();

			shaderBlob = compile(resid, data, compileLength, entrypoint, profile);
			if (!shaderBlob)
			{
				Log::getPtr()->error("Error: Unable to compile Vertex Shader [" + entrypoint + "] in shader [" + resid + "]");
				SAFE_RELEASE(shaderBlob);
				delete [] data;
				return 0;
			}
			else
			{
				SAFE_RELEASE(errorBlob);
				HRESULT hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);

				if (FAILED(hr))
				{
					Log::getPtr()->error("Error: Unable to create Vertex Shader [" + entrypoint + "] in shader [" + resid + "]");
					SAFE_RELEASE(shaderBlob);
					SAFE_RELEASE(shader);
					delete [] data;
					return 0;
				}
			}

			delete [] data;

			return new D3D11VertexShader(shaderBlob, shader);
		}

		//PIXEL SHADER
		Resource *D3D11PixelShaderLoader::load(ResourceStream &stream)
		{
			void *data = (void *)new char[stream.getSize()];
			uint size = stream.getSize();
			stream.read(data, 0, size);

			string resid = stream.getResourceID();

			uint compileLength;
			string entrypoint;
			string profile;

			bool success = read(resid, data, size, "Pixel", compileLength, entrypoint, profile);

			if (!success)
			{
				delete [] data;
				return 0;
			}

			ID3D11PixelShader *shader = 0;
			ID3D10Blob *shaderBlob = 0;
			ID3D10Blob *errorBlob = 0;
			
			ID3D11Device *device = static_cast<D3D11RenderSystem*>(RenderSystem::getPtr())->getDevice();

			shaderBlob = compile(resid, data, compileLength, entrypoint, profile);
			if (!shaderBlob)
			{
				Log::getPtr()->error("Error: Unable to compile Pixel Shader [" + entrypoint + "] in shader [" + resid + "]");
				SAFE_RELEASE(shaderBlob);
				delete [] data;
				return 0;
			}
			else
			{
				SAFE_RELEASE(errorBlob);
				HRESULT hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
				SAFE_RELEASE(shaderBlob);

				if (FAILED(hr))
				{
					Log::getPtr()->error("Error: Unable to create Pixel Shader [" + entrypoint + "] in shader [" + resid + "]");
					SAFE_RELEASE(shaderBlob);
					SAFE_RELEASE(shader);
					delete [] data;
					return 0;
				}
			}

			delete [] data;

			return new D3D11PixelShader(shader);
		}

		//GEOMETRY SHADER
		Resource *D3D11GeometryShaderLoader::load(ResourceStream &stream)
		{
			void *data = (void *)new char[stream.getSize()];
			uint size = stream.getSize();
			stream.read(data, 0, size);

			string resid = stream.getResourceID();

			uint compileLength;
			string entrypoint;
			string profile;

			bool success = read(resid, data, size, "Geometry", compileLength, entrypoint, profile);

			if (!success)
			{
				delete [] data;
				return 0;
			}

			ID3D11GeometryShader *shader = 0;
			ID3D10Blob *shaderBlob = 0;
			ID3D10Blob *errorBlob = 0;
			
			ID3D11Device *device = static_cast<D3D11RenderSystem*>(RenderSystem::getPtr())->getDevice();

			shaderBlob = compile(resid, data, compileLength, entrypoint, profile);
			if (!shaderBlob)
			{
				Log::getPtr()->error("Error: Unable to compile Geometry Shader [" + entrypoint + "] in shader [" + resid + "]");
				SAFE_RELEASE(shaderBlob);
				delete [] data;
				return 0;
			}
			else
			{
				SAFE_RELEASE(errorBlob);
				HRESULT hr = device->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
				SAFE_RELEASE(shaderBlob);

				if (FAILED(hr))
				{
					Log::getPtr()->error("Error: Unable to create Geometry Shader [" + entrypoint + "] in shader [" + resid + "]");
					SAFE_RELEASE(shaderBlob);
					SAFE_RELEASE(shader);
					delete [] data;
					return 0;
				}
			}

			delete [] data;

			return new D3D11GeometryShader(shader);
		}

		//HULL SHADER
		Resource *D3D11HullShaderLoader::load(ResourceStream &stream)
		{
			void *data = (void *)new char[stream.getSize()];
			uint size = stream.getSize();
			stream.read(data, 0, size);

			string resid = stream.getResourceID();

			uint compileLength;
			string entrypoint;
			string profile;

			bool success = read(resid, data, size, "Hull", compileLength, entrypoint, profile);

			if (!success)
			{
				delete [] data;
				return 0;
			}

			ID3D11HullShader *shader = 0;
			ID3D10Blob *shaderBlob = 0;
			ID3D10Blob *errorBlob = 0;
			
			ID3D11Device *device = static_cast<D3D11RenderSystem*>(RenderSystem::getPtr())->getDevice();

			shaderBlob = compile(resid, data, compileLength, entrypoint, profile);
			if (!shaderBlob)
			{
				Log::getPtr()->error("Error: Unable to compile Hull Shader [" + entrypoint + "] in shader [" + resid + "]");
				SAFE_RELEASE(shaderBlob);
				delete [] data;
				return 0;
			}
			else
			{
				SAFE_RELEASE(errorBlob);
				HRESULT hr = device->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
				SAFE_RELEASE(shaderBlob);

				if (FAILED(hr))
				{
					Log::getPtr()->error("Error: Unable to create Hull Shader [" + entrypoint + "] in shader [" + resid + "]");
					SAFE_RELEASE(shaderBlob);
					SAFE_RELEASE(shader);
					delete [] data;
					return 0;
				}
			}

			delete [] data;

			return new D3D11HullShader(shader);
		}

		//DOMAIN SHADER
		Resource *D3D11DomainShaderLoader::load(ResourceStream &stream)
		{
			void *data = (void *)new char[stream.getSize()];
			uint size = stream.getSize();
			stream.read(data, 0, size);

			string resid = stream.getResourceID();

			uint compileLength;
			string entrypoint;
			string profile;

			bool success = read(resid, data, size, "Domain", compileLength, entrypoint, profile);

			if (!success)
			{
				delete [] data;
				return 0;
			}

			ID3D11DomainShader *shader = 0;
			ID3D10Blob *shaderBlob = 0;
			ID3D10Blob *errorBlob = 0;
			
			ID3D11Device *device = static_cast<D3D11RenderSystem*>(RenderSystem::getPtr())->getDevice();

			shaderBlob = compile(resid, data, compileLength, entrypoint, profile);
			if (!shaderBlob)
			{
				Log::getPtr()->error("Error: Unable to compile Domain Shader [" + entrypoint + "] in shader [" + resid + "]");
				SAFE_RELEASE(errorBlob);
				SAFE_RELEASE(shaderBlob);
				delete [] data;
				return 0;
			}
			else
			{
				SAFE_RELEASE(errorBlob);
				HRESULT hr = device->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader);
				SAFE_RELEASE(shaderBlob);

				if (FAILED(hr))
				{
					Log::getPtr()->error("Error: Unable to create Domain Shader [" + entrypoint + "] in shader [" + resid + "]");
					SAFE_RELEASE(shaderBlob);
					SAFE_RELEASE(shader);
					delete [] data;
					return 0;
				}
			}

			delete [] data;

			return new D3D11DomainShader(shader);
		}
	}
}