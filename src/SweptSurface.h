//
// Created by lasagnaphil on 5/1/18.
//

#ifndef GENGINE_SWEPTSURFACE_H
#define GENGINE_SWEPTSURFACE_H


#include <array>
#include "Mesh.h"
#include "Transform.h"

class SweptSurface {
public:
    static constexpr int INTERP_FACTOR = 3;
    static constexpr int TRANSFORM_INTERP_SIZE = 8;

    using Polygon2D = std::vector<glm::vec2>;
    using Polygon3D = std::vector<glm::vec3>;

    enum class Type {
        Bspline, CatmullRom
    };

    static std::shared_ptr<Mesh> constructFromFile(const char* filename, std::shared_ptr<Material> material);

    static std::shared_ptr<Mesh> construct(std::vector<SweptSurface::Polygon2D> controlPolygons,
                                           std::vector<Transform> polygonTransforms,
                                           std::shared_ptr<Material> material,
                                           int numControlPoints, int numCrossSections, Type type);
};


#endif //GENGINE_SWEPTSURFACE_H
