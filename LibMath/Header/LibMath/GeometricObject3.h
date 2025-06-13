#ifndef GEOMETRIC_OBJECT3_H
#define	GEOMETRIC_OBJECT3_H

#include "LibMath/Vector/Vector3.h"
#include "LibMath/Matrix/Matrix4.h"

namespace LibMath
{
	namespace Geometry3D
	{
				
		class Object3D
		{
		public:
			virtual Vector3 const getCenter(void) const = 0;
			/*virtual Vector3& getCenter(void) = 0;*/
			virtual void update(LibMath::Matrix4& transMat) = 0;
			virtual ~Object3D() = default;

		};

		// Todo: Implement tests for all the geometric objects
		class Point : public Object3D
		{
		public:
							Point() = default;
							Point(const float x, const float y, const float z);
							Point(const Point& other);
							~Point() = default;

			Point&			operator=(const Point& other);

			virtual Vector3 const		getCenter(void) const override { return this->toVector3(); };

			virtual void			update(LibMath::Matrix4& transMat) override;

			const Vector3	toVector3(void) const { return Vector3(m_x, m_y, m_z); };

			float&			getX(void) { return m_x; };
			float&			getY(void) { return m_y; };
			float&			getZ(void) { return m_z; };

			float const		getX(void) const { return m_x; };
			float const		getY(void) const { return m_y; };
			float const		getZ(void) const { return m_z; };

			float			getDistanceSquared(const Point&) const;
			float			getDistance(const Point&) const;

			Vector3			operator-(const Point& point);



		private:
			float			m_x = 0.0f;
			float			m_y = 0.0f;
			float			m_z = 0.0f;

		};

		class Line : public Object3D
		{
		public:
									Line() = default;
									Line(const Point& point, const Vector3& dir);   // Director vector should be a unit vector
									Line(const Line& other);
									~Line() = default;

			Line&					operator=(const Line& other);
			Line&					operator*(const float& scalair);

			virtual void					update(LibMath::Matrix4& transMat) override;

			virtual Vector3 const				getCenter(void) const override { return m_origin.toVector3(); };
			/*virtual Vector3&					getCenter(void) override { return m_origin; };*/

			const Point&			getOrigin(void) const { return m_origin; };
			const Vector3&			getDirection(void) const { return m_direction; };

			
			Point			m_origin;
			Vector3			m_direction;
			float			m_length = 0.f;
		private:

		};

		class Plan : public Object3D
		{
		public:
							Plan() = default;
							Plan(const Vector3& normal, const float& distance);
							Plan(const Plan& other);
							~Plan() = default;

			Plan&					operator=(const Plan& other);

			const Vector3			normal(void) const { return m_normal; };
			const float				distance(void) const { return m_distance; };

			


		private:
			Vector3			m_normal;
			float			m_distance = 0.f;

		};

		class AABB :public Object3D
		{
		public:
							AABB() = default;
							AABB(const Point& center, const float& width, const float& height, const float& depth);
							AABB(const AABB& other);
							~AABB() = default;

			AABB&			operator=(const AABB& other);

			virtual void			update(LibMath::Matrix4& transMat) override;

			virtual Vector3 const		getCenter(void) const override { return m_center.toVector3(); };
			/*virtual Vector3&			getCenter(void) override { return m_center; };*/

			Point&			center(void) { return m_center; };
			const Point&	center(void) const { return m_center; };

			float			extentX(void) const;
			float			extentY(void) const;
			float			extentZ(void) const;

			float&			width(void) { return m_width; };
			float&			height(void) { return m_height; };
			float&			depth(void) { return m_depth; };
		private:

			Point			m_center;
			float			m_width = 0.f; // length on X axis
			float			m_height = 0.f; // length on Y axis
			float			m_depth = 0.f; // length on Z axis
		};

		class OBB : public Object3D
		{
		public:
							OBB() = default;
							OBB(const Point& center, const float& width, const float& height, const float& m_depth, const Radian& rotation);
							OBB(const OBB& other);
							~OBB() = default;

			OBB&			operator=(const OBB& other);

			virtual void			update(LibMath::Matrix4& transMat) override;

			virtual Vector3 const		getCenter(void) const override { return m_center.toVector3(); };
			/*virtual Vector3&			getCenter(void) override { return m_center; };*/

			Radian			rotation(void);


		private:
			Point			m_center;
			float			m_width = 0.f;
			float			m_height = 0.f;
			float			m_depth = 0.f;
			LibMath::Radian m_rotation; 
		};

		class Sphere : public Object3D
		{
		public:
							Sphere() = default;
							Sphere(const Point& center, const float& radius);
							Sphere(const Sphere& other);
							~Sphere() = default;

			Sphere&			operator=(const Sphere& other);

			virtual void					update(LibMath::Matrix4& transMat) override;

			virtual Vector3 const				getCenter(void) const override { return m_center.toVector3(); };

			const Point&			center(void) const { return m_center; };
			const float&			radius(void) const { return m_radius; };

		private:
			Point			m_center;
			float			m_radius = 0.f;
		};

		class Capsule :public Object3D
		{
		public:
			Capsule() = default;
			Capsule(const Point& pointA, const Point& pointB, const float& radius);
			Capsule(const Capsule& other);
			~Capsule() = default;

			Capsule& operator=(const Capsule& other);

			virtual void			update(LibMath::Matrix4& transMat) override;

			virtual Vector3 const		getCenter(void) const override { return m_pointA.toVector3(); };
			

			Point			m_pointA;
			Point			m_pointB;
			float			m_radius = 0.f;

		private:

		};

		Point			getClosestToAABB(const LibMath::Geometry3D::AABB&, const Point&);
		Point			getClosestToSegment(const Point&, const Point&, const Point&);

	}
	
}

#endif // !GEOMETRIC_OBJECT3_H
