
#ifndef _GIMBLE_SCRIPTCLASS_
#define _GIMBLE_SCRIPTCLASS_

#include "GimbleReqs.h"
#include "Script/GimbleScriptFunctionContext.h"

namespace Gimble
{
	namespace Script
	{
		template <class T, class R> class ScriptClassFunctionDispatcher
		{
		private:
			ScriptFunctionContext *mContext;

		public:
			ScriptClassFunctionDispatcher(ScriptFunctionContext *context) : mContext(context) { }
			~ScriptClassFunctionDispatcher() { }

			void dispatch(R (T::*func)(), T *t)
			{
				mContext->returnValue<R>((t->*func)());
			}

			template <class P1> void dispatch(P1 *p1, R (T::*func)(P1), T *t)
			{
				mContext->returnValue<R>((t->*func)(*p1));
			}

			template <class P1, class P2> void dispatch(P1 *p1, P2 *p2, R (T::*func)(P1,P2), T *t)
			{
				mContext->returnValue<R>((t->*func)(*p1,*p2));
			}

			template <class P1, class P2, class P3> void dispatch(P1 *p1, P2 *p2, P3 *p3, R (T::*func)(P1,P2,P3), T *t)
			{
				mContext->returnValue<R>((t->*func)(*p1,*p2,*p3));
			}

			template <class P1, class P2, class P3, class P4> void dispatch(P1 *p1, P2 *p2, P3 *p3, P4 *p4, R (T::*func)(P1,P2,P3,P4), T *t)
			{
				mContext->returnValue<R>((t->*func)(*p1,*p2,*p3,*p4));
			}

			template <class P1, class P2, class P3, class P4, class P5> void dispatch(P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5, R (T::*func)(P1,P2,P3,P4,P5), T *t)
			{
				mContext->returnValue<R>((t->*func)(*p1,*p2,*p3,*p4,*p5));
			}

			template <class P1, class P2, class P3, class P4, class P5, class P6> void dispatch(P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5, P6 *p6, R (T::*func)(P1,P2,P3,P4,P5,P6), T *t)
			{
				mContext->returnValue<R>((t->*func)(*p1,*p2,*p3,*p4,*p5,*p6));
			}
		};

		template <class T> class ScriptClassFunctionDispatcher<T, void>
		{
		private:
			ScriptFunctionContext *mContext;

		public:
			ScriptClassFunctionDispatcher(ScriptFunctionContext *context) : mContext(context) { }
			~ScriptClassFunctionDispatcher() { }

			void dispatch(void (T::*func)(), T *t)
			{
				(t->*func)();
			}

			template <class P1> void dispatch(P1 *p1, void (T::*func)(P1), T *t)
			{
				(t->*func)(*p1);
			}

			template <class P1, class P2> void dispatch(P1 *p1, P2 *p2, void (T::*func)(P1,P2), T *t)
			{
				(t->*func)(*p1,*p2);
			}

			template <class P1, class P2, class P3> void dispatch(P1 *p1, P2 *p2, P3 *p3, void (T::*func)(P1,P2,P3), T *t)
			{
				(t->*func)(*p1,*p2,*p3);
			}

			template <class P1, class P2, class P3, class P4> void dispatch(P1 *p1, P2 *p2, P3 *p3, P4 *p4, void (T::*func)(P1,P2,P3,P4), T *t)
			{
				(t->*func)(*p1,*p2,*p3,*p4);
			}

			template <class P1, class P2, class P3, class P4, class P5> void dispatch(P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5, void (T::*func)(P1,P2,P3,P4,P5), T *t)
			{
				(t->*func)(*p1,*p2,*p3,*p4,*p5);
			}

			template <class P1, class P2, class P3, class P4, class P5, class P6> void dispatch(P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5, P6 *p6, void (T::*func)(P1,P2,P3,P4,P5,P6), T *t)
			{
				(t->*func)(*p1,*p2,*p3,*p4,*p5,*p6);
			}
		};

		class GIMBLE_API ScriptClassBase
		{
		public:
			typedef void (*sfunc)(ScriptFunctionContext*);

		public:
			struct Func
			{
				string mName;
				sfunc mFunc;
			};

		private:
			string mName;

		public:
			ScriptClassBase(const string &name) : mName(name) { }
			virtual ~ScriptClassBase() { }

			inline const string &getName() const { return mName; }

			virtual const type_info &getType() const = 0;
			virtual void *create() const = 0;
			virtual void del(ScriptFunctionContext*) const = 0;
			virtual uint numFuncs() const = 0;
			virtual Func getFunc(uint) const = 0;
		};

		template <class T> class ScriptClass : public ScriptClassBase
		{
		private:
			typedef void (*sfunc)(ScriptFunctionContext*);

		private:
			std::vector<Func> mFuncs;

		public:
			ScriptClass(const string &name) : ScriptClassBase(name) { }

			template <class R, R (T::*func)()> void addFunction(const string &name)
			{
				Func f = {name, &ScriptClass<T>::exec0<R, func>};
				mFuncs.push_back(f);
			}

			template <class R, class P1, R (T::*func)(P1)> void addFunction(const string &name)
			{
				Func f= {name, &ScriptClass<T>::exec1<R, P1, func>};
				mFuncs.push_back(f);
			}

			template <class R, class P1, class P2, R (T::*func)(P1,P2)> void addFunction(const string &name)
			{
				Func f= {name, &ScriptClass<T>::exec2<R, P1, P2, func>};
				mFuncs.push_back(f);
			}

			template <class R, class P1, class P2, class P3, R (T::*func)(P1,P2,P3)> void addFunction(const string &name)
			{
				Func f= {name, &ScriptClass<T>::exec3<R, P1, P2, P3, func>};
				mFuncs.push_back(f);
			}

			template <class R, class P1, class P2, class P3, class P4, R (T::*func)(P1,P2,P3,P4)> void addFunction(const string &name)
			{
				Func f= {name, &ScriptClass<T>::exec4<R, P1, P2, P3, P4, func>};
				mFuncs.push_back(f);
			}

			template <class R, class P1, class P2, class P3, class P4, class P5, R (T::*func)(P1,P2,P3,P4,P5)> void addFunction(const string &name)
			{
				Func f= {name, &ScriptClass<T>::exec5<R, P1, P2, P3, P4, P5, func>};
				mFuncs.push_back(f);
			}

			template <class R, class P1, class P2, class P3, class P4, class P5, class P6, R (T::*func)(P1,P2,P3,P4,P5,P6)> void addFunction(const string &name)
			{
				Func f= {name, &ScriptClass<T>::exec6<R, P1, P2, P3, P4, P5, P6, func>};
				mFuncs.push_back(f);
			}

		public:
			const type_info &getType() const
			{
				return typeid(T*);
			}

			uint numFuncs() const
			{
				return mFuncs.size();
			}

			Func getFunc(uint i) const
			{
				assert(i<numFuncs() && "Function index must be less than the total number of functions.");

				return mFuncs[i];
			}

			void *create() const
			{
				return (void*)new T();
			}

			void del(ScriptFunctionContext *context) const
			{
				T *t = context->getObjectContext<T>();
				if (t) delete t;
			}

		private:
			template <class R, R (T::*func)()> static void exec0(ScriptFunctionContext *context)
			{
				T *t = context->getObjectContext<T>();
				if (!t)
				{
					context->raiseError("Invalid object context passed to script class function");
					return;
				}

				ScriptClassFunctionDispatcher<T, R>(context).dispatch(func, t);
			}

			template <class R, class P1, R (T::*func)(P1)> static void exec1(ScriptFunctionContext *context)
			{
				T *t = context->getObjectContext<T>();
				if (!t)
				{
					context->raiseError("Invalid object context passed to script class function");
					return;
				}

				P1 *p1 = context->getParm<P1>(0);
				if (!p1)
				{
					context->raiseError("Invalid argument passed to script class function");
					return;
				}

				ScriptClassFunctionDispatcher<T, R>(context).dispatch(p1, func, t);
			}

			template <class R, class P1, class P2, R (T::*func)(P1,P2)> static void exec2(ScriptFunctionContext *context)
			{
				T *t = context->getObjectContext<T>();
				if (!t)
				{
					context->raiseError("Invalid object context passed to script class function");
					return;
				}

				P1 *p1 = context->getParm<P1>(0);
				P2 *p2 = context->getParm<P2>(1);
				if (!p1 || !p2)
				{
					context->raiseError("Invalid argument passed to script class function");
					return;
				}

				ScriptClassFunctionDispatcher<T, R>(context).dispatch(p1, p2, func, t);
			}

			template <class R, class P1, class P2, class P3, R (T::*func)(P1,P2,P3)> static void exec3(ScriptFunctionContext *context)
			{
				T *t = context->getObjectContext<T>();
				if (!t)
				{
					context->raiseError("Invalid object context passed to script class function");
					return;
				}

				P1 *p1 = context->getParm<P1>(0);
				P2 *p2 = context->getParm<P2>(1);
				P3 *p3 = context->getParm<P3>(2);
				if (!p1 || !p2 || !p3)
				{
					context->raiseError("Invalid argument passed to script class function");
					return;
				}

				ScriptClassFunctionDispatcher<T, R>(context).dispatch(p1, p2, p3, func, t);
			}

			template <class R, class P1, class P2, class P3, class P4, R (T::*func)(P1,P2,P3,P4)> static void exec4(ScriptFunctionContext *context)
			{
				T *t = context->getObjectContext<T>();
				if (!t)
				{
					context->raiseError("Invalid object context passed to script class function");
					return;
				}

				P1 *p1 = context->getParm<P1>(0);
				P2 *p2 = context->getParm<P2>(1);
				P3 *p3 = context->getParm<P3>(2);
				P4 *p4 = context->getParm<P4>(3);
				if (!p1 || !p2 || !p3 || !p4)
				{
					context->raiseError("Invalid argument passed to script class function");
					return;
				}

				ScriptClassFunctionDispatcher<T, R>(context).dispatch(p1, p2, p3, p4, func, t);
			}

			template <class R, class P1, class P2, class P3, class P4, class P5, R (T::*func)(P1,P2,P3,P4,P5)> static void exec5(ScriptFunctionContext *context)
			{
				T *t = context->getObjectContext<T>();
				if (!t)
				{
					context->raiseError("Invalid object context passed to script class function");
					return;
				}

				P1 *p1 = context->getParm<P1>(0);
				P2 *p2 = context->getParm<P2>(1);
				P3 *p3 = context->getParm<P3>(2);
				P4 *p4 = context->getParm<P4>(3);
				P5 *p5 = context->getParm<P5>(4);
				if (!p1 || !p2 || !p3 || !p4 || !p5)
				{
					context->raiseError("Invalid argument passed to script class function");
					return;
				}

				ScriptClassFunctionDispatcher<T, R>(context).dispatch(p1, p2, p3, p4, p5, func, t);
			}

			template <class R, class P1, class P2, class P3, class P4, class P5, class P6, R (T::*func)(P1,P2,P3,P4,P5,P6)> static void exec6(ScriptFunctionContext *context)
			{
				T *t = context->getObjectContext<T>();
				if (!t)
				{
					context->raiseError("Invalid object context passed to script class function");
					return;
				}

				P1 *p1 = context->getParm<P1>(0);
				P2 *p2 = context->getParm<P2>(1);
				P3 *p3 = context->getParm<P3>(2);
				P4 *p4 = context->getParm<P4>(3);
				P5 *p5 = context->getParm<P5>(4);
				P6 *p6 = context->getParm<P6>(5);
				if (!p1 || !p2 || !p3 || !p4 || !p5 || !p6)
				{
					context->raiseError("Invalid argument passed to script class function");
					return;
				}

				ScriptClassFunctionDispatcher<T, R>(context).dispatch(p1, p2, p3, p4, p5, p6, func, t);
			}
		};
	}
}

#endif