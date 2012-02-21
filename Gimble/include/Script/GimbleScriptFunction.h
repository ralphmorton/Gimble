
#ifndef _GIMBLE_SCRIPTFUNCTION_
#define _GIMBLE_SCRIPTFUNCTION_

#include "GimbleReqs.h"
#include "Script/GimbleScriptFunctionContext.h"

namespace Gimble
{
	namespace Script
	{
		class GIMBLE_API ScriptFunctionBase
		{
		private:
			friend class ScriptEngine;

		private:
			uint mNumArgs;
			string mName;

		private:
			virtual void run(ScriptFunctionContext *ctx) const = 0;

		public:
			ScriptFunctionBase(const string &name, uint numArgs) : mName(name), mNumArgs(numArgs) { }
			virtual ~ScriptFunctionBase() { }

			inline const string &getName() const { return mName; }
			inline uint getNumArgs() const { return mNumArgs; }
		};

		template <class T> class ScriptFunctionDispatcher
		{
		private:
			ScriptFunctionContext *mContext;

		public:
			ScriptFunctionDispatcher(ScriptFunctionContext *context) : mContext(context) { }
			~ScriptFunctionDispatcher() { }

			void dispatch(T (*func)())
			{
				mContext->returnValue<T>(func());
			}

			template <class P1> void dispatch(T (*func)(P1), P1 *p1)
			{
				mContext->returnValue<T>(func(*p1));
			}

			template <class P1, class P2> void dispatch(T (*func)(P1,P2), P1 *p1, P2 *p2)
			{
				mContext->returnValue<T>(func(*p1,*p2));
			}

			template <class P1, class P2, class P3> void dispatch(T (*func)(P1,P2,P3), P1 *p1, P2 *p2, P3 *p3)
			{
				mContext->returnValue<T>(func(*p1,*p2,*p3));
			}

			template <class P1, class P2, class P3, class P4> void dispatch(T (*func)(P1,P2,P3,P4), P1 *p1, P2 *p2, P3 *p3, P4 *p4)
			{
				mContext->returnValue<T>(func(*p1,*p2,*p3,*p4));
			}

			template <class P1, class P2, class P3, class P4, class P5> void dispatch(T (*func)(P1,P2,P3,P4,P5), P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5)
			{
				mContext->returnValue<T>(func(*p1,*p2,*p3,*p4,*p5));
			}

			template <class P1, class P2, class P3, class P4, class P5, class P6> void dispatch(T (*func)(P1,P2,P3,P4,P5,P6), P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5, P6 *p6)
			{
				mContext->returnValue<T>(func(*p1,*p2,*p3,*p4,*p5,*p6));
			}

			template <class P1, class P2, class P3, class P4, class P5, class P6, class P7> void dispatch(T (*func)(P1,P2,P3,P4,P5,P6,P7), P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5, P6 *p6, P7 *p7)
			{
				mContext->returnValue<T>(func(*p1,*p2,*p3,*p4,*p5,*p6,*p7));
			}

			template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8> void dispatch(T (*func)(P1,P2,P3,P4,P5,P6,P7,P8), P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5, P6 *p6, P7 *p7, P8 *p8)
			{
				mContext->returnValue<T>(func(*p1,*p2,*p3,*p4,*p5,*p6,*p7,*p8));
			}
		};

		template <> class ScriptFunctionDispatcher<void>
		{
		private:
			ScriptFunctionContext *mContext;

		public:
			ScriptFunctionDispatcher(ScriptFunctionContext *context) : mContext(context) { }
			~ScriptFunctionDispatcher() { }

			void dispatch(void (*func)()) { func(); }

			template <class P1> void dispatch(void (*func)(P1), P1 *p1)
			{
				func(*p1);
			}

			template <class P1, class P2> void dispatch(void (*func)(P1,P2), P1 *p1, P2 *p2)
			{
				func(*p1,*p2);
			}

			template <class P1, class P2, class P3> void dispatch(void (*func)(P1,P2,P3), P1 *p1, P2 *p2, P3 *p3)
			{
				func(*p1,*p2,*p3);
			}

			template <class P1, class P2, class P3, class P4> void dispatch(void (*func)(P1,P2,P3,P4), P1 *p1, P2 *p2, P3 *p3, P4 *p4)
			{
				func(*p1,*p2,*p3,*p4);
			}

			template <class P1, class P2, class P3, class P4, class P5> void dispatch(void (*func)(P1,P2,P3,P4,P5), P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5)
			{
				func(*p1,*p2,*p3,*p4,*p5);
			}

			template <class P1, class P2, class P3, class P4, class P5, class P6> void dispatch(void (*func)(P1,P2,P3,P4,P5,P6), P1 *p1, P2 *p2, P3 *p3, P4 *p4, P5 *p5, P6 *p6)
			{
				func(*p1,*p2,*p3,*p4,*p5,*p6);
			}
		};

		template <class R> class ScriptFunction0 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)();

		public:
			ScriptFunction0(const string &name, R (*func)()) : ScriptFunctionBase(name, 0), mFunc(func) { }
			~ScriptFunction0() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				ScriptFunctionDispatcher<R>(ctx).dispatch(mFunc);
			}
		};

		template <class R, class P1> class ScriptFunction1 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)(P1);

		public:
			ScriptFunction1(const string &name, R (*func)(P1)) : ScriptFunctionBase(name, 1), mFunc(func) { }
			~ScriptFunction1() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				P1 *p = ctx->getParm<P1>(0);
				if (!p)
				{
					ctx->raiseError("Invalid argument passed to script function " + getName() + ".");
					return;
				}

				ScriptFunctionDispatcher<R>(ctx).dispatch(mFunc, p);
			}
		};

		template <class R, class P1, class P2> class ScriptFunction2 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)(P1,P2);

		public:
			ScriptFunction2(const string &name, R (*func)(P1, P2)) : ScriptFunctionBase(name, 2), mFunc(func) { }
			~ScriptFunction2() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				P1 *p1 = ctx->getParm<P1>(0);
				P2 *p2 = ctx->getParm<P2>(1);
				if (!p1 || !p2)
				{
					ctx->raiseError("Invalid argument passed to script function " + getName() + ".");
					return;
				}

				ScriptFunctionDispatcher<R>(ctx).dispatch<P1, P2>(mFunc, p1, p2);
			}
		};

		template <class R, class P1, class P2, class P3> class ScriptFunction3 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)(P1,P2,P3);

		public:
			ScriptFunction3(const string &name, R (*func)(P1, P2, P3)) : ScriptFunctionBase(name, 3), mFunc(func) { }
			~ScriptFunction3() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				P1 *p1 = ctx->getParm<P1>(0);
				P2 *p2 = ctx->getParm<P2>(1);
				P3 *p3 = ctx->getParm<P3>(2);
				if (!p1 || !p2 || !p3)
				{
					ctx->raiseError("Invalid argument passed to script function " + getName() + ".");
					return;
				}

				ScriptFunctionDispatcher<R>(ctx).dispatch<P1,P2,P3>(mFunc, p1, p2, p3);
			}
		};

		template <class R, class P1, class P2, class P3, class P4> class ScriptFunction4 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)(P1,P2,P3,P4);

		public:
			ScriptFunction4(const string &name, R (*func)(P1, P2, P3, P4)) : ScriptFunctionBase(name, 4), mFunc(func) { }
			~ScriptFunction4() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				P1 *p1 = ctx->getParm<P1>(0);
				P2 *p2 = ctx->getParm<P2>(1);
				P3 *p3 = ctx->getParm<P3>(2);
				P4 *p4 = ctx->getParm<P4>(3);
				if (!p1 || !p2 || !p3 || !p4)
				{
					ctx->raiseError("Invalid argument passed to script function " + getName() + ".");
					return;
				}

				ScriptFunctionDispatcher<R>(ctx).dispatch<P1,P2,P3,P4>(mFunc, p1, p2, p3, p4);
			}
		};

		template <class R, class P1, class P2, class P3, class P4, class P5> class ScriptFunction5 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)(P1,P2,P3,P4,P5);

		public:
			ScriptFunction5(const string &name, R (*func)(P1, P2, P3, P4, P5)) : ScriptFunctionBase(name, 5), mFunc(func) { }
			~ScriptFunction5() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				P1 *p1 = ctx->getParm<P1>(0);
				P2 *p2 = ctx->getParm<P2>(1);
				P3 *p3 = ctx->getParm<P3>(2);
				P4 *p4 = ctx->getParm<P4>(3);
				P5 *p5 = ctx->getParm<P5>(4);
				if (!p1 || !p2 || !p3 || !p4 || !p5)
				{
					ctx->raiseError("Invalid argument passed to script function " + getName() + ".");
					return;
				}

				ScriptFunctionDispatcher<R>(ctx).dispatch<P1,P2,P3,P4,P5>(mFunc, p1, p2, p3, p4, p5);
			}
		};

		template <class R, class P1, class P2, class P3, class P4, class P5, class P6> class ScriptFunction6 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)(P1,P2,P3,P4,P5,P6);

		public:
			ScriptFunction6(const string &name, R (*func)(P1, P2, P3, P4, P5, P6)) : ScriptFunctionBase(name, 6), mFunc(func) { }
			~ScriptFunction6() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				P1 *p1 = ctx->getParm<P1>(0);
				P2 *p2 = ctx->getParm<P2>(1);
				P3 *p3 = ctx->getParm<P3>(2);
				P4 *p4 = ctx->getParm<P4>(3);
				P5 *p5 = ctx->getParm<P5>(4);
				P6 *p6 = ctx->getParm<P6>(5);
				if (!p1 || !p2 || !p3 || !p4 || !p5 || !p6)
				{
					ctx->raiseError("Invalid argument passed to script function " + getName() + ".");
					return;
				}

				ScriptFunctionDispatcher<R>(ctx).dispatch<P1,P2,P3,P4,P5,P6>(mFunc, p1, p2, p3, p4, p5, p6);
			}
		};

		template <class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7> class ScriptFunction7 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)(P1,P2,P3,P4,P5,P6,P7);

		public:
			ScriptFunction7(const string &name, R (*func)(P1, P2, P3, P4, P5, P6, P7)) : ScriptFunctionBase(name, 7), mFunc(func) { }
			~ScriptFunction7() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				P1 *p1 = ctx->getParm<P1>(0);
				P2 *p2 = ctx->getParm<P2>(1);
				P3 *p3 = ctx->getParm<P3>(2);
				P4 *p4 = ctx->getParm<P4>(3);
				P5 *p5 = ctx->getParm<P5>(4);
				P6 *p6 = ctx->getParm<P6>(5);
				P7 *p7 = ctx->getParm<P7>(6);
				if (!p1 || !p2 || !p3 || !p4 || !p5 || !p6 || !p7)
				{
					ctx->raiseError("Invalid argument passed to script function " + getName() + ".");
					return;
				}

				ScriptFunctionDispatcher<R>(ctx).dispatch<P1,P2,P3,P4,P5,P6,P7>(mFunc, p1, p2, p3, p4, p5, p6, p7);
			}
		};

		template <class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8> class ScriptFunction8 : public ScriptFunctionBase
		{
		private:
			R (*mFunc)(P1,P2,P3,P4,P5,P6,P7,P8);

		public:
			ScriptFunction8(const string &name, R (*func)(P1, P2, P3, P4, P5, P6, P7, P8)) : ScriptFunctionBase(name, 8), mFunc(func) { }
			~ScriptFunction8() { }

		private:
			void run(ScriptFunctionContext *ctx) const
			{
				P1 *p1 = ctx->getParm<P1>(0);
				P2 *p2 = ctx->getParm<P2>(1);
				P3 *p3 = ctx->getParm<P3>(2);
				P4 *p4 = ctx->getParm<P4>(3);
				P5 *p5 = ctx->getParm<P5>(4);
				P6 *p6 = ctx->getParm<P6>(5);
				P7 *p7 = ctx->getParm<P7>(6);
				P8 *p8 = ctx->getParm<P8>(7);
				if (!p1 || !p2 || !p3 || !p4 || !p5 || !p6 || !p7 || !p8)
				{
					ctx->raiseError("Invalid argument passed to script function " + getName() + ".");
					return;
				}

				ScriptFunctionDispatcher<R>(ctx).dispatch<P1,P2,P3,P4,P5,P6,P7,P8>(mFunc, p1, p2, p3, p4, p5, p6, p7, p8);
			}
		};
	}
}

#endif