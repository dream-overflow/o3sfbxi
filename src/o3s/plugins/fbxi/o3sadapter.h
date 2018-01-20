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

#include <o3d/studio/common/global.h>
#include <o3d/studio/common/importer/importer.h>
#include <o3d/studio/common/importer/importdefinition.h>
#include <o3d/studio/common/workspace/asset.h>
#include <o3d/studio/common/workspace/hub.h>

namespace o3d {
namespace studio {
namespace fbxi {

class O3SAdapter : public Adapter
{
public:

    O3SAdapter(common::ImporterOption *options, common::Entity *parent);
    virtual ~O3SAdapter();

    virtual Bool processImport(std::list<FBXNode*> &rootNodes) override;

private:

    common::ImporterOption *m_options;
    common::Entity *m_parent;

    void setupAsset(common::Asset* asset);
    void setupHub(common::Hub* rootHub);
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_O3SADAPTER_H
