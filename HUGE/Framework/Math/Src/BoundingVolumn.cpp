#include "Precompiled.h"
#include "BoundingVolumn.h"
//
using namespace H::Math;
 
bool H::Math::Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit)
{
	// https://truesculpt.googlecode.com/hg-history/Release%25200.8/Doc/ray_box_intersect.pdf

	// Returns true if ray intersects bounding box
	// Sets d1 to entry distance, d2 to exit distance (along ray.dir)

	Vector3 boxMin = aabb.mCentre - aabb.mExtend;
	Vector3 boxMax = aabb.mCentre + aabb.mExtend;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float divx = 1.0f / ray.dir.x;
	float divy = 1.0f / ray.dir.y;
	float divz = 1.0f / ray.dir.z;

	if (divx >= 0.0f)
	{
		tmin = (boxMin.x - ray.org.x) * divx;
		tmax = (boxMax.x - ray.org.x) * divx;
	}
	else
	{
		tmin = (boxMax.x - ray.org.x) * divx;
		tmax = (boxMin.x - ray.org.x) * divx;
	}
	if (divy >= 0.0f)
	{
		tymin = (boxMin.y - ray.org.y) * divy;
		tymax = (boxMax.y - ray.org.y) * divy;
	}
	else
	{
		tymin = (boxMax.y - ray.org.y) * divy;
		tymax = (boxMin.y - ray.org.y) * divy;
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	if (divz >= 0.0f)
	{
		tzmin = (boxMin.z - ray.org.z) * divz;
		tzmax = (boxMax.z - ray.org.z) * divz;
	}
	else
	{
		tzmin = (boxMax.z - ray.org.z) * divz;
		tzmax = (boxMin.z - ray.org.z) * divz;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	distEntry = tmin;
	distExit = tmax;
	return true;
}

bool H::Math::Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Translation(Vector3{ obb.center.x, obb.center.y, obb.center.z });
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);

	AABB aabb(Vector3::Zero(), obb.extend);
	return Intersect(Ray(org, dir), aabb, distEntry, distExit);
}
bool H::Math::Intersect(const Ray& ray, const Plane& plane, float& distance)
{
	const float orgDotN = Dot(ray.org, plane.n);
	const float dirDotN = Dot(ray.dir, plane.n);

	// Check if ray is parallel to the plane
	if (IsZero(dirDotN))
	{
		if (IsZero(orgDotN - plane.d))
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;
	return true;
}
bool H::Math::Intersect(const Ray & ray, const Vector3 & a, const Vector3 & b, const Vector3 & c, float & distance)
{
	// Reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

		// Find vectors for two edges sharing V1
	Vector3 e1 = b - a;
	Vector3 e2 = c - a;

	// Begin calculating determinant - also used to calculate u parameter
	Vector3 P = Cross(ray.dir, e2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = Dot(e1, P);
	// NOT CULLING
	if (IsZero(det))
	{
		return false;
	}

	float inv_det = 1.0f / det;

	// Calculate distance from V1 to ray origin
	Vector3 T = ray.org - a;

	// Calculate u parameter and test bound
	float u = Dot(T, P) * inv_det;

	// The intersection lies outside of the triangle
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	// Prepare to test v parameter
	Vector3 Q = Cross(T, e1);

	// Calculate V parameter and test bound
	float v = Dot(ray.dir, Q) * inv_det;

	// The intersection lies outside of the triangle
	if (v < 0.0f || u + v  > 1.0f)
	{
		return false;
	}

	// Ray intersection
	float t = Dot(e2, Q) * inv_det;
	if (t <= 0.0f)
	{
		// No hit, no win
		return false;
	}

	distance = t;
	return true;
}
bool H::Math::Intersect(const Vector3& point, const AABB& aabb)
{
	const Vector3 test = point - aabb.mCentre;
	if (abs(test.x) > aabb.mExtend.x) return false;
	if (abs(test.y) > aabb.mExtend.y) return false;
	if (abs(test.z) > aabb.mExtend.z) return false;
	return true;
}
bool H::Math::Intersect(const AABB& a, const AABB& b)
{
 	return (a.Min().x <= b.Max().x && a.Max().x >= b.Min().x)&&
		(a.Min().y <= b.Max().y && a.Max().y >= b.Min().y)&&
		(a.Min().z <= b.Max().z && a.Max().z >= b.Min().z);
}

//----------------------------------------------------------------------------------------------------

bool H::Math::Intersect(const Vector3& point, const OBB& obb)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Translation(Vector3{ obb.center.x, obb.center.y, obb.center.z });
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the point into the OBB's local space
	Vector3 localPoint = TransformCoord(point, matWorldInv);
	AABB aabb(Vector3::Zero(), obb.extend);

	// Test against local AABB
	return Math::Intersect(localPoint, aabb);
}

void H::Math::GetCorners(const OBB& obb, std::vector<Vector3>& corners)
{
	// Compute the local-to-world matrices
	Matrix4 matTrans = Translation(Vector3{ obb.center.x, obb.center.y, obb.center.z });
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;

	// Create a local AABB
	corners.clear();
	corners.push_back(Vector3(-obb.extend.x, -obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, -obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, -obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, -obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, obb.extend.y, -obb.extend.z));

	// Transform AABB into world space to form the OBB
	const uint32_t kNumCorners = (uint32_t)corners.size();
	for (uint32_t i = 0; i < kNumCorners; ++i)
	{
		corners[i] = TransformCoord(corners[i], matWorld);
	}
}

//----------------------------------------------------------------------------------------------------

bool H::Math::GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Translation(Vector3{ obb.center.x, obb.center.y, obb.center.z });
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);
	Ray localRay(org, dir);

	Plane planes[] =
	{
		Plane(0.0f,  0.0f, -1.0f, obb.extend.z),
		Plane(0.0f,  0.0f,  1.0f, obb.extend.z),
		Plane(0.0f, -1.0f,  0.0f, obb.extend.y),
		Plane(0.0f,  1.0f,  0.0f, obb.extend.y),
		Plane(-1.0f,  0.0f,  0.0f, obb.extend.x),
		Plane(1.0f,  0.0f,  0.0f, obb.extend.x)
	};

	uint32_t numIntersections = 0;
	for (uint32_t i = 0; i < 6; ++i)
	{
		const float d = Dot(org, planes[i].n);
		if (d > planes[i].d)
		{
			float distance = 0.0f;
			if (Intersect(localRay, planes[i], distance) && distance >= 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if (abs(pt.x) <= obb.extend.x + 0.01f &&
					abs(pt.y) <= obb.extend.y + 0.01f &&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = planes[i].n;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;
}

Vector3 H::Math::GetClosestPoint(const Ray& ray, const Vector3& point)
{
	Vector3 orgToPoint = point - ray.org;
	float d = Dot(orgToPoint, ray.dir);
	return ray.org + (ray.dir * d);
}