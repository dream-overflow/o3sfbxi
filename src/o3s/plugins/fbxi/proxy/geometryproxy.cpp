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
        o3d::ArrayUInt32 &indices,
        o3d::ArrayUInt32 &toOld);

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

//
//[INFO] subnode Geometry: LayerElementMaterial
//[INFO] subnode LayerElementMaterial: Version
//[INFO] subnode LayerElementMaterial: Name
//[INFO] subnode LayerElementMaterial: MappingInformationType
//[INFO] subnode LayerElementMaterial: ReferenceInformationType
//[INFO] subnode LayerElementMaterial: Materials

// usefull ??
//[INFO] subnodeGeometry: Layer
//[INFO] subnodeLayer: Version
//[INFO] subnodeLayer: LayerElement
//[INFO] subnodeLayerElement: Type
//[INFO] subnodeLayerElement: TypedIndex
//[INFO] subnodeLayer: LayerElement
//[INFO] subnodeLayerElement: Type
//[INFO] subnodeLayerElement: TypedIndex

o3d::SmartArrayInt32 GeometryProxy::indices()
{
    return m_indices;
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

    SmartArrayInt32 indices;

    FBXNode *polygonVertexIndex = m_node->child("PolygonVertexIndex");
    if (polygonVertexIndex) {
        PropertyInt32Array *data = static_cast<PropertyInt32Array*>(polygonVertexIndex->property(0));
        indices = data->value();
    } else {
        O3D_ERROR(E_InvalidFormat("Missing vertices indices array"));
    }

    // triangulate
    ArrayUInt32 toOldIndices;
    triangulate(indices, m_toOldVertices, toOldIndices);

    m_vertexData[DATA_VERTICES].allocate(m_toOldVertices.getSize());

    for (UInt32 i = 0; i < m_toOldVertices.getSize(); ++i) {
        m_vertexData[DATA_VERTICES][i] = vertices[m_toOldVertices[i]];
    }

    FBXNode *layerElementMaterial = m_node->child("LayerElementMaterial");
    if (layerElementMaterial) {
        FBXNode *version = layerElementMaterial->child("Version");
        if (!version || version->directAsInt32() != 232) {
            O3D_ERROR(E_InvalidFormat(String("Must be a {0} node version 232").arg(layerElementMaterial->name())));
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

            // @todo
        } else {
            if (mapTypeName != "AllSame") {
                O3D_ERROR(E_InvalidFormat(String("Mapping of Materials data is not supported")));
            }
        }
    }

    // uvs
    FBXNode *layerElementUV = m_node->child("LayerElementUV");
    if (layerElementUV) {
        SmartArrayDouble uvs;
        SmartArrayInt32 uvsIndices;
        MapType uvsMapType;

        readVertexData(layerElementUV,
               "UV",
               "UVIndex",
               uvs,
               uvsIndices,
               uvsMapType);

        if (!uvs.isEmpty()) {
            // @todo
        }
    }

    // tangents
    FBXNode *layerElementTangents = m_node->child("LayerElementTangents");
    if (layerElementTangents) {
        SmartArrayDouble tangents;
        SmartArrayInt32 tangetsIndices;
        MapType tangentsMapType;

        if (layerElementTangents->child("Tangent")) {
            readVertexData(layerElementTangents,
                           "Tangent",
                           "TangentIndex",
                           tangents,
                           tangetsIndices,
                           tangentsMapType);
        } else if (layerElementTangents->child("Tangents")) {
            readVertexData(layerElementTangents,
                           "Tangents",
                           "TangentsIndex",
                           tangents,
                           tangetsIndices,
                           tangentsMapType);
        }

        if (!tangents.isEmpty()) {
            // @todo
        }
    }

    // colors
    FBXNode *layerElementColor = m_node->child("LayerElementColor");
    if (layerElementColor) {
        SmartArrayDouble colors;
        SmartArrayInt32 colorsIndices;
        MapType colorsMapType;

        readVertexData(layerElementColor,
               "Color",
               "ColorIndex",
               colors,
               colorsIndices,
               colorsMapType);

        if (!colors.isEmpty()) {
            // @todo
        }
    }

    // normals
    FBXNode *layerElementNormal = m_node->child("LayerElementNormal");
    if (layerElementNormal) {
        SmartArrayDouble normals;
        SmartArrayInt32 normalIndices;
        MapType normalMapType;

        readVertexData(layerElementNormal,
               "Normals",
               "NormalsIndex",
               normals,
               normalIndices,
               normalMapType);

        if (!normals.isEmpty()) {
            // @todo
        }
    }

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
    if (!version || version->directAsInt32() != 232) {
        O3D_ERROR(E_InvalidFormat(String("Must be a {0} node version 232").arg(node->name())));
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

static void triangulate(
        const o3d::SmartArrayInt32 oldIndices,
        o3d::ArrayUInt32 &indices,
        o3d::ArrayUInt32 &toOld)
{
    auto getIdx = [&oldIndices](o3d::Int32 i) -> o3d::Int32 {
        o3d::Int32 idx = oldIndices[i];
        return idx < 0 ? -idx - 1 : idx;
    };

    o3d::Int32 inPolygonIdx = 0;
    for (o3d::UInt32 i = 0; i < oldIndices.getSize(); ++i) {
        int idx = getIdx(i);

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
