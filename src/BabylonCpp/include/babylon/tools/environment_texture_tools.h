#ifndef BABYLON_TOOLS_ENVIRONMENT_TEXTURE_TOOLS
#define BABYLON_TOOLS_ENVIRONMENT_TEXTURE_TOOLS

#include <array>
#include <memory>

#include <babylon/babylon_api.h>
#include <babylon/babylon_common.h>

namespace BABYLON {

struct ArrayBufferView;
class EnvironmentTextureInfo;
class InternalTexture;
using EnvironmentTextureInfoPtr = std::shared_ptr<EnvironmentTextureInfo>;
using InternalTexturePtr        = std::shared_ptr<InternalTexture>;

/**
 * @brief Sets of helpers addressing the serialization and deserialization of
 * environment texture stored in a BabylonJS env file. Those files are usually
 * stored as .env files.
 */
class BABYLON_SHARED_EXPORT EnvironmentTextureTools {

private:
  /**
   * Magic number identifying the env file.
   */
  static std::array<uint8_t, 8> _MagicBytes;

public:
  EnvironmentTextureTools();
  ~EnvironmentTextureTools();

  /**
   * @brief Gets the environment info from an env file.
   * @param data The array buffer containing the .env bytes.
   * @returns the environment file info (the json header) if successfully
   * parsed.
   */
  static EnvironmentTextureInfoPtr GetEnvInfo(const ArrayBuffer& data);

  /**
   * @brief Uploads the texture info contained in the env file to the GPU.
   * @param texture defines the internal texture to upload to
   * @param arrayBuffer defines the buffer cotaining the data to load
   * @param info defines the texture info retrieved through the GetEnvInfo
   * method
   */
  static void UploadEnvLevels(const InternalTexturePtr& texture,
                              const ArrayBuffer& arrayBuffer,
                              const EnvironmentTextureInfo& info);

  /**
   * @brief Uploads the levels of image data to the GPU.
   * @param texture defines the internal texture to upload to
   * @param imageData defines the array buffer views of image data
   * [mipmap][face]
   */
  static void
  UploadLevels(const InternalTexturePtr& texture,
               const std::vector<std::vector<ArrayBuffer>>& imageData);

  /**
   * @brief Uploads spherical polynomials information to the texture.
   * @param texture defines the texture we are trying to upload the information
   * to
   * @param info defines the environment texture info retrieved through the
   * GetEnvInfo method
   */
  static void UploadEnvSpherical(const InternalTexturePtr& texture,
                                 const EnvironmentTextureInfo& info);

}; // end of class EnvironmentTextureTools

} // end of namespace BABYLON

#endif // end of BABYLON_TOOLS_ENVIRONMENT_TEXTURE_TOOLS
