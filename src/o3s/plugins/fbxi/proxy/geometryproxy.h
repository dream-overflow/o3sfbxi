/**
 * @brief FBX node geometry data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#ifndef _O3DS_FBXI_GEOMETRYPROXY_H
#define _O3DS_FBXI_GEOMETRYPROXY_H

#include "objectproxy.h"
#include <o3d/core/templatearray.h>

namespace o3d {
namespace studio {
namespace fbxi {

class GeometryProxy : public ObjectProxy
{
public:

    enum VertexData
    {
        DATA_VERTICES = 0,
        DATA_COLORS = 1,
        DATA_UVS = 2,
        DATA_NORMALS = 3,
        DATA_TANGENTS = 4,
        DATA_BITANGENTS = 5
    };

    enum MapType
    {
        BY_POLYGON_VERTEX,
        BY_POLYGON,
        BY_VERTEX
    };

    /**
     * @brief GeometryProxy
     * @param node Node named Camera
     */
    GeometryProxy(FBXNode *node);

    /**
     * @brief Process the geometry conversion and set array to
     * be accessed once processing is done.
     * @return True if success. Can raise exceptions.
     */
    Bool processGeometry();

    /**
     * @brief Access to a specific vertex data array once geometry has been processed.
     * @param Vertex array type.
     * @return Can be an empty array if not defined.
     */
    SmartArrayFloat vertexData(VertexData v);

    /**
     * @brief Global indices array.
     */
    SmartArrayUInt32 indices();

    /**
     * @brief Per vertex material id.
     */
    SmartArrayInt32 materials();

    /**
     * @brief As parsed edges (not processed).
     */
    SmartArrayInt32 edges();

    /**
     * @brief Number of materials if, corresponding to number of relative indices arrays.
     */
    UInt32 numMaterials() const;

    /**
     * @brief Indices relative array per material id.
     * @param id Material id.
     */
    SmartArrayInt32 indices(UInt32 id);

    struct Vertex
    {
        ~Vertex()
        {
            o3d::deletePtr(next);
        }

        Int32 index = -1;
        Vertex* next = nullptr;
    };

private:

    Bool readVertexData(FBXNode *node,
            const String& name,
            const String &indexName,
            SmartArrayDouble &data,
            SmartArrayInt32 &indices,
            MapType &mapType);

    void mergeVertexData();
    void mergeMaterials();

    SmartArrayFloat m_vertexData[DATA_BITANGENTS+1];
    SmartArrayInt32 m_materials;
    SmartArrayUInt32 m_indices;

    ArrayInt32 m_toOldVertices;

    TemplateArray<Vertex> m_toNewVertices;
    std::vector<SmartArrayInt32> m_perMaterialIndices;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_GEOMETRYPROXY_H
