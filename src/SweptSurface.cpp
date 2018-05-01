//
// Created by lasagnaphil on 5/1/18.
//

#include "SweptSurface.h"

std::shared_ptr<Mesh> SweptSurface::constructFromFile(const char* filename) {
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
    return construct(polygons, transforms, numControlPoints, numCrossSections);
}

std::shared_ptr<Mesh>
SweptSurface::construct(std::vector<SweptSurface::Polygon2D> controlPolygons, std::vector<Transform> polygonTransforms,
                        int numControlPoints, int numCrossSections) {

    assert(controlPolygons.size() == numCrossSections);
    assert(polygonTransforms.size() == numCrossSections);

    int interpPolygonSize = numControlPoints * (1 << INTERP_FACTOR);
    std::vector<Polygon2D> bsplines;
    for (auto polygon : controlPolygons) {
        polygon.resize(interpPolygonSize);
        Polygon2D otherPolygon;
        otherPolygon.resize(interpPolygonSize);
        Polygon2D* prevP = &otherPolygon;
        Polygon2D* nextP = &polygon;

        for (int iter = 0; iter < INTERP_FACTOR; iter++) {
            Polygon2D* temp = prevP;
            prevP = nextP;
            nextP = temp;
            for (int i = 0; i < numControlPoints * (1 << iter); i += 4) {
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
        Polygon2D result = *nextP;
        bsplines.push_back(result);
    }

    std::vector<WireframeVertex> vertices;
    std::vector<WireframeVertex> verticeBuffer1, verticeBuffer2;
    verticeBuffer1.reserve(interpPolygonSize);
    verticeBuffer2.reserve(interpPolygonSize);
    std::vector<WireframeVertex>* prevVertices = &verticeBuffer1;
    std::vector<WireframeVertex>* nextVertices = &verticeBuffer2;

    for (int i = 0; i < numCrossSections - 1; ++i) {
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
                WireframeVertex vertex;
                vertex.position = transform.toMat4() * glm::vec4(pos.x, pos.y, 0.0f, 0.0f);
                nextVertices->push_back(vertex);
            }

            if (j > 0) {
                // using prev and next vertices, we create the triangle strip
                for (int k = 0; k < interpPolygonSize; ++k) {
                    int kp = (k + 1) % interpPolygonSize;
                    WireframeVertex v1 = (*prevVertices)[k];
                    v1.barycentric = {1.0f, 0.0f, 0.0f};
                    WireframeVertex v2 = (*prevVertices)[kp];
                    v2.barycentric = {0.0f, 1.0f, 0.0f};
                    WireframeVertex v3 = (*nextVertices)[k];
                    v3.barycentric = {0.0f, 0.0f, 1.0f};
                    WireframeVertex v4 = (*prevVertices)[kp];
                    v4.barycentric = {1.0f, 0.0f, 0.0f};
                    WireframeVertex v5 = (*prevVertices)[kp];
                    v5.barycentric = {0.0f, 1.0f, 0.0f};
                    WireframeVertex v6 = (*nextVertices)[k];
                    v6.barycentric = {0.0f, 0.0f, 1.0f};
                    vertices.insert(vertices.end(), {v1, v2, v3, v4, v5, v6});
                }
            }
        }
        auto temp = prevVertices;
        prevVertices = nextVertices;
        nextVertices = temp;
    }

    std::shared_ptr<Mesh> mesh(new Mesh(vertices));
    return mesh;
}
