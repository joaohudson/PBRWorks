#ifndef TRIANGLE
#define TRIANGLE

#include "primitive.h"

class Triangle : public Primitive
{
	public:
	Triangle( void );
	Triangle(const glm::vec3& v0,
			 const glm::vec3& v1,
			 const glm::vec3& v2);

	bool intersect(const Ray& ray, IntersectionRecord& intersection_record) const;
	void show(void) const;

	private:
	glm::vec3 v0, v1, v2;
	bool containsPoint(const glm::vec3& point) const;
};


#endif