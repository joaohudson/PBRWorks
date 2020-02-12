#include "perspective_camera.h"

PerspectiveCamera::PerspectiveCamera( void ){}
PerspectiveCamera::PerspectiveCamera(
	float minX,
	float maxX,
	float minY,
	float maxY, 
	const glm::ivec2 &resolution,
	const glm::vec3 &position,
	const glm::vec3 &up_vector,
	const glm::vec3 &look_at ) :
	Camera(resolution, position, up_vector, look_at)
	{
		this->minX = minX;
		this->maxX = maxX;
		this->minY = minY;
		this->maxY = maxY;
	}
					   
Ray PerspectiveCamera::getWorldSpaceRay( const glm::vec2 &pixel_coord ) const
{
	float width = maxX - minX;
    float height = maxY - minY;

    glm::vec3 origin{ pixel_coord[0] / static_cast< float >( resolution_[0] ) * width + minX,
                      pixel_coord[1] / static_cast< float >( resolution_[1] ) * height + minY,
                      0.0f };

    return Ray{ position_, 
                glm::normalize( onb_.getBasisMatrix() * origin - position_ ) };
}