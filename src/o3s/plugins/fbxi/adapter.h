/**
 * @brief FBX importer adapter interface
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_ADAPTER_H
#define _O3DS_FBXI_ADAPTER_H

#include <list>
#include "fbxnode.h"

namespace o3d {
namespace studio {
namespace fbxi {

class O3S_PLUGIN_API Adapter
{
public:

    /**
     * @brief Do here your import processing using the structure of FBXNode.
     * @return True if success.
     */
    virtual Bool processImport() = 0;

    /**
     * @brief Do here your import processing using the structure of FBXNode.
     * @return True if success.
     */
    virtual Bool processImportLazy() = 0;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_ADAPTER_H
