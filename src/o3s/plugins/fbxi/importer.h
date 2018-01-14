/**
 * @brief FBX importer controller
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-14
 * @details
 */

#ifndef _O3DS_FBXI_IMPORTER_H
#define _O3DS_FBXI_IMPORTER_H

#include <o3d/core/instream.h>
#include <o3d/studio/common/importer/importer.h>

namespace o3d {
namespace studio {
namespace fbxi {

class Importer : public common::Importer
{
public:

    Importer();
    virtual ~Importer();

    virtual common::ImporterOption* buildOptions() override;
    virtual Bool introspect(const String &filename) override;
    virtual Bool import(const String &filename, common::ImporterOption *options, common::Entity *parent) override;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_IMPORTER_H
