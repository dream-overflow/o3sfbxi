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
    Proxy(node)
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
}

o3d::SmartArrayDouble GeometryProxy::vertices()
{
    SmartArrayDouble array;

    FBXNode *vertices = m_node->child("Vertices");
    if (vertices) {
        PropertyFloat64Array *data = static_cast<PropertyFloat64Array*>(vertices->property(0));
        array = data->value();
    }

    return array;
}

o3d::SmartArrayDouble GeometryProxy::normals()
{
    SmartArrayDouble array;

//    [INFO] subnode Geometry: LayerElementNormal
//    [INFO] subnode LayerElementNormal: Version
//    [INFO] subnode LayerElementNormal: Name
//    [INFO] subnode LayerElementNormal: MappingInformationType
//    [INFO] subnode LayerElementNormal: ReferenceInformationType
//    [INFO] subnode LayerElementNormal: Normals

    return array;
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

