#ifndef BABYLON_GIZMOS_GIZMO_H
#define BABYLON_GIZMOS_GIZMO_H

#include <babylon/babylon_global.h>
#include <babylon/interfaces/idisposable.h>
#include <babylon/math/matrix.h>
#include <babylon/math/vector3.h>
#include <babylon/tools/observer.h>

namespace BABYLON {

/**
 * @brief Renders gizmos on top of an existing scene which provide controls for
 * position, rotation, etc.
 */
class BABYLON_SHARED_EXPORT Gizmo : public IDisposable {

public:
  /**
   * @brief Creates a gizmo.
   * @param gizmoLayer The utility layer the gizmo will be added to
   */
  Gizmo(const shared_ptr_t<UtilityLayerRenderer>& gizmoLayer);
  virtual ~Gizmo();

  /**
   * @brief Disposes and replaces the current meshes in the gizmo with the
   * specified mesh.
   * @param mesh The mesh to replace the default mesh of the gizmo
   */
  virtual void setCustomMesh(Mesh* mesh);

  /**
   * @brief Disposes of the gizmo.
   */
  void dispose(bool doNotRecurse               = false,
               bool disposeMaterialAndTextures = false) override;

protected:
  /**
   * @brief Gets the mesh that the gizmo will be attached to. (eg. on a drag
   * gizmo the mesh that will be dragged).
   */
  AbstractMesh*& get_attachedMesh();

  /**
   * @brief Sets the mesh that the gizmo will be attached to. (eg. on a drag
   * gizmo the mesh that will be dragged).
   */
  virtual void set_attachedMesh(AbstractMesh* const& value);

  virtual void _attachedMeshChanged(AbstractMesh* value);

  /**
   * Hidden
   * @brief Updates the gizmo to match the attached mesh's position/rotation.
   */
  void _update();

public:
  /**
   * Mesh that the gizmo will be attached to. (eg. on a drag gizmo the mesh that
   * will be dragged)
   * * When set, interactions will be enabled
   */
  Property<Gizmo, AbstractMesh*> attachedMesh;

  /**
   * The utility layer the gizmo will be added to
   */
  shared_ptr_t<UtilityLayerRenderer> gizmoLayer;

  /**
   * If set the gizmo's rotation will be updated to match the attached mesh each
   * frame (Default: true)
   */
  bool updateGizmoRotationToMatchAttachedMesh;

  /**
   * If set the gizmo's position will be updated to match the attached mesh each
   * frame (Default: true)
   */
  bool updateGizmoPositionToMatchAttachedMesh;

protected:
  /**
   * The root mesh of the gizmo
   */
  MeshPtr _rootMesh;

  /**
   * If a custom mesh has been set (Default: false)
   */
  bool _customMeshSet;

  /**
   * When set, the gizmo will always appear the same size no matter where the
   * camera is (default: false)
   */
  bool _updateScale;
  bool _interactionsEnabled;

private:
  AbstractMesh* _attachedMesh;
  float _scaleFactor;
  Matrix _tmpMatrix;
  Vector3 _tempVector;
  Observer<Scene>::Ptr _beforeRenderObserver;

}; // end of class Gizmo

} // end of namespace BABYLON

#endif // end of BABYLON_GIZMOS_GIZMO_H
