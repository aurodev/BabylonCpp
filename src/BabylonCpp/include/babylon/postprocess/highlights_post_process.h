#ifndef BABYLON_POSTPROCESS_HIGHLIGHTS_POST_PROCESS_H
#define BABYLON_POSTPROCESS_HIGHLIGHTS_POST_PROCESS_H

#include <babylon/babylon_api.h>
#include <babylon/engine/engine_constants.h>
#include <babylon/postprocess/post_process.h>

namespace BABYLON {

/**
 * @brief Extracts highlights from the image.
 * @see https://doc.babylonjs.com/how_to/how_to_use_postprocesses
 */
class BABYLON_SHARED_EXPORT HighlightsPostProcess : public PostProcess {

public:
  /**
   * @brief Extracts highlights from the image.
   * @see https://doc.babylonjs.com/how_to/how_to_use_postprocesses
   * @param name The name of the effect.
   * @param options The required width/height ratio to downsize to before
   * computing the render pass.
   * @param camera The camera to apply the render pass to.
   * @param samplingMode The sampling mode to be used when computing the pass.
   * (default: 0)
   * @param engine The engine which the post process will be applied. (default:
   * current engine)
   * @param reusable If the post process can be reused on the same frame.
   * (default: false)
   * @param textureType Type of texture for the post process (default:
   * Engine.TEXTURETYPE_UNSIGNED_INT)
   */
  HighlightsPostProcess(const std::string& name, float ratio,
                        const CameraPtr& camera, unsigned int samplingMode,
                        Engine* engine, bool reusable = false,
                        unsigned int textureType
                        = EngineConstants::TEXTURETYPE_UNSIGNED_INT);
  ~HighlightsPostProcess();

}; // end of class HighlightsPostProcess

} // end of namespace BABYLON

#endif // end of BABYLON_POSTPROCESS_HIGHLIGHTS_POST_PROCESS_H
