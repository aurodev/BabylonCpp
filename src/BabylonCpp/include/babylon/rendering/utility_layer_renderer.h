#ifndef BABYLON_RENDERING_UTILITY_LAYER_RENDER_H
#define BABYLON_RENDERING_UTILITY_LAYER_RENDER_H

#include <babylon/babylon_global.h>
#include <babylon/core/structs.h>
#include <babylon/interfaces/idisposable.h>
#include <babylon/tools/observable.h>
#include <babylon/tools/observer.h>

namespace BABYLON {

/**
 * @brief Renders a layer on top of an existing scene.
 */
class BABYLON_SHARED_EXPORT UtilityLayerRenderer : public IDisposable {

public:
  /**
   * @brief Instantiates a UtilityLayerRenderer.
   * @param originalScene the original scene that will be rendered on top of
   */
  UtilityLayerRenderer(Scene* originalScene);
  virtual ~UtilityLayerRenderer();

  /**
   * @brief Renders the utility layers scene on top of the original scene.
   */
  void render();

  /**
   * @brief Disposes of the renderer.
   */
  void dispose(bool doNotRecurse               = false,
               bool disposeMaterialAndTextures = false) override;

private:
  void _notifyObservers(const PointerInfoPre& prePointerInfo,
                        const PickingInfo& pickInfo,
                        const PointerEvent& pointerEvent);
  void _updateCamera();

public:
  /**
   * The scene that is rendered on top of the original scene
   */
  unique_ptr_t<Scene> utilityLayerScene;

  /**
   *  If the utility layer should automatically be rendered on top of existing
   * scene
   */
  bool shouldRender;

  /**
   * If set to true, only pointer down onPointerObservable events will be
   * blocked when picking is occluded by original scene
   */
  bool onlyCheckPointerDownEvents;

  /**
   * If set to false, only pointerUp, pointerDown and pointerMove will be sent
   * to the utilityLayerScene (false by default)
   */
  bool processAllEvents;

  /**
   * The original scene that will be rendered on top of
   */
  Scene* originalScene;

  /**
   * Observable raised when the pointer move from the utility layer scene to the
   * main scene
   */
  Observable<int> onPointerOutObservable;

  /**
   * Gets or sets a predicate that will be used to indicate utility meshes
   * present in the main scene
   */
  ::std::function<bool(AbstractMesh* mesh)> mainSceneTrackerPredicate;

private:
  unordered_map_t<int, bool> _pointerCaptures;
  unordered_map_t<int, PointerType> _lastPointerEvents;
  Observer<Scene>::Ptr _afterRenderObserver;
  Observer<Scene>::Ptr _sceneDisposeObserver;
  Observer<PointerInfoPre>::Ptr _originalPointerObserver;

}; // end of class UtilityLayerRenderer

} // end of namespace BABYLON

#endif // end of BABYLON_RENDERING_UTILITY_LAYER_RENDER_H