/**
 * @brief FBX importer adapter to O3S implementation
 * @copyright Copyright (C) 2018 Dream Overflow. All rights reserved.
 * @author Frederic SCHERMA (frederic.scherma@dreamoverflow.org)
 * @date 2018-01-20
 * @details
 */

#include "o3sadapter.h"
#include "loader.h"

#include "proxy/headerproxy.h"
#include "proxy/definitionsproxy.h"
#include "proxy/objectsproxy.h"
#include "proxy/modelproxy.h"
#include "proxy/relationsproxy.h"
#include "proxy/globalsettingsproxy.h"
#include "proxy/connectionsproxy.h"
// #include "proxy/takesproxy.h"
// #include "proxy/takeproxy.h"

#include "proxy/modelproxy.h"
#include "proxy/materialproxy.h"
#include "proxy/cameraproxy.h"
#include "proxy/lightproxy.h"
#include "proxy/textureproxy.h"
#include "proxy/nodeattributeproxy.h"
#include "proxy/geometryproxy.h"
#include "proxy/boneproxy.h"
#include "proxy/meshproxy.h"

#include "property/propertyint32.h"
#include "property/propertyint64.h"
#include "property/propertystring.h"

#include <o3d/studio/common/objectref.h>
#include <o3d/studio/common/application.h>

#include <o3d/studio/common/component/componentregistry.h>
#include <o3d/studio/common/component/spacialnodehub.h>
#include <o3d/studio/common/component/meshhub.h>
#include <o3d/studio/common/component/camerahub.h>
// #include <o3d/studio/common/component/lighthub.h>
// #include <o3d/studio/common/component/materialhub.h>
// #include <o3d/studio/common/component/texturehub.h>
// #include <o3d/studio/common/component/skeletonhub.h>
// #include <o3d/studio/common/component/animationlayerhub.h>
// #include <o3d/studio/common/component/animationtrackhub.h>
// #include <o3d/studio/common/component/animationnodehub.h>

#include <o3d/studio/common/workspace/project.h>
#include <o3d/studio/common/objectref.h>
#include <o3d/studio/common/component/dummyhub.h>

using namespace o3d::studio::fbxi;

O3SAdapter::O3SAdapter(Parser *parser,
        o3d::studio::common::ImporterOption *options,
        common::Hub *parent,
        FbxImportDefinition* def) :
    m_parser(parser),
    m_loader(nullptr),
    m_options(options),
    m_parent(parent),
    m_def(def),
    m_upAxis(0, 1, 0),
    m_frontAxis(0, 0, 1),
    m_unitScale(1.0)
{
    O3D_ASSERT(parser);
    O3D_ASSERT(options);
    O3D_ASSERT(parent);
    O3D_ASSERT(def);
}

O3SAdapter::~O3SAdapter()
{
    deletePtr(m_loader);
}

o3d::Bool O3SAdapter::processImport()
{
    common::Hub *topLevelHub = nullptr;

    if (m_parent->ref().light().baseTypeOf(common::TypeRef::hub())) {
        topLevelHub = static_cast<common::Hub*>(m_parent);
    }

    if (m_loader) {
        delete m_loader;
    }

    m_loader = new Loader(m_parser, m_def);
    m_loader->load();

    toScene();

    return True;
}

o3d::Bool O3SAdapter::toScene()
{
    // @todo from loader
    // setup model node to editor
    // @todo remove hub instances above
    ObjectProxy *currentProxy = m_loader->objectProxy(0);
    std::list<size_t> cursor;
    cursor.push_back(0);

    common::Component *spacialNodeComponent = common::Application::instance()->components().component("o3s::common::component::spacialhub");
    common::Component *cameraComponent = common::Application::instance()->components().component("o3s::common::component::camerahub");
    // common::Component *lightComponent = common::Application::instance()->components().component("o3s::common::component::lighthub");
    common::Component *meshComponent = common::Application::instance()->components().component("o3s::common::component::meshhub");

    common::Project* project = m_parent->project();
    common::Hub* rootHub = m_parent;

    common::Hub *parentHub;
    common::Entity *parentEntity;

    // temporary map to retrieve parent hub
    std::map<ObjectProxy*, common::Hub*> m_proxyToHub;
    m_proxyToHub[currentProxy] = rootHub;

    // @todo mesh geometry, skin, skeleton, material, texture

    while (currentProxy != nullptr) {
        System::print(currentProxy->name(), currentProxy->subClass() + String(" uid={0}").arg(currentProxy->uid()));

        // retrieve the parent hub
        if (currentProxy->parent()) {
            parentHub = m_proxyToHub[currentProxy->parent()];
            parentEntity = parentHub;
        } else {
            parentHub = nullptr;
            parentEntity = project;
        }

        if (currentProxy->objectType() == ObjectProxy::OBJECT_NULL_NODE) {
            // @todo is it processed ? what for hier of model ?
        } else if (currentProxy->objectType() == ObjectProxy::OBJECT_MESH_MODEL) {
            MeshModelProxy *proxy = static_cast<MeshModelProxy*>(currentProxy);
            common::SpacialNodeHub *spacialHub = static_cast<common::SpacialNodeHub*>(spacialNodeComponent->buildHub(
                "Node " + proxy->name(), project, parentEntity));

            spacialHub->setRef(common::ObjectRef::buildRef(project, spacialHub->typeRef()));

            // transform
            spacialHub->setPosition(0, proxy->position());
            spacialHub->setRotation(0, proxy->rotation());
            spacialHub->setScale(0, proxy->scale());

            if (parentHub) {
                parentHub->addHub(spacialHub);
            }

            // store for lookup
            m_proxyToHub[proxy] = spacialHub;

            common::MeshHub *meshHub = static_cast<common::MeshHub*>(meshComponent->buildHub(
                proxy->name(), project, spacialHub));

            meshHub->setRef(common::ObjectRef::buildRef(project, meshHub->typeRef()));

            // @todo geometry

            spacialHub->addHub(meshHub);
        } else if (currentProxy->objectType() == ObjectProxy::OBJECT_CAMERA_MODEL) {
            CameraModelProxy *proxy = static_cast<CameraModelProxy*>(currentProxy);
            common::SpacialNodeHub *spacialHub = static_cast<common::SpacialNodeHub*>(spacialNodeComponent->buildHub(
                "Node " + proxy->name(), project, parentEntity));

            spacialHub->setRef(common::ObjectRef::buildRef(project, spacialHub->typeRef()));

            // transform
            spacialHub->setPosition(0, proxy->position());
            spacialHub->setRotation(0, proxy->rotation());
            spacialHub->setScale(0, proxy->scale());

            if (parentHub) {
                parentHub->addHub(spacialHub);
            }

            // store for lookup
            m_proxyToHub[proxy] = spacialHub;

            common::CameraHub *cameraHub = static_cast<common::CameraHub*>(cameraComponent->buildHub(
                proxy->name(), project, spacialHub));

            cameraHub->setRef(common::ObjectRef::buildRef(project, cameraHub->typeRef()));

            // @todo camera settings

            spacialHub->addHub(cameraHub);
        } else if (currentProxy->objectType() == ObjectProxy::OBJECT_LIGHT_MODEL) {
            // @todo need light hub
        } else if (currentProxy->objectType() == ObjectProxy::OBJECT_SKIN) {
            // @todo need skin proxy and skin hub
        }

        currentProxy = currentProxy->recursiveNext(cursor);
    }

    m_proxyToHub.clear();

    return True;
}

o3d::studio::common::Hub *O3SAdapter::hub(o3d::Int64 uid)
{
    auto it = m_hubs.find(uid);
    if (it != m_hubs.end()) {
        return it->second;
    }

    return nullptr;
}

//                case ObjectProxy::OBJECT_LIMB_NODE_MODEL:
//                {
//                    BoneModelProxy *lp = objects->boneModel(i);
//                    m_objects[lp->uid()] = lp;

//                    // @todo uses a Bone component
//                    common::Component *component = common::Application::instance()->components().component("o3s::common::component::spacialhub");
//                    common::SpacialNodeHub *hub = static_cast<common::SpacialNodeHub*>(component->buildHub("Bone " + lp->name(), project, project));

//                    hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                    hub->setProject(project);

//                    // parent will be know during connections
//                    m_hubs[lp->uid()] = hub;
//                }
//                    break;
//                case ObjectProxy::OBJECT_TEXTURE:
//                {
//                    TextureProxy *tp = objects->texture(i);
//                    m_objects[tp->uid()] = tp;

//                    // @todo add a TextureResource/hub ?

//                    // hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                    // hub->setProject(project);

//                    // @todo
//                }
//                    break;
//                default:
//                    break;
//            }
//        }
