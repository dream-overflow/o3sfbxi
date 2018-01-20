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

class Adapter
{
public:

    /**
     * @brief Do here your import processing using the structure of FBXNode.
     * @param rootNodes First level of FBX nodes.
     * @return True if success.
     */
    virtual Bool processImport(std::list<FBXNode*> &rootNodes) = 0;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_ADAPTER_H
