#include <babylon/engine/asset_container.h>

#include <babylon/audio/sound.h>
#include <babylon/audio/sound_track.h>
#include <babylon/engine/iscene_serializable_component.h>
#include <babylon/engine/scene.h>
#include <babylon/mesh/mesh.h>

namespace BABYLON {

AssetContainer::AssetContainer(Scene* iScene) : AbstractScene{}, scene{iScene}
{
}

AssetContainer::~AssetContainer()
{
}

void AssetContainer::addAllToScene()
{
  for (const auto& o : cameras) {
    scene->addCamera(o);
  }
  for (const auto& o : lights) {
    scene->addLight(o);
  }
  for (const auto& o : meshes) {
    scene->addMesh(o);
  }
  for (const auto& o : skeletons) {
    scene->addSkeleton(o);
  }
  for (const auto& o : animations) {
    scene->addAnimation(o);
  }
  for (const auto& o : animationGroups) {
    scene->addAnimationGroup(o);
  }
  for (const auto& o : multiMaterials) {
    scene->addMultiMaterial(o);
  }
  for (const auto& o : materials) {
    scene->addMaterial(o);
  }
  for (const auto& o : morphTargetManagers) {
    scene->addMorphTargetManager(o);
  }
  for (const auto& o : geometries) {
    scene->addGeometry(o);
  }
  for (const auto& o : transformNodes) {
    scene->addTransformNode(o);
  }
  for (const auto& o : actionManagers) {
    scene->addActionManager(o);
  }
  for (const auto& o : sounds) {
    o->play();
    o->autoplay = true;
    scene->mainSoundTrack()->AddSound(o);
  };
  for (const auto& o : textures) {
    scene->addTexture(o);
  }

  for (const auto& component : scene->_serializableComponents) {
    component->addFromContainer(scene);
  }
}

void AssetContainer::removeAllFromScene()
{
  for (const auto& o : cameras) {
    scene->removeCamera(o);
  }
  for (const auto& o : lights) {
    scene->removeLight(o);
  }
  for (const auto& o : meshes) {
    scene->removeMesh(o);
  }
  for (const auto& o : skeletons) {
    scene->removeSkeleton(o);
  }
  for (const auto& o : animations) {
    scene->removeAnimation(o);
  }
  for (const auto& o : animationGroups) {
    scene->removeAnimationGroup(o);
  }
  for (const auto& o : multiMaterials) {
    scene->removeMultiMaterial(o);
  }
  for (const auto& o : materials) {
    scene->removeMaterial(o);
  }
  for (const auto& o : morphTargetManagers) {
    scene->removeMorphTargetManager(o);
  }
  for (const auto& o : geometries) {
    scene->removeGeometry(o);
  }
  for (const auto& o : transformNodes) {
    scene->removeTransformNode(o);
  }
  for (const auto& o : actionManagers) {
    scene->removeActionManager(o);
  }
  for (const auto& o : sounds) {
    o->stop();
    o->autoplay = false;
    scene->mainSoundTrack()->RemoveSound(o);
  }
  for (const auto& o : textures) {
    scene->removeTexture(o);
  }

  for (const auto& component : scene->_serializableComponents) {
    component->removeFromContainer(scene);
  }
}

MeshPtr AssetContainer::createRootMesh()
{
  auto rootMesh = Mesh::New("assetContainerRootMesh", scene);
  for (const auto& m : meshes) {
    if (!m->parent) {
      rootMesh->addChild(*m);
    }
  }
  meshes.insert(meshes.begin(), rootMesh);
  return rootMesh;
}

} // end of namespace BABYLON
