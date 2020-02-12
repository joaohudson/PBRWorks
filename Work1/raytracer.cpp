#include "raytracer.h"

RayTracer::RayTracer( Camera &camera,
                      const Scene &scene,
                      const glm::vec3 background_color,
                      Buffer &buffer ) :
        camera_( camera ),
        scene_( scene ),
        background_color_{ background_color },
        buffer_( buffer )
{}

void RayTracer::integrate( void )
{
    IntersectionRecord intersection_record;

    // Image space origin (i.e. x = 0 and y = 0) at the top left corner.

    // Loops over image rows
    for ( std::size_t y = 0; y < buffer_.v_resolution_; y++ )
    {
        std::stringstream progress_stream;
        progress_stream << "\r  progress .........................: "
                        << std::fixed << std::setw( 6 )
                        << std::setprecision( 2 )
                        << 100.0 * y / ( buffer_.v_resolution_ - 1 )
                        << "%";

        std::clog << progress_stream.str();

        // Loops over image columns
        for ( std::size_t x = 0; x < buffer_.h_resolution_; x++ )
        {
           intersection_record.t_ = std::numeric_limits< double >::max();

            Ray ray{ camera_.getWorldSpaceRay( glm::vec2{ x + 0.5f, y + 0.5f } ) };

            if (scene_.intersect( ray, intersection_record ) )
                //buffer_.buffer_data_[x][y] = glm::vec3{ 1.0f, 0.0f, 0.0f };
                //buffer_.buffer_data_[x][y] = glm::vec3{ intersection_record.t_ * 0.2f };
                //iluminação improvisada estilo rasterização(a luz está na própria câmera)
                buffer_.buffer_data_[x][y] = glm::abs(glm::dot(intersection_record.normal_, camera_.direction_))
                * glm::vec3(1.0f, 1.0f, 1.0f) / (intersection_record.t_ >= 1.0f ? intersection_record.t_ : 1.0f);// * intersection_record.t_ * 0.2f;
        }
    }
    
    //teste
    std::clog << std::endl;
    glm::vec3 plus{0.0f};
    int w = buffer_.h_resolution_ - 1;
    int h = buffer_.v_resolution_ - 1;

    for(int y = 1; y < h; y++)
    {
        std::stringstream progress_stream;
        progress_stream << "\r  progress .........................: "
                        << std::fixed << std::setw( 6 )
                        << std::setprecision( 2 )
                        << 100.0 * y / ( h - 1 )
                        << "%";

        std::clog << progress_stream.str();

        for(int x = 1; x < w; x++)
        {
            //apply media filter:
            plus += buffer_.buffer_data_[x + -1][y + -1] * 0.25f;
            plus += buffer_.buffer_data_[x + 1][y + -1] * 0.25f;
            plus += buffer_.buffer_data_[x + -1][y + 1] * 0.25f;
            plus += buffer_.buffer_data_[x + 1][y + 1] * 0.25f;

            buffer_.buffer_data_[x][y] = plus;
            plus = glm::vec3{0.0f, 0.0f, 0.0f};
        }
    }

    std::clog << std::endl;
}

