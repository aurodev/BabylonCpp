#ifndef BABYLON_CAMERAS_STEREOSCOPIC_ANAGLYPH_ARC_ROTATE_CAMERA_H
#define BABYLON_CAMERAS_STEREOSCOPIC_ANAGLYPH_ARC_ROTATE_CAMERA_H

#include <babylon/babylon_global.h>
#include <babylon/cameras/arc_rotate_camera.h>

namespace BABYLON {

/**
 * @brief Camera used to simulate anaglyphic rendering (based on
 * ArcRotateCamera).
 */
class BABYLON_SHARED_EXPORT AnaglyphArcRotateCamera : public ArcRotateCamera {

public:
  static void AddNodeConstructor();

public:
  template <typename... Ts>
  static AnaglyphArcRotateCameraPtr New(Ts&&... args)
  {
    auto camera = shared_ptr_t<AnaglyphArcRotateCamera>(
      new AnaglyphArcRotateCamera(::std::forward<Ts>(args)...));
    camera->addToScene(camera);

    return camera;
  }
  ~AnaglyphArcRotateCamera() override;

  /**
   * @brief Gets camera class name
   * @returns AnaglyphArcRotateCamera
   */
  const string_t getClassName() const override;

protected:
  /**
   * Creates a new AnaglyphArcRotateCamera
   * @param name defines camera name
   * @param alpha defines alpha angle (in radians)
   * @param beta defines beta angle (in radians)
   * @param radius defines radius
   * @param target defines camera target
   * @param interaxialDistance defines distance between each color axis
   * @param scene defines the hosting scene
   */
  AnaglyphArcRotateCamera(const string_t& name, float alpha, float beta,
                          float radius, const Vector3& target,
                          float interaxialDistance, Scene* scene);

private:
  static bool NodeConstructorAdded;

}; // end of class AnaglyphArcRotateCamera

} // end of namespace BABYLON

#endif // end of BABYLON_CAMERAS_STEREOSCOPIC_ANAGLYPH_ARC_ROTATE_CAMERA_H
