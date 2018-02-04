/**
 * @brief FBX data proxy base abstract class
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#ifndef _O3DS_FBXI_PROXY_H
#define _O3DS_FBXI_PROXY_H

#include "../fbxnode.h"

namespace o3d {
namespace studio {
namespace fbxi {

class Proxy
{
public:

    Proxy(FBXNode *node);
    virtual ~Proxy();

    virtual Int32 version();

    FBXNode *node();
    const FBXNode *node() const;

protected:

    FBXNode *m_node;
    Int32 m_version;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PROXY_H
