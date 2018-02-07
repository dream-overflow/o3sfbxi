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
        o3d::studio::common::Entity *parent,
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

//    // delete non assigned hubs (deletion is recursive)
//    for (auto it = m_hubs.begin(); it != m_hubs.end(); ++it) {
//        common::Hub *hub = it->second;
//        if (!hub->parent()) {
//            delete hub;
//        } else {
//            if (hub->parent()->ref().light().baseTypeOf(common::TypeRef::project())) {
//                common::Project *project = static_cast<common::Project*>(hub->parent());
//                if (!project->hub(hub->ref().light())) {
//                    delete hub;
//                }
//            } else if (hub->parent()->ref().light().baseTypeOf(common::TypeRef::hub())) {
//                common::Hub *parentHub = static_cast<common::Hub*>(hub->parent());
//                if (!parentHub->hub(hub->ref().light())) {
//                    delete hub;
//                }
//            }
//        }
//    }
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
    common::Hub* rootHub = nullptr;

    common::Hub *parentHub;
    common::Entity *parentEntity;

    if (m_parent->ref().light().baseTypeOf(common::TypeRef::hub())) {
        rootHub = static_cast<common::Hub*>(m_parent);
    }

    // temporary map to retrieve parent hub
    std::map<ObjectProxy*, common::Hub*> m_proxyToHub;
    m_proxyToHub[currentProxy] = rootHub;

    // parent hub @todo
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

            // @todo transform

            if (parentHub) {
                parentHub->addHub(spacialHub);
            } else {
                project->addHub(spacialHub);
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

            // @todo transform

            if (parentHub) {
                parentHub->addHub(spacialHub);
            } else {
                project->addHub(spacialHub);
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

//void O3SAdapter::setupDef(common::Hub *topLevelHub)
//{
//    common::Project *project = m_parent->project();

//    // global informations about this document
//    FBXNode *node = m_parser->child("FBXHeaderExtension");
//    if (node) {
//        HeaderProxy *hp = new HeaderProxy(node);
//        m_def->m_creator = hp->creator();
//        m_def->m_creationTimestamp = hp->creationTimeStamp();

//        delete hp;
//    }

//    // global settings of the data format (for transform, and details)
//    node = m_parser->child("GlobalSettings");
//    if (node) {
//        GlobalSettingsProxy *gs = new GlobalSettingsProxy(node);
//        m_unitScale = gs->unitScale();
//        m_upAxis = gs->upAxis();
//        m_frontAxis = gs->frontAxis();
//        m_startTime = gs->timeSpanStart();
//        m_endTime = gs->timeSpanEnd();

//        m_def->m_ambientColor = gs->ambientColor();

//        delete gs;
//    }

//    // definitions of the templates
//    node = m_parser->child("Definitions");
//    if (node) {
//        DefinitionsProxy *definitions = new DefinitionsProxy(node);

//        // @todo TemplateProxy ... apply template to objects after ...
//        Int32 count = definitions->count();

//        delete definitions;
//    }

//    node = m_parser->child("Objects");
//    if (node) {
//        ObjectsProxy *objects = new ObjectsProxy(node);

//        for (UInt32 i = 0; i < objects->numObjects(); ++i) {
//            switch (objects->objectType(i)) {
//                case ObjectProxy::OBJECT_CAMERA_MODEL:
//                {
//                    // not very interesting
//                    if (0) {
//                        CameraModelProxy *cp = objects->cameraModel(i);
//                        m_objects[cp->uid()] = cp;

//                        common::Component *component = common::Application::instance()->components().component("o3s::common::component::spacialhub");
//                        common::SpacialNodeHub *hub = static_cast<common::SpacialNodeHub*>(component->buildHub(cp->name(), project, project));

//                        hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                        hub->setProject(project);

//                        m_hubs[cp->uid()] = hub;
//                    }
//                }
//                    break;
//                case ObjectProxy::OBJECT_CAMERA_NODE_ATTR:
//                {
//                    // not very interesting
//                    if (0) {
//                        CameraNodeProxy *cp = objects->cameraNode(i);
//                        m_objects[cp->uid()] = cp;

//                        common::Component *component = common::Application::instance()->components().component("o3s::common::component::camerahub");
//                        common::CameraHub *hub = static_cast<common::CameraHub*>(component->buildHub(cp->name(), project, project));

//                        hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                        hub->setProject(project);

//                        // @todo ortho, fov...

//                        m_hubs[cp->uid()] = hub;
//                    }
//                }
//                    break;
//                case ObjectProxy::OBJECT_GEOMETRY:
//                {
//                    GeometryProxy *gp = objects->geometry(i);
//                    m_objects[gp->uid()] = gp;
//               //     common::MeshHub *hub = new common::MeshHub(gp->name());

//                    // hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                    // hub->setProject(project);

//                    // @todo explode vertices, uvs...
//                    // hub->setVertices(gp->vertices());

//                    // probably used by a model node
//                //    m_hubs[gp->uuid()] = hub;
//                }
//                    break;
//                case ObjectProxy::OBJECT_LIGHT_NODE_ATTR:
//                {
//                    // not very interesting
//                    if (0) {
//                        LightNodeProxy *lp = objects->lightNode(i);
//                        // common::LightHub *hub = new common::LightHub(mp->name());

//                        // hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                        // hub->setProject(project);

//                        // @todo ambient, diffuse, specular, type, size...

//                        // m_hubs[mp->typeName()] = hub;
//                        delete lp;
//                    }
//                }
//                    break;
//                case ObjectProxy::OBJECT_MATERIAL:
//                {
//                    MaterialProxy *mp = objects->material(i);
//                    m_objects[mp->uid()] = mp;

////                    common::MaterialHub *hub = new common::MaterialHub(mp->name());

////                    hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                    // hub->setProject(project);

////                    // @todo ambient, diffuse, specular, transparency...

////                    m_hubs[mp->uuid()] = hub;
//                }
//                    break;
//                case ObjectProxy::OBJECT_MODEL:
//                {
//                    // unspecialized are "Null" and "Root" or unsupported
//                    ModelProxy *mp = objects->model(i);
//                    m_objects[mp->uid()] = mp;

//                    common::Component *component = common::Application::instance()->components().component("o3s::common::component::spacialhub");
//                    common::SpacialNodeHub *hub = static_cast<common::SpacialNodeHub*>(component->buildHub("Node " + mp->name(), project, project));

//                    hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                    hub->setProject(project);

//                    hub->setPosition(0, mp->position());
//                    hub->setRotation(0, mp->rotation());
//                    hub->setScale(0, mp->scale());

//                    // parent will be know during connections
//                    m_hubs[mp->uid()] = hub;
//                }
//                    break;
//                case ObjectProxy::OBJECT_NODE_ATTRIBUTE:
//                {
//                    // unspecialized are unsupported
//                    NodeAttributeProxy *np = objects->nodeAttribute(i);
//                    m_objects[np->uid()] = np;

//                    common::Component *component = common::Application::instance()->components().component("o3s::common::component::spacialhub");
//                    common::SpacialNodeHub *hub = static_cast<common::SpacialNodeHub*>(component->buildHub("Attr" + np->name(), project, project));

//                    hub->setRef(common::ObjectRef::buildRef(project, hub->typeRef()));
//                    hub->setProject(project);

//                    // parent will be know during connections
//                    m_hubs[np->uid()] = hub;
//                }
//                    break;
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
//                case ObjectProxy::OBJECT_MESH_MODEL:
//                {
//                    MeshModelProxy *mp = objects->meshModel(i);
//                    m_objects[mp->uid()] = mp;

//                    // a spacial node hub
//                    common::Component *snComponent = common::Application::instance()->components().component("o3s::common::component::spacialhub");
//                    common::SpacialNodeHub *snHub = static_cast<common::SpacialNodeHub*>(snComponent->buildHub("Node " + mp->name(), project, project));

//                    snHub->setRef(common::ObjectRef::buildRef(project, snHub->typeRef()));
//                    snHub->setProject(project);

//                    m_hubs[mp->uid()] = snHub;

//                    // and a mesh hub
//                    common::Component *mComponent = common::Application::instance()->components().component("o3s::common::component::meshhub");
//                    common::MeshHub *mHub = static_cast<common::MeshHub*>(mComponent->buildHub(mp->name(), project, snHub));

//                    mHub->setRef(common::ObjectRef::buildRef(project, mHub->typeRef()));
//                    mHub->setProject(project);

//                    snHub->addHub(mHub);
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

//        delete objects;
//    }

//    // add a root proxy at uid 0
//    FBXNode *m_rootNode = new FBXNode("Model");
//    m_rootNode->addProperty(new PropertyInt64(0));
//    m_rootNode->addProperty(new PropertyString("Root::Root"));
//    m_rootNode->addProperty(new PropertyString("Root"));

//    FBXNode *rootNodeVersion = new FBXNode("Version");
//    rootNodeVersion->addProperty(new PropertyInt32(232));
//    m_rootNode->addChild(rootNodeVersion);

//    ModelProxy *rootProxy = new ModelProxy(m_rootNode);
//    m_objects[0] = rootProxy;

//    m_def->m_unit = m_unitScale;

//    node = m_parser->child("Connections");
//    if (node) {
//        ConnectionsProxy *connections = new ConnectionsProxy(node);

//        // setup connections between objects
//        for (UInt32 i = 0; i < connections->numConnections(); ++i) {
//            if (connections->connectionType(i) == ConnectionsProxy::CONN_OO) {
//                Int64 parentUid, childUid;
//                connections->objectRelation(i, parentUid, childUid);

//                common::Hub* parentHub = hub(parentUid);
//                common::Hub* childHub = hub(childUid);

//                ObjectProxy* parentProxy = objectProxy(parentUid);
//                ObjectProxy* childProxy = objectProxy(childUid);

//                // parent proxy not found
//                if (parentUid != 0 && !parentProxy) {
//                    continue;
//                }

//                // parent hub not found
//                if (parentUid != 0 && !parentHub) {
//                    continue;
//                }

//                // not imported children ignore the connection
//                if (!childHub || !childProxy) {
//                    continue;
//                }

//                ObjectProxy::ObjectType childType = childProxy->objectType();
//                // @todo subclasses
//                if (childType == ObjectProxy::OBJECT_NODE_ATTRIBUTE) {
//                    // must be assigned to only one parent model
//                    if (childHub->parent() != project) {
//                        O3D_ERROR(E_InvalidPrecondition("Node attribute is already assigned to a parent"));
//                    }
//                }

//                ObjectProxy::ObjectType parentType = parentProxy->objectType();
//                if (parentType == ObjectProxy::OBJECT_MESH_MODEL) {
//                    if (childType == ObjectProxy::OBJECT_GEOMETRY) {
//                        // @todo setup geometry resource to mesh
//                    } else if (childType == ObjectProxy::OBJECT_MATERIAL) {
//                        // @todo setup material to mesh
//                    }
//                } else if (parentType == ObjectProxy::OBJECT_SKIN) {
//                    if (childType == ObjectProxy::OBJECT_CLUSTER) {
//                        // @todo add the clustor to the skin
//                        // can be defined only once
//                    }
//                } else if (parentType == ObjectProxy::OBJECT_GEOMETRY) {
//                    if (childType == ObjectProxy::OBJECT_SKIN) {

//                    }
//                } else if (parentType == ObjectProxy::OBJECT_CLUSTER) {
//                    if (childType == ObjectProxy::OBJECT_LIMB_NODE_MODEL ||
//                        childType == ObjectProxy::OBJECT_MESH_MODEL ||
//                        childType == ObjectProxy::OBJECT_NULL_NODE) {
//                        // cluster->link(childHub);
//                    }
//                } else if (parentType == ObjectProxy::OBJECT_ANIMATION_LAYER) {
//                    if (childType == ObjectProxy::OBJECT_ANIMATION_CURVE_NODE) {
//                        // @todo
//                    }
//                } else if (parentType == ObjectProxy::OBJECT_ANIMATION_CURVE_NODE) {
//                    if (childType == ObjectProxy::OBJECT_ANIMATION_CURVE) {
//                        // @todo
//                    }
//                }

//                // parent uid 0 means root (project or given root hub)
//                if (parentUid == 0) {
//                    if (topLevelHub) {
//                        // reparent
//                        childHub->setParent(topLevelHub);
//                        topLevelHub->addHub(childHub);
//                    } else {
//                        // reparent
//                        childHub->setParent(project);
//                        project->addHub(childHub);
//                    }
//                } else {
//                    // reparent
//                    childHub->setParent(parentHub);
//                    parentHub->addHub(childHub);
//                }
//            } else if (connections->connectionType(i) == ConnectionsProxy::CONN_OP) {
//                Int64 parentUid, childUid;
//                String propertyName = connections->propertyRelation(i, parentUid, childUid);

//                common::Hub* parentHub = hub(parentUid);
//                common::Hub* childHub = hub(childUid);

//                ObjectProxy* parentProxy = objectProxy(parentUid);
//                ObjectProxy* childProxy = objectProxy(childUid);

//                // parent proxy not found
//                if (parentUid != 0 && !parentProxy) {
//                    continue;
//                }

//                // parent hub not found
//                if (parentUid != 0 && !parentHub) {
//                    continue;
//                }

//                // not imported children ignore the connection
//                if (!childHub || !childProxy) {
//                    continue;
//                }

//                ObjectProxy::ObjectType childType = childProxy->objectType();
//                if (childType == ObjectProxy::OBJECT_ANIMATION_CURVE_NODE) {
//                    // @todo the parent is a reference on the bone
//                    // and the property is linked to this reference
//                }

//                ObjectProxy::ObjectType parentType = parentProxy->objectType();
//                if (parentType == ObjectProxy::OBJECT_MATERIAL) {
////                    common::MaterialHub *materialHub = static_cast<MaterialHub*>(parentHub);
////                    if (childType == ObjectProxy::OBJECT_TEXTURE) {
////                        if (propertyName == "NormalMap") {
////                            // @todo set texture
////                        } else if (propertyName == "DiffuseColor") {
////                            // @todo set texture
////                        }
////                    }
//                }
//            }
//        }

//        delete connections;
//    }

//    // @todo Post process each OBJECT_CLUSTER

//    // setup resources to editor
//    // @todo geometry, material, texture

//    // setup model node to editor
//    // @todo remove hub instances above
//    ModelProxy *currentProxy = rootProxy;
//    std::list<std::list<ModelProxy*>::iterator> cursor;

//    while (currentProxy != nullptr) {
//        // @todo type... creation...
//        currentProxy = currentProxy->recursiveNext(cursor);
//    }

//    // delete object proxies including the root proxy
//    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
//        delete it->second;
//    }

//    // and the fake root node (manually created)
//    delete m_rootNode;

//    m_objects.clear();
//}
