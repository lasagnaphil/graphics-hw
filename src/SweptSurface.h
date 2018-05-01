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
    static constexpr int INTERP_FACTOR = 4;
    static constexpr int TRANSFORM_INTERP_SIZE = 4;

    using Polygon2D = std::vector<glm::vec2>;
    using Polygon3D = std::vector<glm::vec3>;

    static std::shared_ptr<Mesh> constructFromFile(const char* filename, std::shared_ptr<Material> material) {
        using std::cerr;
        using std::endl;

        std::ifstream istream(filename);
        std::string line;
        // get title
        if (!std::getline(istream, line)) {
            std::cerr << "File empty!" << std::endl;
        }

        if (line != "BSPLINE") {
            std::cerr << "Sorry, we only allow BSPLINE" << std::endl;
        }
        int numCrossSections = 0;
        int numControlPoints = 0;

        std::getline(istream, line);
        numCrossSections = std::stoi(line);
        std::getline(istream, line);
        numControlPoints = std::stoi(line);

        std::vector<Polygon2D> polygons;
        std::vector<Transform> transforms;

        for (int j = 0; j < numCrossSections; ++j) {
            Transform transform;
            Polygon2D polygon;
            for (int i = 0; i < numControlPoints; ++i) {
                glm::vec2 pos;
                std::getline(istream, line);
                std::istringstream(line) >> pos.x >> pos.y;
                polygon.push_back(pos);
            }
            std::getline(istream, line);
            float scale = std::stof(line);
            transform.scale = {scale, scale, scale};
            std::getline(istream, line);
            std::istringstream(line) >> transform.rotation.w
                                     >> transform.rotation.x
                                     >> transform.rotation.y
                                     >> transform.rotation.z;
            std::getline(istream, line);
            std::istringstream(line) >> transform.position.x
                                     >> transform.position.y
                                     >> transform.position.z;

            polygons.push_back(polygon);
            transforms.push_back(transform);
        }
        return construct(polygons, transforms, material, numControlPoints, numCrossSections);
    }

    static std::shared_ptr<Mesh> construct(std::vector<Polygon2D> controlPolygons,
                                           std::vector<Transform> polygonTransforms,
                                           std::shared_ptr<Material> material,
                                           int numControlPoints,
                                           int numCrossSections) {

        std::vector<Polygon2D> bsplines;
        for (auto polygon : controlPolygons) {
            polygon.reserve(numControlPoints);
            Polygon2D otherPolygon;
            otherPolygon.reserve(numControlPoints);
            Polygon2D* prevP = &otherPolygon;
            Polygon2D* nextP = &polygon;

            for (int iter = 0; iter < INTERP_FACTOR; iter++) {
                Polygon2D* temp = prevP;
                prevP = nextP;
                nextP = temp;
                for (int i = 0; i < iter * 4; i += 4) {
                    auto& next = *nextP;
                    auto& prev = *prevP;
                    next[2*i] = (prev[i] + prev[i+1]) / 2.f;
                    next[2*i+1] = (prev[i] + 6.f * prev[i+1] + prev[i+2]) / 8.f;
                    next[2*i+2] = (prev[i+1] + prev[i+2]) / 2.f;
                    next[2*i+3] = (prev[i+1] + 6.f * prev[i+2] + prev[i+3]) / 8.f;
                    next[2*i+4] = (prev[i+2] + prev[i+3]) / 2.f;
                    next[2*i+5] = (prev[i+2] + 6.f * prev[i+2] + prev[i+3]) / 8.f;
                    next[2*i+6] = (prev[i+3] + prev[i]) / 2.f;
                    next[2*i+7] = (prev[i+3] + 6.f * prev[i+3] + prev[i]) / 8.f;
                }

            }
            bsplines.push_back(*prevP);
        }

        std::vector<Vertex> vertices;
        for (int i = 0; i < polygonTransforms.size() - 1; ++i) {
            auto& tprev = polygonTransforms[i];
            auto& tnext = polygonTransforms[i + 1];

            auto& bspline = bsplines[i];

            for (int j = 0; j < TRANSFORM_INTERP_SIZE; ++j) {
                Transform transform;
                float alpha = (float)(j) / TRANSFORM_INTERP_SIZE;
                transform.position = alpha * tprev.position + (1 - alpha) * tnext.position;
                transform.rotation = glm::slerp(tprev.rotation, tnext.rotation, alpha);
                transform.scale = alpha * tprev.scale + (1 - alpha) * tnext.scale;

                for (auto& pos : bspline) {
                    Vertex vertex;
                    vertex.position = transform.toMat4() * glm::vec4(pos.x, pos.y, 0.0f, 0.0f);
                    vertex.normal = {};
                    vertex.texCoords = {};
                    vertices.push_back(vertex);
                }
            }
        }

        std::shared_ptr<Mesh> mesh(new Mesh(vertices, {}, std::move(material), GL_TRIANGLES, false));
    }
};


#endif //GENGINE_SWEPTSURFACE_H
