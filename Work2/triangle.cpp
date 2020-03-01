#include "triangle.h"
#include <iostream>
//calcula o ângulo entre 2 vetores:
#define ANGLE(a, b) (glm::acos(glm::dot(glm::normalize(a), glm::normalize(b))))

Triangle::Triangle(){}

Triangle::Triangle(
	const glm::vec3& v0,
	const glm::vec3& v1,
	const glm::vec3& v2) : 
	v0{v0}, v1{v1}, v2{v2}{}

bool Triangle::intersect(const Ray& ray, IntersectionRecord& intersection_record) const
{
	glm::vec3 point;
	glm::vec3 a = v1 - v0;
	glm::vec3 b = v1 - v2;
	glm::vec3 n = glm::cross(a, b);
	const float d = glm::dot(v0, n);
	const float h = glm::dot(ray.direction_, n);
	float t;

	if(h == 0.0f)
		return false;
	
	t = (d  - glm::dot(ray.origin_, n))/ h;
	point = ray.origin_ + ray.direction_ * t;

	intersection_record.t_ = t;
	intersection_record.position_ = point;
	intersection_record.normal_ = n;

	return containsPoint(point);
}

bool Triangle::containsPoint(const glm::vec3& point) const
{
	glm::vec3 u0 = v0 - point;
	glm::vec3 u1 = v1 - point;
	glm::vec3 u2 = v2 - point;

	const float a = ANGLE(u0, u1);
	const float b = ANGLE(u1, u2);
	const float c = ANGLE(u2, u0);
	//verifica uma igualde aproximada de um ângulo máximo(deve ser uma aproximação baixa para compensar o erro numérico)
	return (a + b + c) >= 6.28f;
}

void Triangle::show() const
{
	std::cout << "a: x=" << v0.x << "y=" << v0.y << "z=" << v0.z << std::endl;
	std::cout << "b: x=" << v1.x << "y=" << v1.y << "z=" << v1.z << std::endl;
	std::cout << "c: x=" << v2.x << "y=" << v2.y << "z=" << v2.z << std::endl;
}

