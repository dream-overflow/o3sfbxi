/**
 * @brief FBX importer adapter to O3S implementation
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_O3SADAPTER_H
#define _O3DS_FBXI_O3SADAPTER_H

#include "adapter.h"
#include "parser.h"

#include "importer.h"

#include <o3d/studio/common/workspace/asset.h>
#include <o3d/studio/common/workspace/hub.h>

#include <o3d/core/vector3.h>

namespace o3d {
namespace studio {
namespace fbxi {

class O3S_PLUGIN_API O3SAdapter : public Adapter
{
public:

    O3SAdapter(
            Parser *parser,
            common::ImporterOption *options,
            common::Entity *parent,
            FbxImportDefinition* def);

    virtual ~O3SAdapter();

    virtual Bool processImport() override;
    virtual Bool processImportLazy() override;

private:

    Parser *m_parser;

    common::ImporterOption *m_options;
    common::Entity *m_parent;

    FbxImportDefinition *m_def;

    void setupDef();
    void setupAsset(common::Asset* asset);
    void setupHub(common::Hub* rootHub);

    Vector3 m_upAxis;
    Vector3 m_frontAxis;
    Float m_unitScale;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_O3SADAPTER_H
