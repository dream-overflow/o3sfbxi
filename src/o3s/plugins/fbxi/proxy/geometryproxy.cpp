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

o3d::SmartArrayDouble GeometryProxy::vertices()
{
    SmartArrayDouble array;

    FBXNode *vertices = m_node->child("Vertices");
    if (vertices) {
        PropertyFloat64Array *data = static_cast<PropertyFloat64Array*>(vertices->property(0));
        array = data->value();
    }

    // @todo process

    return array;
}

o3d::SmartArrayFloat GeometryProxy::normals()
{
    SmartArrayFloat normals;

    SmartArrayDouble data;
    SmartArrayInt32 indices;

    FBXNode *layerElementNormal = m_node->child("LayerElementNormal");
    if (!layerElementNormal) {
        return normals;
    }

    MapType mapType;
    vertexData(layerElementNormal,
               "Normals",
               "NormalsIndex",
               data,
               indices,
               mapType);

    // @todo process normals

    return normals;
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

o3d::SmartArrayDouble GeometryProxy::uvs()
{
    SmartArrayDouble array;

    // @todo process

    return array;
}

o3d::SmartArrayInt32 GeometryProxy::indexes()
{
    SmartArrayInt32 array;

    FBXNode *indexes = m_node->child("PolygonVertexIndex");
    if (indexes) {
        PropertyInt32Array *data = static_cast<PropertyInt32Array*>(indexes->property(0));
        array = data->value();
    }

    return array;
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
    // @todo
    return True;
}

o3d::Bool GeometryProxy::vertexData(
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
