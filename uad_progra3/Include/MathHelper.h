#pragma once

// These are some simple math helpers to enable the template to render a spinning cube. It is not a complete math library.
// You can replace this with your favorite math library that's suitable for your target platforms, e.g. DirectXMath or GLM.

#include "CVector3.h"
#include <math.h>

namespace MathHelper
{
	struct Matrix4
	{
		Matrix4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}

		float m[4][4];

	};

	inline static Matrix4 IdentityMatrix()
	{
		return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
            		   0.0f, 1.0f, 0.0f, 0.0f,
			           0.0f, 0.0f, 1.0f, 0.0f,
			           0.0f, 0.0f, 0.0f, 1.0f);
	}

	// Rotate around Y
	inline static Matrix4 SimpleModelMatrix(float angleInRadians)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);
		
		return Matrix4(cosine, 0.0f, -sine,  0.0f,
			           0.0f,   1.0f, 0.0f,   0.0f,
			           sine,   0.0f, cosine, 0.0f,
			           0.0f,   0.0f, 0.0f,   1.0f);
	}

	// Rotate around Y + Translate
	inline static Matrix4 ModelMatrix(float angleInRadians, CVector3 translation)
	{
		float cosine = cosf(angleInRadians);
		float sine = sinf(angleInRadians);

		// NOTE: Remember this needs to be a transpose of the rotation matrix
		return Matrix4(cosine,             0.0f,               -sine,              0.0f,
			           0.0f,               1.0f,               0.0f,               0.0f,
			           sine,               0.0f,               cosine,             0.0f,
			           translation.getX(), translation.getY(), translation.getZ(), 1.0f);
	}

	inline static Matrix4 SimpleViewMatrix(float cameraDistance)
	{
		// Camera is at 60 degrees to the ground, in the YZ plane.
		// Camera Look-At is hardcoded to (0, 0, 0).
		// Camera Up is hardcoded to (0, 1, 0).
		const float sqrt3over2 = 0.86603f;

		return Matrix4(1.0f,  0.0f,      0.0f,           0.0f,
			           0.0f, sqrt3over2, 0.5f,           0.0f,
			           0.0f, -0.5f,      sqrt3over2,     0.0f,
			           0.0f,  0.0f,     -cameraDistance, 1.0f);
	}

	inline static Matrix4 SimpleProjectionMatrix(float aspectRatio)
	{
		// Far plane is at 50.0f, near plane is at 1.0f.
		// FoV is hardcoded to pi/3.
		const float cotangent = 1 / tanf(3.14159f / 6.0f);

		return Matrix4(cotangent / aspectRatio, 0.0f,      0.0f,                   0.0f,
			           0.0f,                    cotangent, 0.0f,                   0.0f,
			           0.0f,                    0.0f,     -50.0f / (50.0f - 1.0f), (-50.0f * 1.0f) / (50.0f - 1.0f),
			           0.0f,                    0.0f,     -1.0f,                   0.0f);
	}

	inline static Matrix4 FirstPersonModelMatrix(float anglePitchRadians, float angleYawRadians, CVector3 camPos)
	{
		float cosPitch = cosf(anglePitchRadians);
		float sinPitch = sinf(anglePitchRadians);
		float cosYaw = cosf(-angleYawRadians);
		float sinYaw = sinf(-angleYawRadians);

		CVector3 xaxis (cosPitch, 0, -sinPitch );
		CVector3 yaxis (sinPitch * sinYaw, cosYaw, cosPitch * sinYaw );
		CVector3 zaxis (sinPitch * cosYaw, -sinYaw, cosYaw * cosPitch );

		Matrix4 viewMatrix (
			xaxis.getX(),            yaxis.getX(),          zaxis.getX(),		0,
			xaxis.getY(),            yaxis.getY(),          zaxis.getY(),		0,
			xaxis.getZ(),            yaxis.getZ(),			zaxis.getZ(),		0,
			-xaxis.dot(camPos),		-yaxis.dot(camPos),		-zaxis.dot(camPos), 1
		);

		return viewMatrix;
	}
}