#pragma once
#include <SlateEngine/Engine/Graphics/Vertex.h>
#include <vector>
#include <string>
#include <map>
#include <functional>

template<class Vertex = VertexPNT, class Index = DWORD>
struct MeshData
{
    std::vector<Vertex> vVertex;
    std::vector<Index> vIndices;

    MeshData()
    {
        static_assert(sizeof(Index) == 2 || sizeof(Index) == 4, "Size of index bigger than 2 and 4!");
        static_assert(std::is_unsigned<Index>::value, "Index type is not Integer!");
    }
};


namespace BuiltInMesh {

    struct VertexData
    {
        vec3f pos;
        vec3f normal;
        vec4f tangent;
        vec4f color;
        vec2f tex;
    };

    template<class Vertex>
    inline void InsertVertexElement(Vertex& vertexDst, const VertexData& vertexSrc)
    {
        static std::string semanticName;
        static const std::map<std::string, std::pair<size_t, size_t>> semanticSizeMap = {
            {"POSITION", std::pair<size_t, size_t>(0, 12)},
            {"NORMAL", std::pair<size_t, size_t>(12, 24)},
            {"TANGENT", std::pair<size_t, size_t>(24, 40)},
            {"COLOR", std::pair<size_t, size_t>(40, 56)},
            {"TEXCOORD", std::pair<size_t, size_t>(56, 64)}
        };

        for (size_t i = 0; i < ARRAYSIZE(Vertex::inputLayout); i++)
        {
            semanticName = Vertex::inputLayout[i].SemanticName;
            const auto& range = semanticSizeMap.at(semanticName);
            memcpy_s(reinterpret_cast<char*>(&vertexDst) + Vertex::inputLayout[i].AlignedByteOffset,
                range.second - range.first,
                reinterpret_cast<const char*>(&vertexSrc) + range.first,
                range.second - range.first);
        }

    }


    template<class Vertex = VertexPNT, class Index = DWORD>
    MeshData<Vertex, Index> CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20,
        const vec4f& color = { 1.0f, 1.0f, 1.0f, 1.0f });

    template<class Vertex = VertexPNT, class Index = DWORD>
    MeshData<Vertex, Index> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f,
        const vec4f& color = { 1.0f, 1.0f, 1.0f, 1.0f });

    template<class Vertex = VertexPNT, class Index = DWORD>
    MeshData<Vertex, Index> CreateCylinder(float radius = 1.0f, float height = 2.0f, UINT slices = 20, UINT stacks = 10,
        float texU = 1.0f, float texV = 1.0f, const vec4f& color = { 1.0f, 1.0f, 1.0f, 1.0f });

    template<class Vertex = VertexPNT, class Index = DWORD>
    MeshData<Vertex, Index> CreateCylinderNoCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20, UINT stacks = 10,
        float texU = 1.0f, float texV = 1.0f, const vec4f& color = { 1.0f, 1.0f, 1.0f, 1.0f });

    template<class Vertex, class Index>
    inline MeshData<Vertex, Index> CreateBox(float width, float height, float depth, const vec4f& color)
    {
        using namespace DirectX;

        MeshData<Vertex, Index> meshData;
        meshData.vVertex.resize(24);

        VertexData vertexDataArr[24];
        float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

        vertexDataArr[0].pos = vec3f(w2, -h2, -d2);
        vertexDataArr[1].pos = vec3f(w2, h2, -d2);
        vertexDataArr[2].pos = vec3f(w2, h2, d2);
        vertexDataArr[3].pos = vec3f(w2, -h2, d2);

        vertexDataArr[4].pos = vec3f(-w2, -h2, d2);
        vertexDataArr[5].pos = vec3f(-w2, h2, d2);
        vertexDataArr[6].pos = vec3f(-w2, h2, -d2);
        vertexDataArr[7].pos = vec3f(-w2, -h2, -d2);

        vertexDataArr[8].pos = vec3f(-w2, h2, -d2);
        vertexDataArr[9].pos = vec3f(-w2, h2, d2);
        vertexDataArr[10].pos = vec3f(w2, h2, d2);
        vertexDataArr[11].pos = vec3f(w2, h2, -d2);

        vertexDataArr[12].pos = vec3f(w2, -h2, -d2);
        vertexDataArr[13].pos = vec3f(w2, -h2, d2);
        vertexDataArr[14].pos = vec3f(-w2, -h2, d2);
        vertexDataArr[15].pos = vec3f(-w2, -h2, -d2);

        vertexDataArr[16].pos = vec3f(w2, -h2, d2);
        vertexDataArr[17].pos = vec3f(w2, h2, d2);
        vertexDataArr[18].pos = vec3f(-w2, h2, d2);
        vertexDataArr[19].pos = vec3f(-w2, -h2, d2);

        vertexDataArr[20].pos = vec3f(-w2, -h2, -d2);
        vertexDataArr[21].pos = vec3f(-w2, h2, -d2);
        vertexDataArr[22].pos = vec3f(w2, h2, -d2);
        vertexDataArr[23].pos = vec3f(w2, -h2, -d2);

        for (UINT i = 0; i < 4; ++i)
        {

            vertexDataArr[i].normal = vec3f(1.0f, 0.0f, 0.0f);
            vertexDataArr[i].tangent = vec4f(0.0f, 0.0f, 1.0f, 1.0f);
            vertexDataArr[i].color = color;

            vertexDataArr[i + 4].normal = vec3f(-1.0f, 0.0f, 0.0f);
            vertexDataArr[i + 4].tangent = vec4f(0.0f, 0.0f, -1.0f, 1.0f);
            vertexDataArr[i + 4].color = color;

            vertexDataArr[i + 8].normal = vec3f(0.0f, 1.0f, 0.0f);
            vertexDataArr[i + 8].tangent = vec4f(1.0f, 0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 8].color = color;

            vertexDataArr[i + 12].normal = vec3f(0.0f, -1.0f, 0.0f);
            vertexDataArr[i + 12].tangent = vec4f(-1.0f, 0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 12].color = color;

            vertexDataArr[i + 16].normal = vec3f(0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 16].tangent = vec4f(-1.0f, 0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 16].color = color;

            vertexDataArr[i + 20].normal = vec3f(0.0f, 0.0f, -1.0f);
            vertexDataArr[i + 20].tangent = vec4f(1.0f, 0.0f, 0.0f, 1.0f);
            vertexDataArr[i + 20].color = color;
        }

        for (UINT i = 0; i < 6; ++i)
        {
            vertexDataArr[i * 4].tex = vec2f(0.0f, 1.0f);
            vertexDataArr[i * 4 + 1].tex = vec2f(0.0f, 0.0f);
            vertexDataArr[i * 4 + 2].tex = vec2f(1.0f, 0.0f);
            vertexDataArr[i * 4 + 3].tex = vec2f(1.0f, 1.0f);
        }

        for (UINT i = 0; i < 24; ++i)
        {
            InsertVertexElement(meshData.vVertex[i], vertexDataArr[i]);
        }

        meshData.vIndices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        };

        return meshData;
    }

    template<class Vertex, class Index>
    inline MeshData<Vertex, Index> CreateSphere(float radius, UINT levels, UINT slices, const vec4f& color)
    {
        using namespace DirectX;

        MeshData<Vertex, Index> meshData;
        UINT vertexCount = 2 + (levels - 1) * (slices + 1);
        UINT indexCount = 6 * (levels - 1) * slices;
        meshData.vVertex.resize(vertexCount);
        meshData.vIndices.resize(indexCount);

        VertexData vertexData;
        Index vIndex = 0, iIndex = 0;

        float phi = 0.0f, theta = 0.0f;
        float per_phi = XM_PI / levels;
        float per_theta = XM_2PI / slices;
        float x, y, z;

        vertexData = { vec3f(0.0f, radius, 0.0f), vec3f(0.0f, 1.0f, 0.0f), vec4f(1.0f, 0.0f, 0.0f, 1.0f), color, vec2f(0.0f, 0.0f) };
        InsertVertexElement(meshData.vVertex[vIndex++], vertexData);

        for (UINT i = 1; i < levels; ++i)
        {
            phi = per_phi * i;

            for (UINT j = 0; j <= slices; ++j)
            {
                theta = per_theta * j;
                x = radius * sinf(phi) * cosf(theta);
                y = radius * cosf(phi);
                z = radius * sinf(phi) * sinf(theta);

                vec3f pos = vec3f(x, y, z), normal;
                normal = pos.normalized();

                vertexData = { pos, normal, vec4f(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, vec2f(theta / XM_2PI, phi / XM_PI) };
                InsertVertexElement(meshData.vVertex[vIndex++], vertexData);
            }
        }

        vertexData = { vec3f(0.0f, -radius, 0.0f), vec3f(0.0f, -1.0f, 0.0f),
            vec4f(-1.0f, 0.0f, 0.0f, 1.0f), color, vec2f(0.0f, 1.0f) };
        InsertVertexElement(meshData.vVertex[vIndex++], vertexData);


        if (levels > 1)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.vIndices[iIndex++] = 0;
                meshData.vIndices[iIndex++] = j % (slices + 1) + 1;
                meshData.vIndices[iIndex++] = j;
            }
        }


        for (UINT i = 1; i < levels - 1; ++i)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.vIndices[iIndex++] = (i - 1) * (slices + 1) + j;
                meshData.vIndices[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
                meshData.vIndices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

                meshData.vIndices[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
                meshData.vIndices[iIndex++] = i * (slices + 1) + j;
                meshData.vIndices[iIndex++] = (i - 1) * (slices + 1) + j;
            }
        }

        if (levels > 1)
        {
            for (UINT j = 1; j <= slices; ++j)
            {
                meshData.vIndices[iIndex++] = (levels - 2) * (slices + 1) + j;
                meshData.vIndices[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
                meshData.vIndices[iIndex++] = (levels - 1) * (slices + 1) + 1;
            }
        }


        return meshData;
    }

    template<class Vertex, class Index>
    inline MeshData<Vertex, Index> CreateCylinder(float radius, float height, UINT slices, UINT stacks, float texU, float texV, const vec4f& color)
    {
        using namespace DirectX;

        auto meshData = CreateCylinderNoCap<Vertex, Index>(radius, height, slices, stacks, texU, texV, color);
        UINT vertexCount = (slices + 1) * (stacks + 3) + 2;
        UINT indexCount = 6 * slices * (stacks + 1);
        meshData.vVertex.resize(vertexCount);
        meshData.vIndices.resize(indexCount);

        float h2 = height / 2;
        float theta = 0.0f;
        float per_theta = XM_2PI / slices;

        Index vIndex = (slices + 1) * (stacks + 1), iIndex = 6 * slices * stacks;
        Index offset = vIndex;
        VertexData vertexData;

        vertexData = { vec3f(0.0f, h2, 0.0f), vec3f(0.0f, 1.0f, 0.0f),
                        vec4f(1.0f, 0.0f, 0.0f, 1.0f), color, vec2f(0.5f, 0.5f) };
        InsertVertexElement(meshData.vVertex[vIndex++], vertexData);

        for (UINT i = 0; i <= slices; ++i)
        {
            theta = i * per_theta;
            float u = cosf(theta) * radius / height + 0.5f;
            float v = sinf(theta) * radius / height + 0.5f;
            vertexData = { vec3f(radius * cosf(theta), h2, radius * sinf(theta)), vec3f(0.0f, 1.0f, 0.0f),
                            vec4f(1.0f, 0.0f, 0.0f, 1.0f), color, vec2f(u, v) };
            InsertVertexElement(meshData.vVertex[vIndex++], vertexData);
        }

        vertexData = { vec3f(0.0f, -h2, 0.0f), vec3f(0.0f, -1.0f, 0.0f),
                        vec4f(-1.0f, 0.0f, 0.0f, 1.0f), color, vec2f(0.5f, 0.5f) };
        InsertVertexElement(meshData.vVertex[vIndex++], vertexData);

        for (UINT i = 0; i <= slices; ++i)
        {
            theta = i * per_theta;
            float u = cosf(theta) * radius / height + 0.5f;
            float v = sinf(theta) * radius / height + 0.5f;
            vertexData = { vec3f(radius * cosf(theta), -h2, radius * sinf(theta)), vec3f(0.0f, -1.0f, 0.0f),
                            vec4f(-1.0f, 0.0f, 0.0f, 1.0f), color, vec2f(u, v) };
            InsertVertexElement(meshData.vVertex[vIndex++], vertexData);
        }



        for (UINT i = 1; i <= slices; ++i)
        {
            meshData.vIndices[iIndex++] = offset;
            meshData.vIndices[iIndex++] = offset + i % (slices + 1) + 1;
            meshData.vIndices[iIndex++] = offset + i;
        }

        offset += slices + 2;
        for (UINT i = 1; i <= slices; ++i)
        {
            meshData.vIndices[iIndex++] = offset;
            meshData.vIndices[iIndex++] = offset + i;
            meshData.vIndices[iIndex++] = offset + i % (slices + 1) + 1;
        }

        return meshData;
    }

    template<class Vertex, class Index>
    inline MeshData<Vertex, Index> CreateCylinderNoCap(float radius, float height, UINT slices, UINT stacks,
        float texU, float texV, const vec4f& color)
    {
        using namespace DirectX;

        MeshData<Vertex, Index> meshData;
        UINT vertexCount = (slices + 1) * (stacks + 1);
        UINT indexCount = 6 * slices * stacks;
        meshData.vVertex.resize(vertexCount);
        meshData.vIndices.resize(indexCount);

        float h2 = height / 2;
        float theta = 0.0f;
        float per_theta = XM_2PI / slices;
        float stackHeight = height / stacks;

        VertexData vertexData;


        UINT vIndex = 0;
        for (UINT i = 0; i < stacks + 1; ++i)
        {
            float y = -h2 + i * stackHeight;

            for (UINT j = 0; j <= slices; ++j)
            {
                theta = j * per_theta;
                float u = theta / XM_2PI;
                float v = 1.0f - (float)i / stacks;
                vertexData = { vec3f(radius * cosf(theta), y, radius * sinf(theta)), vec3f(cosf(theta), 0.0f, sinf(theta)),
                    vec4f(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, vec2f(u * texU, v * texV) };
                InsertVertexElement(meshData.vVertex[vIndex++], vertexData);
            }
        }

        UINT iIndex = 0;
        for (UINT i = 0; i < stacks; ++i)
        {
            for (UINT j = 0; j < slices; ++j)
            {
                meshData.vIndices[iIndex++] = i * (slices + 1) + j;
                meshData.vIndices[iIndex++] = (i + 1) * (slices + 1) + j;
                meshData.vIndices[iIndex++] = (i + 1) * (slices + 1) + j + 1;

                meshData.vIndices[iIndex++] = i * (slices + 1) + j;
                meshData.vIndices[iIndex++] = (i + 1) * (slices + 1) + j + 1;
                meshData.vIndices[iIndex++] = i * (slices + 1) + j + 1;
            }
        }

        return meshData;
    }
}

