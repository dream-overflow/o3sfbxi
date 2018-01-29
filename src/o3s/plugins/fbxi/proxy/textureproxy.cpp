/**
 * @brief FBX texture data proxy
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-29
 * @details
 */

#include "textureproxy.h"
#include <o3d/core/debug.h>

#include "../property/propertystring.h"
#include "../property/propertyint32.h"
#include "../property/propertyfloat32.h"
#include "../property/propertyfloat64.h"

using namespace o3d::studio::fbxi;

TextureProxy::TextureProxy(FBXNode *node) :
    Proxy(node)
{
    if (!m_node || m_node->name() != "Texture") {
        O3D_ERROR(E_InvalidParameter("Must be a Texture node"));
    }

    // @todo check Version == 100
}
