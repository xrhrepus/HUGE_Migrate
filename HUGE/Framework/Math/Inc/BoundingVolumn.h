#ifndef HUGE_MATH_BOUNDINGVOLUMN_H
#define HUGE_MATH_BOUNDINGVOLUMN_H
#include "MathFunctions.h"

namespace H::Math
{
	struct AABB
	{

		 AABB() = default;
		constexpr AABB(const Vector3& centre, const Vector3& extend) 
			: mCentre(centre), mExtend(extend)
		{}

		Vector3 mCentre;
		Vector3 mExtend;


		constexpr Vector3 Min() const { return mCentre - mExtend; }
		constexpr Vector3 Max() const { return mCentre + mExtend; }
		constexpr static AABB FromMinMax(const Vector3& min, const Vector3& max)
		{
			return { (min + max) * 0.5f , (max - min) * 0.5f };
		}


	};
	struct Sphere
	{
		 Sphere() = default;
		constexpr Sphere(const Vector3& centre,const float& radius) 
			: mCentre(centre), mRadius(radius)
		{}


		Vector3 mCentre;
		float mRadius;

	};

	struct Rect
	{
		Rect() = default;
		constexpr Rect(const Vector2& centre, const Vector2& extend) 
			: mCentre(centre), mExtend(extend)
		{}

		Vector2 mCentre;
		Vector2 mExtend;

	};
	struct Circle
	{
		Circle() = default;
		
		constexpr Circle(const Vector2& centre, float radius)
			: centre(centre), radius(radius)
		{}
		Vector2 centre;
		float radius;
	};
	struct LineSegment
	{
		LineSegment() = default;
		constexpr LineSegment (const Vector3& vs, const Vector3& ve)
			:from(vs), to(ve)
		{
		}


		Vector3 from;
		Vector3 to;
	};

	struct LineSegment2D
	{
		LineSegment2D() = default;

		constexpr LineSegment2D(const Vector2& vs, const Vector2& ve)
			:from(vs), to(ve)
		{
		}

		Vector2 from;
		Vector2 to;;
	};
	struct Plane
	{
		Vector3 n;
		float d;

		constexpr Plane() : n(0.0f, 1.0f, 0.0f), d(0.0f) {}
		constexpr Plane(float a, float b, float c, float d) : n(a, b, c), d(d) {}
	};
	struct Ray
	{
		Vector3 org;
		Vector3 dir;

		constexpr Ray() : org(0.0f, 0.0f, 0.0f), dir(0.0f, 0.0f, 1.0f) {}
		constexpr Ray(float x, float y, float z, float dx, float dy, float dz) : org(x, y, z), dir(dx, dy, dz) {}
		constexpr Ray(const Vector3& org, const Vector3& dir) : org(org), dir(dir) {}
	};



	struct OBB
	{
		Vector3 center;
		Vector3 extend;
		Quaternion orientation;

		constexpr OBB()
			: center(0.0f, 0.0f, 0.0f)
			, extend(1.0f, 1.0f, 1.0f)
			, orientation(0.0f, 0.0f, 0.0f, 1.0f)
		{}

		constexpr OBB(float x, float y, float z, float sx, float sy, float sz)
			: center(x, y, z)
			, extend(sx, sy, sz)
			, orientation(0.0f, 0.0f, 0.0f, 1.0f)
		{}

		constexpr OBB(float x, float y, float z, float sx, float sy, float sz, const Quaternion& orientation)
			: center(x, y, z)
			, extend(sx, sy, sz)
			, orientation(orientation)
		{}

		constexpr OBB(const Vector3& center, const Vector3& extend, const Quaternion& orientation)
			: center(center)
			, extend(extend)
			, orientation(orientation)
		{}
	};
	
	inline Vector3 GetPoint(const Ray& ray, float distance) { return ray.org + (ray.dir * distance); }

	bool Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit);
	bool Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit);
	bool Intersect(const Ray& ray, const Plane& plane, float& distance);
	bool Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance);
	bool Intersect(const Vector3& point, const AABB& aabb);
	bool Intersect(const Vector3& point, const OBB& obb);
	bool Intersect(const AABB& a, const AABB& b);


	void GetCorners(const OBB& obb, std::vector<Vector3>& corners);
	bool GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal);
	Vector3 GetClosestPoint(const Ray& ray, const Vector3& point);

}


#endif // !HUGE_MATH_BOUNDINGVOLUMN_H
