
#ifndef _GIMBLE_REQS_
#define _GIMBLE_REQS_

#if defined(GIMBLE_EXPORT)
	#define GIMBLE_API __declspec(dllexport)
#else
	#define GIMBLE_API __declspec(dllimport)
#endif

#define NOMINMAX

#include <math.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <conio.h>
#include <assert.h>
#include <typeinfo>

//typedefs
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef int GIMBLE_HANDLE;

namespace Gimble
{
	typedef std::string string;
}

//Forward declarations
namespace Gimble
{
	class Math;
	class Vector2;
	class Vector3;
	class Vector4;
	class Quaternion;
	class Matrix;
	class Transform;
	
	class AABB;

	class Log;
	class LogDevice;
	class Timer;
	class Config;

	class Thread;
	class Mutex;

	class MultiMap;

	class ResourceManager;
	class ResourceLocator;
	class ResourceLoader;
	class ResourceStream;
	class Resource;

	class RenderWindow;
	
	namespace Game
	{
		class Engine;
		class App;
	}

	namespace Script
	{
		class ScriptFunctionContext;
		class ScriptFunctionBase;
		class ScriptClassBase;
		class ScriptEngine;
		class LuaScriptEngine;
	}

	class RenderWindow;

	namespace Graphics
	{
		enum PixelFormat;
		enum PrimitiveTopology;

		class Color;

		class VertexElement;
		class InputLayout;
		class Buffer;
		class MultiBuffer;
		class VertexShader;
		class PixelShader;
		class GeometryShader;
		class HullShader;
		class DomainShader;
		class ComputeShader;
		class Texture;
		class TextureLoader;
		class RenderTarget;
		class MultiRenderTarget;
		class DepthStencil;
		class MultiDepthStencil;
		class DepthStencilState;
		class RasterizerState;
		class BlendState;
		class Sampler;
		class ShaderResourceGroup;
		class RenderSystem;

		class Mesh;
		class Model;
		class ModelLoader;

		class Material;
		class MaterialLoader;
	}

	namespace Physics
	{
		class CollisionWorld;
		class CollisionShape;
		class BoxShape;
		class CollisionObject;
		class Contact;
		class ContactManifold;
		class MPR;

		class RigidBody;
		class Simulator;
		class CollisionSolver;
		class Joint;

		class Broadphase;
		class SAP;
		class PairCache;
	}

	namespace Input
	{
		class InputSystem;
	}
}

#pragma warning(disable:4018) //Signed/unsigned mismatch
#pragma warning(disable:4251) //Dll-interface

#endif