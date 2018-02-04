/**
 * @brief FBX node texture data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#ifndef _O3DS_FBXI_TEXTUREPROXY_H
#define _O3DS_FBXI_TEXTUREPROXY_H

#include "objectproxy.h"
#include <o3d/image/color.h>

namespace o3d {
namespace studio {
namespace fbxi {

class TextureProxy : public ObjectProxy
{
public:

    /**
     * @brief TextureProxy
     * @param node Node named Texture
     */
    TextureProxy(FBXNode *node);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_TEXTUREPROXY_H
