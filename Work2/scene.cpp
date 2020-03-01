#include "scene.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>

Scene::Scene( void )
{}

Scene::~Scene( void )
{}

bool Scene::intersect( const Ray &ray,
                       IntersectionRecord &intersection_record ) const
{
    bool intersection_result = false;
    IntersectionRecord tmp_intersection_record;
    std::size_t num_primitives = primitives_.size();

    // Loops over the list of primitives, testing the intersection of each primitive against the given ray 
    for ( std::size_t primitive_id = 0; primitive_id < num_primitives; primitive_id++ )
        if ( primitives_[primitive_id]->intersect( ray, tmp_intersection_record ) )
            if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0 ) )
            {
                intersection_record = tmp_intersection_record;
                intersection_result = true; // the ray intersects a primitive!
            }

    return intersection_result;
}

void Scene::load( void ) 
{
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 0.0f,  0.0f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ -0.5f, 0.0f, -1.0f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f,-0.5f, -2.0f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 1.5f, -3.0f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle(
        glm::vec3{0.0f, 0.0f, 0.2f},
        glm::vec3{0.5f, 0.0f, 0.2f},
        glm::vec3{0.5f, 0.7f, 0.2f})
       ));
}

static void printVec(const glm::vec3& vec)
{
    std::cout << "x: " << vec.x << "\ty: " << vec.y << "\tz: " << vec.z << std::endl;
}

static void clearFace(std::string& line)
{
    unsigned int i = 0;
    unsigned int j = 0;

    for(int k = 0; k < 3; k++)
    {
        while(i < line.length() && line[i] != '/')
        {
            i++;
        }

        j = i;

        while(j < line.length() && line[j] != ' ')
        {
            j++;
        }

        if(i < line.length() && j < line.length())
            line.erase(i, j - i);
    }
}

static void loadVertex(std::string& line, glm::vec3& vec)
{
    std::stringstream str;

    //discard 'v'
    str << (line.c_str() + 1);

    str >> vec.x >> vec.y >> vec.z;
}

static Triangle* loadFace(std::string& line, const std::vector<glm::vec3>& vs)
{
    int a, b, c;
    std::stringstream str;

    clearFace(line);

    str << (line.c_str() + 1);

    str >> a >> b >> c;

    return new Triangle(vs[a - 1], vs[b - 1], vs[c - 1]);
}

void Scene::load(const char* fileName)
{
    std::fstream in;
    std::vector<glm::vec3> vs;
    glm::vec3 vec;
    std::string line;
    Triangle *triangle;

    in.open(fileName, std::ios::in);

    while(!in.eof())
    {
        std::getline(in, line);

        switch(line.c_str()[0])
        {
            case 'v':
            loadVertex(line, vec);
            vs.push_back(vec);
            break;

            case 'f':
            triangle = loadFace(line, vs);
            primitives_.push_back( Primitive::PrimitiveUniquePtr(triangle));
            break;
        }
    }

    in.close();
}