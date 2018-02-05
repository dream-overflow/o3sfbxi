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
#include <o3d/image/color.h>
#include <o3d/studio/common/global.h>
#include <o3d/studio/common/importer/importer.h>
#include <o3d/studio/common/importer/importdefinition.h>

namespace o3d {
namespace studio {
namespace fbxi {

/**
 * @brief The FbxImportDefinition class
 * @todo more detailed objects list (name, info per type...)
 */
class O3S_PLUGIN_API FbxImportDefinition : public common::ImportDefinition
{
    friend class Loader;

public:

    virtual ~FbxImportDefinition();

    virtual String creator() const override;
    virtual DateTime creationDateTime() const override;
    virtual Float unit() const override;
    virtual Color ambientColor() const override;

    virtual UInt32 numModel() const override;
    virtual UInt32 numGeometry() const override;
    virtual UInt32 numMaterial() const override;
    virtual UInt32 numCamera() const override;
    virtual UInt32 numLight() const override;

private:

    String m_creator;
    DateTime m_creationTimestamp;
    Float m_unit;
    Color m_ambientColor;

    UInt32 m_numModel;
    UInt32 m_numGeometry;
    UInt32 m_numMaterial;
    UInt32 m_numCamera;
    UInt32 m_numLight;
};

/**
 * @brief The Importer class specialisation for FBX.
 */
class O3S_PLUGIN_API Importer : public common::Importer
{
public:

    Importer();
    virtual ~Importer();

    virtual common::ImporterOption* buildOptions() override;
    virtual common::ImportDefinition* introspect(const String &filename) override;
    virtual common::ImportDefinition* import(
            const String &filename,
            common::ImporterOption *options,
            common::Entity *parent) override;
};

} // namespace fbxi
} // namespace studio
} // namespace o3d

#endif // _O3DS_FBXI_IMPORTER_H
