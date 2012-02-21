
#ifndef _GIMBLE_TRANSFORM_
#define _GIMBLE_TRANSFORM_

#include "GimbleReqs.h"
#include "GimbleVector3.h"
#include "GimbleQuaternion.h"
#include "GimbleMatrix.h"

namespace Gimble
{
	class GIMBLE_API Transform
	{
	private:
		Quaternion mOrientation;
		Vector3 mTranslation;
		Vector3 mScale;
		Matrix mMat;
		Matrix mInverseMat;

	public:
		Transform(const Quaternion &orientation, const Vector3 &translation, const Vector3 &scale) : mOrientation(orientation), mTranslation(translation), mScale(scale) 
		{ 
			update(); 
		}

		~Transform() { }

		inline Quaternion getOrientation() const 
		{
			return mOrientation;
		}

		inline Vector3 getTranslation() const 
		{
			return mTranslation;
		}

		inline Vector3 getScale() const 
		{
			return mScale;
		}

		inline Matrix getMat() const 
		{
			return mMat;
		}

		inline Matrix getInverseMat() const 
		{
			return mInverseMat;
		}

		inline void setOrientation(const Quaternion &orientation)
		{
			mOrientation = orientation;
		}

		inline void setTranslation(const Vector3 &translation)
		{
			mTranslation = translation;
		}

		inline void setScale(const Vector3 &scale)
		{
			mScale = scale;
		}

		void update()
		{
			mOrientation.normalise();

			mMat = 
				Matrix::createScale(mScale) * 
				Matrix(mOrientation) * 
				Matrix::createTranslation(mTranslation);

			mInverseMat = 
				Matrix::createTranslation(-mTranslation) * 
				Matrix(mOrientation.conjugate()) * 
				Matrix::createScale(1.0f / mScale);
		}
	};
}

#endif