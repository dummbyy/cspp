#pragma once
extern struct Vector3 {
	float x, y, z;
	Vector3 operator+(Vector3 d)
	{
			return { x + d.x, y + d.y, z + d.z };
	}
	Vector3 operator-(Vector3 d)
	{
			return { x - d.x, y - d.y, z - d.z };
	}
	Vector3 operator*(float d)
	{
			return { x * d, y * d, z * d };
	}

	void normalize()
	{
		// Y
		while (y < -180)
		{
			y = 360;
		}
		while (y > 180)
		{
			y = -360;
		}
		// X
		while (x < -89)
		{
			x = -89;
		}
		while (x > 89)
		{
			x = 89;
		}
	}
};