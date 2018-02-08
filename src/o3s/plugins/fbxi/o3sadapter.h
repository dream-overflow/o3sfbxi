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
#include "loader.h"

#include "importer.h"

#include <o3d/studio/common/workspace/asset.h>
#include <o3d/studio/common/workspace/hub.h>

#include <o3d/core/vector3.h>
#include <o3d/core/stringmap.h>

namespace o3d {
namespace studio {
namespace fbxi {

class ObjectProxy;

class O3S_PLUGIN_API O3SAdapter : public Adapter
{
public:

    O3SAdapter(
            Parser *parser,
            common::ImporterOption *options,
            common::Hub *parent,
            FbxImportDefinition* def);

    virtual ~O3SAdapter();

    virtual Bool processImport() override;
    virtual Bool toScene() override;

    common::Hub* hub(Int64 uid);

private:

    Parser *m_parser;
    Loader *m_loader;

    common::ImporterOption *m_options;
    common::Hub *m_parent;

    std::map<Int64, common::Hub*> m_hubs;

    FbxImportDefinition *m_def;

    Vector3 m_upAxis;
    Vector3 m_frontAxis;
    Float m_unitScale;

    Int64 m_startTime;   //!< unit of 1/46186158000 seconds
    Int64 m_endTime;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_O3SADAPTER_H
