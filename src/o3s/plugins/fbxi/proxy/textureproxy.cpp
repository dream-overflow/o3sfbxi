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
    ObjectProxy(node)
{
    if (!m_node || m_node->name() != "Texture") {
        O3D_ERROR(E_InvalidParameter("Must be a Texture node"));
    }

    FBXNode *version = m_node->child("Version");
    if (version) {
        m_version = version->directAsInt32();
    }

    if (m_version != 202) {
        O3D_ERROR(E_InvalidParameter("Must be a Model node version 202"));
    }

    // TextureName
    // Properties70
    // Media
    // ModelUVTranslation
    // ModelUVScaling
    // Texture_Alpha_Source
    // Cropping

    m_objectType = OBJECT_TEXTURE;
}

o3d::String TextureProxy::relativeFilename()
{
    String filename;

    FBXNode *relativeFilenameNode = m_node->child("RelativeFilename");
    if (relativeFilenameNode) {
        filename = relativeFilenameNode->directAsString();
    }

    return filename;
}

o3d::String TextureProxy::filename()
{
    String filename;

    FBXNode *fileNameNode = m_node->child("FileName");
    if (fileNameNode) {
        filename = fileNameNode->directAsString();
    }

    return filename;
}
