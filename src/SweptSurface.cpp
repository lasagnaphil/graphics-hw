//
// Created by lasagnaphil on 5/1/18.
//

#include <string>

#include "SweptSurface.h"

std::string rtrim(std::string& str) {
    str.erase(str.find_last_not_of(" \n\r\t")+1);
    return str;
}

void getline(std::ifstream& istream, std::string& line) {
    while (true) {
        std::getline(istream, line);
        if (!rtrim(line).empty()) break;
    }
}

std::shared_ptr<Mesh> SweptSurface::constructFromFile(const char* filename, std::shared_ptr<Material> material) {
    using std::cerr;
    using std::endl;

    std::ifstream istream(filename);
    std::string line;

    // get title
    getline(istream, line);

    if (line != "BSPLINE") {
        std::cerr << "Sorry, we only allow BSPLINE" << std::endl;
    }
    int numCrossSections = 0;
    int numControlPoints = 0;

    getline(istream, line);
    numCrossSections = std::stoi(line);
    getline(istream, line);
    numControlPoints = std::stoi(line);

    std::vector<Polygon2D> polygons;
    std::vector<Transform> transforms;

    for (int j = 0; j < numCrossSections; ++j) {
        Transform transform;
        Polygon2D polygon;
        for (int i = 0; i < numControlPoints; ++i) {
            glm::vec2 pos;
            getline(istream, line);
            std::istringstream(line) >> pos.x >> pos.y;
            polygon.push_back(pos);
        }
        getline(istream, line);
        float scale = std::stof(line);
        transform.scale = {scale, scale, scale};
        getline(istream, line);
        std::istringstream(line) >> transform.rotation.x
                                 >> transform.rotation.y
                                 >> transform.rotation.z
                                 >> transform.rotation.w;
        getline(istream, line);
        std::istringstream(line) >> transform.position.x
                                 >> transform.position.y
                                 >> transform.position.z;
        polygons.push_back(polygon);
        transforms.push_back(transform);
    }
    return construct(polygons, transforms, std::move(material), numControlPoints, numCrossSections);
}

glm::vec3 normalOfTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    return glm::normalize(glm::cross(v2 - v1, v3 - v1));
}

std::shared_ptr<Mesh>
SweptSurface::construct(std::vector<SweptSurface::Polygon2D> controlPolygons,
                        std::vector<Transform> polygonTransforms,
                        std::shared_ptr<Material> material,
                        int numControlPoints, int numCrossSections) {

    assert(controlPolygons.size() == numCrossSections);
    assert(polygonTransforms.size() == numCrossSections);

    int interpPolygonSize = numControlPoints * (1 << INTERP_FACTOR);
    std::vector<Polygon2D> bsplines;
    for (auto polygon : controlPolygons) {
        Polygon2D prev;
        Polygon2D next = polygon;
        for (int iter = 0; iter < INTERP_FACTOR; iter++) {
            prev = next;
            int numVertices = numControlPoints * (1 << iter);
            next.clear();
            next.resize(numVertices * 2);
            for (int i = 0; i < numVertices; ++i) {
                int iprev = (i - 1) % numVertices;
                int inext = (i + 1) % numVertices;
                next[2*i] = (prev[iprev] + 6.f * prev[i] + prev[inext]) / 8.f;
                next[2*i+1] = (prev[i] + prev[inext]) / 2.f;
            }
        }
        bsplines.push_back(next);
    }

    /*
    for (auto& bspline : bsplines) {
        std::cout << std::endl;
        for (auto& pos : bspline) {
            std::cout << pos.x << " " << pos.y << std::endl;
        }
    }
     */

    //
    // Calculate the final B-spline positions
    //
    int finalBsplineCount = (numCrossSections - 1) * TRANSFORM_INTERP_SIZE + 1;
    std::vector<std::vector<Vertex>> finalBsplines(finalBsplineCount);

    for (auto& v : finalBsplines) {
        v.reserve(interpPolygonSize);
    }

    for (int i = 0; i < numCrossSections - 1; ++i) {
        auto& tprev = polygonTransforms[i];
        auto& tnext = polygonTransforms[i + 1];

        auto& bspline = bsplines[i];

        for (int j = 0; j < TRANSFORM_INTERP_SIZE; ++j) {
            Transform transform;
            float alpha = (float) (j) / TRANSFORM_INTERP_SIZE;
            transform.position = (1 - alpha) * tprev.position + alpha * tnext.position;
            transform.rotation = glm::slerp(tprev.rotation, tnext.rotation, alpha);
            transform.scale = (1 - alpha) * tprev.scale + alpha * tnext.scale;

            for (auto& pos : bspline) {
                Vertex vertex;
                vertex.position = transform.toMat4() * glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
                finalBsplines[i * TRANSFORM_INTERP_SIZE + j].push_back(vertex);
            }
        }
    }

    // Don't forget to include the last one!
    auto& lastBspline = bsplines[numCrossSections - 1];
    auto& lastTransform = polygonTransforms[numCrossSections - 1];
    for (auto& pos : lastBspline) {
        Vertex vertex;
        vertex.position = lastTransform.toMat4() * glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
        finalBsplines[finalBsplineCount - 1].push_back(vertex);
    }

    /*
    for (auto& bspline : finalBsplines) {
        std::cout << std::endl;
        for (auto& vertex : bspline) {
            std::cout << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << " " << std::endl;
        }
    }
     */

    //
    // Find all the normals of the bspline vertices
    //
    for (int i = 0; i < finalBsplineCount; ++i) {
        for (int j = 0; j < interpPolygonSize; ++j) {
            int jnext = (j + 1) % interpPolygonSize;
            int jprev = (j - 1) % interpPolygonSize;
            finalBsplines[i][j].normal = {0.0f, 0.0f, 0.0f};
            if (i != finalBsplineCount - 1) {
                finalBsplines[i][j].normal += normalOfTriangle(
                        finalBsplines[i][j].position,
                        finalBsplines[i + 1][j].position,
                        finalBsplines[i][jnext].position
                );
                finalBsplines[i][j].normal += normalOfTriangle(
                        finalBsplines[i][j].position,
                        finalBsplines[i][jprev].position,
                        finalBsplines[i + 1][j].position
                );
            }
            if (i != 0) {
                finalBsplines[i][j].normal += normalOfTriangle(
                        finalBsplines[i][j].position,
                        finalBsplines[i][jnext].position,
                        finalBsplines[i - 1][j].position
                );
                finalBsplines[i][j].normal += normalOfTriangle(
                        finalBsplines[i][j].position,
                        finalBsplines[i - 1][j].position,
                        finalBsplines[i][jprev].position
                );
            }
            finalBsplines[i][j].normal = glm::normalize(finalBsplines[i][j].normal);
        }
    }

    // create the final vertices list
    std::vector<Vertex> vertices;
    for (int i = 0; i < finalBsplineCount - 1; ++i) {
        for (int j = 0; j < interpPolygonSize; ++j) {
            int jnext = (j + 1) % interpPolygonSize;
            auto& v1 = finalBsplines[i][j];
            auto& v2 = finalBsplines[i+1][j];
            auto& v3 = finalBsplines[i][jnext];
            auto& v4 = finalBsplines[i+1][jnext];
            v1.texCoords = {0.0f, 0.0f}; v2.texCoords = {1.0f, 0.0f}; v3.texCoords = {0.0f, 1.0f}; v4.texCoords = {1.0f, 1.0f};
            vertices.insert(vertices.end(), {v1, v2, v3, v2, v3, v4});
        }
    }

    /*
    for (auto& vertex : vertices) {
        std::cout << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << " " << std::endl;
    }
     */

    std::shared_ptr<Mesh> mesh(new Mesh(vertices, {}, std::move(material), GL_TRIANGLES, false));
    return mesh;
}
