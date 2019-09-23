#include "main.hpp"

namespace M
{
	void VectorTransform(Vector& in1, Vector *in2, Vector& out)
	{
		out.x = in1.Dot(in2[0]) + in2[0].z;
		out.y = in1.Dot(in2[1]) + in2[1].z;
		out.z = in1.Dot(in2[2]) + in2[2].z;
	}

	Angle CalcAngle(Vector src, Vector dst)
	{
		Angle angles;
		Vector delta = src - dst;

		M::VectorAngles(delta, angles);
		//angles.Normalize();

		return angles;
	}

	void NormalizeAngles(Angle& angles)
	{
		for (auto i = 0; i < 3; i++)
		{
			while (angles[i] < -180.0f)
				angles[i] += 360.0f;
			while (angles[i] > 180.0f)
				angles[i] -= 360.0f;
		}
	}

	float GetFOV(const Angle &viewangles, const Angle &aimangles)
	{
		Vector ang, aim;

		M::AngleVectors(viewangles, &aim);
		M::AngleVectors(aimangles, &ang);

		return RAD2DEG(std::acos(aim.Dot(ang) / aim.LengthSqr()));
	}

	void MatrixAngles(const Vector matrix[3], Angle &angles)
	{
		Vector forward, left, up;

		//
		// Extract the basis vectors from the matrix. Since we only need the Z
		// component of the up vector, we don't get X and Y.
		//
		forward[0] = matrix[0][0];
		forward[1] = matrix[1][0];
		forward[2] = matrix[2][0];
		left[0] = matrix[0][1];
		left[1] = matrix[1][1];
		left[2] = matrix[2][1];
		up[2] = matrix[2][2];

		float xyDist = std::sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

		// enough here to get angles?
		if (xyDist > 0.001f)
		{
			// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the X axis
			angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

			// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
			angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

			// (roll)	z = ATAN( left.z, up.z );
			angles[2] = RAD2DEG(atan2f(left[2], up[2]));
		}
		else	// forward is mostly Z, gimbal lock-
		{
			// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
			angles[1] = RAD2DEG(atan2f(-left[0], left[1]));

			// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
			angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

			// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
			angles[2] = 0;
		}
	}

	inline void MatrixPosition(const Vector matrix[3], Vector &position)
	{
		position[0] = matrix[0][3];
		position[1] = matrix[1][3];
		position[2] = matrix[2][3];
	}

	inline void MatrixAngles(const Vector matrix[3], Angle &angles, Vector &position)
	{
		M::MatrixAngles(matrix, angles);
		MatrixPosition(matrix, position);
	}

	bool DirectXWorldToScreen(const Vector &point, Vector &screen)
	{
		DirectX::XMFLOAT3 out_2d;

		DirectX::XMVECTOR out = DirectX::XMVector3Project(DirectX::XMVectorSet(point.x, point.y, point.z, 0.f), 0, 0, (float)O::Screen->Width,
			(float)O::Screen->Height, 0, 1, O::Matrices->ProjectionMatrix, O::Matrices->ViewMatrix, O::Matrices->WorldMatrix);

		DirectX::XMStoreFloat3(&out_2d, out);

		if (out_2d.z > 0.f)
		{
			screen = { out_2d.x, out_2d.y, out_2d.z };
			return true;
		}
		return false;
	}

	void AngleVectors(const Angle& angles, Vector* forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[1]), sy, cy);
		SinCos(DEG2RAD(angles[0]), sp, cp);

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	void M::VectorAngles(const Vector& forward, Angle& angles)
	{
		if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;	// Pitch	up	 / down
			angles[1] = 0.0f;									// Yaw		left / right
		}
		else
		{
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
			angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

			if (angles[1] > 90)
				angles[1] -= 180;
			else if (angles[1] < 90)
				angles[1] += 180;
			else if (angles[1] == 90)
				angles[1] = 0;
		}

		angles[2] = 0.0f;
	}

	//void VectorAngles(const Vector& forward, Angle& angles)
	//{
	//	if (forward.x == 0.0f && forward.y == 0.0f)
	//	{
	//		angles.x = (forward.y > 0.0f) ? 270.0f : 90.0f;		// Pitch (up/down)
	//		angles.y = 0.0f;									// Yaw left/right
	//	}
	//	else
	//	{
	//		angles.x = RAD2DEG(std::atan2(-forward.z, forward.Length2D()));
	//		angles.y = RAD2DEG(std::atan2(forward.y, forward.x));
	//		
	//		if (angles.y > 90)
	//			angles.y -= 180;
	//		else if (angles.y < 90)
	//			angles.y += 180;
	//		else if (angles.y == 90)
	//			angles.y = 0;
	//	}
	//}

	//bool WorldToScreen(const Vector &pos, ImVec2& screen)
	//{
	//	float w = 0.f;
	//	Vector res;

	//	const VMatrix &view_matrix = (VMatrix &)O::Game->ViewMatrix;

	//	res[0]	= view_matrix[0][0] * pos[0] + view_matrix[1][0] * pos[1] + view_matrix[2][0] * pos[2] + view_matrix[3][0];
	//	res[1]	= view_matrix[0][1] * pos[0] + view_matrix[1][1] * pos[1] + view_matrix[2][1] * pos[2] + view_matrix[3][1];
	//	res[2]	= view_matrix[0][2] * pos[0] + view_matrix[1][2] * pos[1] + view_matrix[2][2] * pos[2] + view_matrix[3][2];
	//	w		= view_matrix[0][3] * pos[0] + view_matrix[1][3] * pos[1] + view_matrix[2][3] * pos[2] + view_matrix[3][3];

	//	res *= (1.0f / w);

	//	if (res.z < 0.01f)
	//		return false;

	//	float x = O::Screen->Width / 2;
	//	float y = O::Screen->Height / 2;
	//	x += 0.5 * res[0] * O::Screen->Width + 0.5;
	//	y -= 0.5 * res[1] * O::Screen->Height + 0.5;

	//	screen.x = x;
	//	screen.y = y;
	//	return true;
	//}
}