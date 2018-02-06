/**
 * @brief FBX geometry data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#include "geometryproxy.h"

#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"
#include "../property/propertyfloat64array.h"
#include "../property/propertyint32array.h"

using namespace o3d::studio::fbxi;

static void triangulate(
        const o3d::SmartArrayInt32 oldIndices,
        o3d::ArrayInt32 &indices,
        o3d::ArrayInt32 &toOld);

static void addVertex(GeometryProxy::Vertex &v, o3d::Int32 index);

static void splatVertexData(
    const o3d::UInt32 sizeType,
    o3d::SmartArrayFloat &out,
    GeometryProxy::MapType mapType,
    const o3d::SmartArrayDouble &data,
    const o3d::SmartArrayInt32 &indices,
    const o3d::SmartArrayInt32 &originalIndices);

static void remapVertexData(
        const o3d::UInt32 sizeType,
        o3d::SmartArrayFloat &out,
        const o3d::ArrayInt32 &map);

GeometryProxy::GeometryProxy(FBXNode *node) :
    ObjectProxy(node)
{
    if (!m_node || m_node->name() != "Geometry") {
        O3D_ERROR(E_InvalidParameter("Must be a Geometry node"));
    }

    FBXNode *version = m_node->child("GeometryVersion");
    if (version) {
        m_version = version->directAsInt32();
    }

    if (m_version != 124) {
        O3D_ERROR(E_InvalidParameter("Must be a Geometry node version 124"));
    }

    m_objectType = OBJECT_GEOMETRY;
}

o3d::SmartArrayFloat GeometryProxy::vertexData(GeometryProxy::VertexData v)
{
    return m_vertexData[v];
}

o3d::SmartArrayInt32 GeometryProxy::indices()
{
    return m_indices;
}

o3d::SmartArrayInt32 GeometryProxy::materials()
{
    return m_materials;
}

o3d::SmartArrayInt32 GeometryProxy::edges()
{
    SmartArrayInt32 array;

    FBXNode *edges = m_node->child("Edges");
    if (edges) {
        PropertyInt32Array *data = static_cast<PropertyInt32Array*>(edges->property(0));
        array = data->value();
    }

    return array;
}

o3d::UInt32 GeometryProxy::numMaterials() const
{
    return (UInt32)m_perMaterialIndices.size();
}

o3d::SmartArrayInt32 GeometryProxy::indices(o3d::UInt32 id)
{
    if (id > (UInt32)m_perMaterialIndices.size()) {
        O3D_ERROR(E_IndexOutOfRange("Material index"));
    }

    return m_perMaterialIndices[id];
}

o3d::Bool GeometryProxy::processGeometry()
{
    SmartArrayDouble vertices;

    FBXNode *verticesNode = m_node->child("Vertices");
    if (verticesNode) {
        PropertyFloat64Array *data = static_cast<PropertyFloat64Array*>(verticesNode->property(0));
        vertices = data->value();
    } else {
        O3D_ERROR(E_InvalidFormat("Missing vertices array"));
    }

    SmartArrayInt32 originalIndices;

    FBXNode *polygonVertexIndex = m_node->child("PolygonVertexIndex");
    if (polygonVertexIndex) {
        PropertyInt32Array *data = static_cast<PropertyInt32Array*>(polygonVertexIndex->property(0));
        originalIndices = data->value();
    } else {
        O3D_ERROR(E_InvalidFormat("Missing vertices indices array"));
    }

    // triangulate
    ArrayInt32 toOldIndices;
    triangulate(originalIndices, m_toOldVertices, toOldIndices);

    m_vertexData[DATA_VERTICES].allocate(m_toOldVertices.getSize()*3);

    Float *pOfs = m_vertexData[DATA_VERTICES].getData();
    UInt32 p;

    for (UInt32 i = 0; i < m_toOldVertices.getSize(); ++i) {
        p = m_toOldVertices[i] * 3;

        *pOfs++ = vertices[p];
        *pOfs++ = vertices[p+1];
        *pOfs++ = vertices[p+2];
    }

    // some vertices can be unused, so this isn't necessarily the same size as toOldVertices.
    m_toNewVertices.setSize(vertices.getSize());

    for (UInt32 i = 0; i < m_toOldVertices.getSize(); ++i) {
        UInt32 old = m_toOldVertices[i];
        addVertex(m_toNewVertices[old], i);
    }

    FBXNode *layerElementMaterial = m_node->child("LayerElementMaterial");
    if (layerElementMaterial) {
        FBXNode *version = layerElementMaterial->child("Version");
        if (!version || version->directAsInt32() != 101) {
            O3D_ERROR(E_InvalidFormat(String("Must be a {0} node version 101").arg(layerElementMaterial->name())));
        }

        FBXNode *mappingInformationType = layerElementMaterial->child("MappingInformationType");
        if (!mappingInformationType) {
            O3D_ERROR(E_InvalidFormat("Missing MappingInformationType node"));
        }

        FBXNode *referenceInformationType = layerElementMaterial->child("ReferenceInformationType");
        if (!referenceInformationType) {
            O3D_ERROR(E_InvalidFormat("Missing ReferenceInformationType node"));
        }

        String mapTypeName = mappingInformationType->directAsString();
        String refEltName = referenceInformationType->directAsString();

        if (mapTypeName == "ByPolygonVertex" && refEltName == "IndexToDirect") {
            FBXNode *materialsData = layerElementMaterial->child("Materials");
            if (!materialsData) {
                O3D_ERROR(E_InvalidFormat(String("Missing {0} node").arg("Materials")));
            }

            PropertyInt32Array *materialsArray = static_cast<PropertyInt32Array*>(materialsData->property(0));
            if (!materialsArray) {
                O3D_ERROR(E_InvalidFormat(String("Missing {0} data").arg("Materials")));
            }

            SmartArrayInt32 orginalMaterialsArray = materialsArray->value();
            m_materials.allocate(m_vertexData[DATA_VERTICES].getSize() / 3);

            // unitialized materials
            for (UInt32 i = 0; i < m_materials.getSize(); ++i) {
                m_materials[i] = -1;
            }

            Int32 tmpI = 0, c = 0;
            for (UInt32 poly = 0; poly < orginalMaterialsArray.getSize(); ++poly) {
                // @todo and regroup by material id to form many indices arrays
//                UInt32 triCount = getTriCountFromPoly(originalIndices, &tmpI);
//                for (UInt32 i = 0; i < triCount; ++i) {
//                    m_materials[c] = orginalMaterialsArray[poly];
//                    ++c;
//                }
            }
        } else {
            if (mapTypeName != "AllSame") {
                O3D_ERROR(E_InvalidFormat(String("Mapping of Materials data is not supported")));
            }
        }
    }

    // uvs
    FBXNode *layerElementUV = m_node->child("LayerElementUV");
    if (layerElementUV) {
        SmartArrayDouble array;
        SmartArrayInt32 indices;
        MapType mapping;

        readVertexData(layerElementUV,
               "UV",
               "UVIndex",
               array,
               indices,
               mapping);

        if (!array.isEmpty()) {
            splatVertexData(2, m_vertexData[DATA_UVS], mapping, array, indices, originalIndices);
            remapVertexData(2, m_vertexData[DATA_UVS], toOldIndices);
        }
    }

    // tangents
    FBXNode *layerElementTangents = m_node->child("LayerElementTangents");
    if (layerElementTangents) {
        SmartArrayDouble array;
        SmartArrayInt32 indices;
        MapType mapping;

        if (layerElementTangents->child("Tangent")) {
            readVertexData(layerElementTangents,
                           "Tangent",
                           "TangentIndex",
                           array,
                           indices,
                           mapping);
        } else if (layerElementTangents->child("Tangents")) {
            readVertexData(layerElementTangents,
                           "Tangents",
                           "TangentsIndex",
                           array,
                           indices,
                           mapping);
        }

        if (!array.isEmpty()) {
            splatVertexData(3, m_vertexData[DATA_TANGENTS], mapping, array, indices, originalIndices);
            remapVertexData(3, m_vertexData[DATA_TANGENTS], toOldIndices);
        }
    }

    // colors
    FBXNode *layerElementColor = m_node->child("LayerElementColor");
    if (layerElementColor) {
        SmartArrayDouble array;
        SmartArrayInt32 indices;
        MapType mapping;

        readVertexData(layerElementColor,
               "Color",
               "ColorIndex",
               array,
               indices,
               mapping);

        if (!array.isEmpty()) {
            splatVertexData(4, m_vertexData[DATA_COLORS], mapping, array, indices, originalIndices);
            remapVertexData(4, m_vertexData[DATA_COLORS], toOldIndices);
        }
    }

    // normals
    FBXNode *layerElementNormal = m_node->child("LayerElementNormal");
    if (layerElementNormal) {
        SmartArrayDouble array;
        SmartArrayInt32 indices;
        MapType mapping;

        readVertexData(layerElementNormal,
               "Normals",
               "NormalsIndex",
               array,
               indices,
               mapping);

        if (!array.isEmpty()) {
            splatVertexData(3, m_vertexData[DATA_NORMALS], mapping, array, indices, originalIndices);
            remapVertexData(3, m_vertexData[DATA_NORMALS], toOldIndices);
        }
    }

    // layer (MultiLayer) if multiLayer many Layer nodes
//    FBXNode *layer = m_node->child("Layer");
//    if (layer) {
//        // is this usefull ?
//        FBXNode *version = layer->child("Version");
//        if (!version || version->directAsInt32() != 100) {
//            O3D_ERROR(E_InvalidFormat(String("Must be a {0} node version 100").arg(layer->name())));
//        }

//        // @todo many LayerElement nodes
//        //  Layer: LayerElement
//        //   LayerElement: Type
//        //   LayerElement: TypedIndex
//        //  Layer: LayerElement
//        //   LayerElement: Type
//        //   LayerElement: TypedIndex
//    }

    // MultiTake: 1
    // Shading: Y
    // Culling: "CullingOff"

    mergeVertexData();
    mergeMaterials();

    return True;
}

o3d::Bool GeometryProxy::readVertexData(
        FBXNode *node,
        const o3d::String &name,
        const o3d::String &indexName,
        o3d::SmartArrayDouble &data,
        o3d::SmartArrayInt32 &indices,
        MapType &mapType)
{
    if (!node) {
        return False;
    }

    FBXNode *version = node->child("Version");
    if (!version || version->directAsInt32() != 101) {
        O3D_ERROR(E_InvalidFormat(String("Must be a {0} node version 101").arg(node->name())));
    }

    FBXNode *mappingInformationType = node->child("MappingInformationType");
    if (!mappingInformationType) {
        O3D_ERROR(E_InvalidFormat("Missing MappingInformationType node"));
    }

    String mapTypeName = mappingInformationType->directAsString();
    if (mapTypeName == "ByPolygonVertex") {
        mapType = BY_POLYGON_VERTEX;
    } else if (mapTypeName == "ByPolygon") {
        mapType = BY_POLYGON;
    } else if (mapTypeName == "ByVertice" || mapTypeName == "ByVertex") {
        mapType = BY_VERTEX;
    }

    FBXNode *referenceInformationType = node->child("ReferenceInformationType");
    if (!referenceInformationType) {
        O3D_ERROR(E_InvalidFormat("Missing ReferenceInformationType node"));
    }

    String refEltName = referenceInformationType->directAsString();
    if (refEltName == "IndexToDirect") {
        FBXNode *indicesData = node->child(indexName);
        if (!indicesData) {
            O3D_ERROR(E_InvalidFormat(String("Missing {0} node").arg(name)));
        }

        PropertyInt32Array *indicesArray = static_cast<PropertyInt32Array*>(indicesData->property(0));
        if (!indicesArray) {
            O3D_ERROR(E_InvalidFormat(String("Missing {0} data").arg(indexName)));
        }

        indices = indicesArray->value();
    } else if (refEltName != "Direct") {
        O3D_ERROR(E_InvalidFormat("ReferenceInformationType is not Direct"));
    }

    FBXNode *dataNode = node->child(name);
    if (!dataNode) {
        O3D_ERROR(E_InvalidFormat(String("Missing {0} node").arg(name)));
    }

    PropertyFloat64Array *dataArray = static_cast<PropertyFloat64Array*>(dataNode->property(0));
    if (!dataArray) {
        O3D_ERROR(E_InvalidFormat(String("Missing {0} data").arg(name)));
    }

    data = dataArray->value();

    return True;
}

void GeometryProxy::mergeVertexData()
{
    // @todo need to refine because of doubled vertices during triangulation
}

void GeometryProxy::mergeMaterials()
{
    // @todo group indices by material id
}

static void triangulate(
        const o3d::SmartArrayInt32 oldIndices,
        o3d::ArrayInt32 &indices,
        o3d::ArrayInt32 &toOld)
{
    auto getIdx = [&oldIndices](o3d::Int32 i) -> o3d::Int32 {
        o3d::Int32 idx = oldIndices[i];
        return idx < 0 ? -idx - 1 : idx;
    };

    o3d::Int32 inPolygonIdx = 0;
    for (o3d::UInt32 i = 0; i < oldIndices.getSize(); ++i) {
        o3d::Int32 idx = getIdx(i);

        if (inPolygonIdx <= 2) {
            indices.push(idx);
            toOld.push(i);
        } else {
            indices.push(oldIndices[i - inPolygonIdx]);
            toOld.push(i - inPolygonIdx);
            indices.push(oldIndices[i - 1]);
            toOld.push(i - 1);
            indices.push(idx);
            toOld.push(i);
        }

        ++inPolygonIdx;
        if (oldIndices[i] < 0) {
            inPolygonIdx = 0;
        }
    }
}

static void addVertex(GeometryProxy::Vertex &v, o3d::Int32 index)
{
    if (v.index == -1) {
        v.index = index;
    } else if (v.next) {
        addVertex(*v.next, index);
    } else {
        v.next = new GeometryProxy::Vertex;
        v.next->index = index;
    }
}

static void splatVertexData(
        const o3d::UInt32 sizeType,
        o3d::SmartArrayFloat &out,
        GeometryProxy::MapType mapType,
        const o3d::SmartArrayDouble &data,
        const o3d::SmartArrayInt32 &indices,
        const o3d::SmartArrayInt32 &originalIndices)
{
    if (mapType == GeometryProxy::BY_POLYGON_VERTEX) {
        if (indices.isEmpty()) {
            out.allocate(data.getSize());

            // just convert from double to float
            for (o3d::UInt32 i = 0; i < data.getSize(); ++i) {
                out[i] = (o3d::Float)data[i];
            }
        } else {
            const o3d::UInt32 dataSize = data.getSize();
            out.allocate(indices.getSize() * sizeType);

            o3d::Float *outPtr = out.getData();
            o3d::Int32 p;

            if (sizeType == 2) {
                for (o3d::UInt32 i = 0; i < indices.getSize(); ++i) {
                    p = indices[i] * 2;

                    if (indices[i] < dataSize) {
                        *outPtr++ = (o3d::Float)data[p];
                        *outPtr++ = (o3d::Float)data[p+1];
                    } else {
                        *outPtr++ = 0;
                        *outPtr++ = 0;
                    }
                }
            } else if (sizeType == 3) {
                for (o3d::UInt32 i = 0; i < indices.getSize(); ++i) {
                    p = indices[i] * 3;

                    if (indices[i] < dataSize) {
                        *outPtr++ = (o3d::Float)data[p];
                        *outPtr++ = (o3d::Float)data[p+1];
                        *outPtr++ = (o3d::Float)data[p+2];
                    } else {
                        *outPtr++ = 0;
                        *outPtr++ = 0;
                        *outPtr++ = 0;
                    }
                }
            } else if (sizeType == 4) {
                for (o3d::UInt32 i = 0; i < indices.getSize(); ++i) {
                    if (indices[i] < dataSize) {
                        p = indices[i] * 4;

                        *outPtr++ = (o3d::Float)data[p];
                        *outPtr++ = (o3d::Float)data[p+1];
                        *outPtr++ = (o3d::Float)data[p+2];
                        *outPtr++ = (o3d::Float)data[p+3];
                    } else {
                        *outPtr++ = 0;
                        *outPtr++ = 0;
                        *outPtr++ = 0;
                        *outPtr++ = 0;
                    }
                }
            }
        }
    } else if (mapType == GeometryProxy::BY_VERTEX) {
        const o3d::UInt32 dataSize = data.getSize();
        o3d::Int32 idx;

        out.allocate(originalIndices.getSize() * sizeType);

        o3d::Float *outPtr = out.getData();
        o3d::Int32 p;

        if (sizeType == 2) {
            for (o3d::UInt32 i = 0; i < originalIndices.getSize(); ++i) {
                idx = originalIndices[i];
                if (idx < 0) {
                    idx = -idx - 1;
                }

                if (idx < dataSize) {
                    p = idx * 2;

                    *outPtr++ = (o3d::Float)data[p];
                    *outPtr++ = (o3d::Float)data[p+1];
                } else {
                    *outPtr++ = 0;
                    *outPtr++ = 0;
                }
            }
        } else if (sizeType == 3) {
            for (o3d::UInt32 i = 0; i < originalIndices.getSize(); ++i) {
                idx = originalIndices[i];
                if (idx < 0) {
                    idx = -idx - 1;
                }

                if (idx < dataSize) {
                    p = idx * 3;

                    *outPtr++ = (o3d::Float)data[p];
                    *outPtr++ = (o3d::Float)data[p+1];
                    *outPtr++ = (o3d::Float)data[p+2];
                } else {
                    *outPtr++ = 0;
                    *outPtr++ = 0;
                    *outPtr++ = 0;
                }
            }
        } else if (sizeType == 4) {
            for (o3d::UInt32 i = 0; i < originalIndices.getSize(); ++i) {
                idx = originalIndices[i];
                if (idx < 0) {
                    idx = -idx - 1;
                }

                if (idx < dataSize) {
                    p = idx * 4;

                    *outPtr++ = (o3d::Float)data[p];
                    *outPtr++ = (o3d::Float)data[p+1];
                    *outPtr++ = (o3d::Float)data[p+2];
                    *outPtr++ = (o3d::Float)data[p+3];
                } else {
                    *outPtr++ = 0;
                    *outPtr++ = 0;
                    *outPtr++ = 0;
                    *outPtr++ = 0;
                }
            }
        }
    } else {
        using o3d::Debug;
        using o3d::Logger;
        O3D_ERROR(o3d::E_InvalidFormat("Unsupported vertex data mapping type"));
    }
}

static void remapVertexData(
        const o3d::UInt32 sizeType,
        o3d::SmartArrayFloat &out,
        const o3d::ArrayInt32 &map)
{
    if (out.isEmpty()) {
        return;
    }

    o3d::SmartArrayFloat old(out);
    out = o3d::SmartArrayFloat(map.getSize() * sizeType);

    o3d::Int32 oldSize = old.getSize();

    o3d::Float *outPtr = out.getData();
    o3d::Int32 p;

    if (sizeType == 2) {
        for (o3d::UInt32 i = 0; i < map.getSize(); ++i) {
            if(map[i] < oldSize) {
                p = map[i] * 2;

                // uv
                // @todo is uv reverse needed ?
                *outPtr++ = old[p];
                *outPtr++ = old[p+1];
            } else {
                *outPtr++ = 0;
                *outPtr++ = 0;
            }
        }
    } else if (sizeType == 3) {
        for (o3d::UInt32 i = 0; i < map.getSize(); ++i) {
            if(map[i] < oldSize) {
                p = map[i] * 3;

                // vertex, normal, tangent
                // @todo if not Y+ up and Z- toward need conversion
                *outPtr++ = old[p];
                *outPtr++ = old[p+1];
                *outPtr++ = old[p+2];
            } else {
                *outPtr++ = 0;
                *outPtr++ = 0;
                *outPtr++ = 0;
            }
        }
    } else if (sizeType == 2) {
        for (o3d::UInt32 i = 0; i < map.getSize(); ++i) {
            if(map[i] < oldSize) {
                p = map[i] * 4;

                // color mostly
                *outPtr++ = old[p];
                *outPtr++ = old[p+1];
                *outPtr++ = old[p+2];
                *outPtr++ = old[p+3];
            } else {
                *outPtr++ = 0;
                *outPtr++ = 0;
                *outPtr++ = 0;
                *outPtr++ = 0;
            }
        }
    }
}
