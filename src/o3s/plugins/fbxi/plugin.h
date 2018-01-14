/**
 * @brief FBX import import module object
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#ifndef _O3DS_FBXI_PLUGIN_H
#define _O3DS_FBXI_PLUGIN_H

#include <o3d/studio/common/dynamicmodule.h>

namespace o3d {
namespace studio {
namespace fbxi {

class O3S_PLUGIN_API Plugin : public studio::common::DynamicModule
{
public:

    Plugin(const String &name, DynamicLibrary *library);
    virtual ~Plugin();

    virtual Bool start() override;
    virtual Bool stop() override;

    virtual common::Importer* importer() override;

private:

    class Importer *m_importer;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_PLUGIN_H
