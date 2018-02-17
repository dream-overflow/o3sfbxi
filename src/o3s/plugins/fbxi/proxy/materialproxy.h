/**
 * @brief FBX node material data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#ifndef _O3DS_FBXI_MATERIALPROXY_H
#define _O3DS_FBXI_MATERIALPROXY_H

#include "objectproxy.h"

namespace o3d {
namespace studio {
namespace fbxi {

class TextureProxy;

class MaterialProxy : public ObjectProxy
{
public:

    enum MapType
    {
        MAP_AMBIENT = 0,
        MAP_DIFFUSE,
        MAP_EMISSIVE,
        MAP_SPECULAR,
        MAP_NORMAL
    };

    /**
     * @brief MaterialProxy
     * @param node Node named material
     */
    MaterialProxy(FBXNode *node);

    void setTexture(MapType map, TextureProxy *texture);
    TextureProxy* texture(MapType map);

    Bool hasTexture(MapType map) const;

protected:

    TextureProxy *m_textures[MAP_NORMAL+1];
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_MATERIALPROXY_H
