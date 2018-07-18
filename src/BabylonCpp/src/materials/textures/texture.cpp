#include <babylon/materials/textures/texture.h>

#include <babylon/babylon_stl_util.h>
#include <babylon/core/json.h>
#include <babylon/engine/engine.h>
#include <babylon/engine/scene.h>
#include <babylon/materials/material.h>
#include <babylon/materials/textures/internal_texture.h>
#include <babylon/mesh/buffer.h>
#include <babylon/tools/tools.h>

namespace BABYLON {

bool Texture::UseSerializedUrlIfAny = false;

Texture::Texture(const string_t& _url, Scene* scene, bool noMipmap,
                 bool invertY, unsigned int samplingMode,
                 const ::std::function<void()>& onLoad,
                 const ::std::function<void()>& onError,
                 const Nullable<Variant<ArrayBuffer, Image>>& buffer,
                 bool deleteBuffer, const Nullable<unsigned int>& format)
    : BaseTexture{scene}
    , url{_url}
    , uOffset{0.f}
    , vOffset{0.f}
    , uScale{1.0}
    , vScale{1.0}
    , uAng{0.f}
    , vAng{0.f}
    , wAng{0.f}
    , uRotationCenter{0.5f}
    , vRotationCenter{0.5f}
    , wRotationCenter{0.5f}
    , noMipmap{this, &Texture::get_noMipmap}
    , _invertY{invertY}
    , _samplingMode{samplingMode}
    , samplingMode{this, &Texture::get_samplingMode}
    , onLoadObservable{this, &Texture::get_onLoadObservable}
    , _format{format}
    , _isBlocking{true}
    , _noMipmap{noMipmap}
    , _rowGenerationMatrix{nullptr}
    , _cachedTextureMatrix{nullptr}
    , _projectionModeMatrix{nullptr}
    , _buffer{buffer}
    , _deleteBuffer{deleteBuffer}
    , _onLoad{onLoad}
{
  name = _url;

  if (!scene) {
    return;
  }
  scene->getEngine()->onBeforeTextureInitObservable.notifyObservers(this);

  _load = [this](InternalTexture*, EventState&) {
    auto onLoadObservable = *_onLoadObservable;
    if (onLoadObservable.hasObservers()) {
      onLoadObservable.notifyObservers(this);
    }
    if (_onLoad) {
      _onLoad();
    }

    auto scene = getScene();
    if (!isBlocking() && scene) {
      scene->resetCachedMaterial();
    }
  };

  if (url.empty()) {
    _delayedOnLoad  = _load;
    _delayedOnError = onError;
    return;
  }

  _texture = _getFromCache(url, noMipmap, samplingMode);

  if (!_texture) {
    if (!scene->useDelayedTextureLoading) {
      _texture = scene->getEngine()->createTexture(
        url, noMipmap, invertY, scene, _samplingMode, _load, onError, _buffer,
        nullptr, _format);
      if (deleteBuffer) {
        _buffer = nullptr;
      }
    }
    else {
      delayLoadState = EngineConstants::DELAYLOADSTATE_NOTLOADED;

      _delayedOnLoad  = _load;
      _delayedOnError = onError;
    }
  }
  else {
    if (_texture->isReady) {
      Tools::SetImmediate([this]() {
        EventState es{-1};
        _load(nullptr, es);
      });
    }
    else {
      _texture->onLoadedObservable.add(_load);
    }
  }
}

Texture::~Texture()
{
}

IReflect::Type Texture::type() const
{
  return IReflect::Type::TEXTURE;
}

const char* Texture::getClassName() const
{
  return "Texture";
}

void Texture::set_isBlocking(bool value)
{
  _isBlocking = value;
}

bool Texture::get_isBlocking() const
{
  return _isBlocking;
}

unsigned int Texture::get_samplingMode() const
{
  return _samplingMode;
}

bool Texture::get_noMipmap() const
{
  return _noMipmap;
}

void Texture::updateURL(const string_t& iUrl,
                        const Nullable<Variant<ArrayBuffer, Image>>& buffer)
{
  if (iUrl.empty()) {
    throw ::std::runtime_error("URL is already set");
  }

  url            = iUrl;
  _buffer        = buffer;
  delayLoadState = EngineConstants::DELAYLOADSTATE_NOTLOADED;
  delayLoad();
}

void Texture::delayLoad()
{
  if (delayLoadState != EngineConstants::DELAYLOADSTATE_NOTLOADED) {
    return;
  }

  auto scene = getScene();
  if (!scene) {
    return;
  }

  delayLoadState = EngineConstants::DELAYLOADSTATE_LOADED;
  _texture       = _getFromCache(url, _noMipmap, _samplingMode);

  if (!_texture) {
#if 0
    _texture = scene->getEngine()->createTexture(
      url, _noMipmap, _invertY, getScene(), _samplingMode, _delayedOnLoad,
      _delayedOnError, _buffer, nullptr, _format);
    if (_deleteBuffer) {
      delete _buffer;
    }
#endif
  }
  else {
    if (_delayedOnLoad) {
      if (_texture->isReady) {
        Tools::SetImmediate([this]() {
          EventState es{-1};
          _delayedOnLoad(nullptr, es);
        });
      }
      else {
        _texture->onLoadedObservable.add(_delayedOnLoad);
      }
    }
  }

  _delayedOnLoad  = nullptr;
  _delayedOnError = nullptr;
}

void Texture::updateSamplingMode(unsigned int samplingMode)
{
  if (!_texture) {
    return;
  }

  auto scene = getScene();
  if (!scene) {
    return;
  }

  _samplingMode = samplingMode;
  scene->getEngine()->updateTextureSamplingMode(samplingMode, _texture);
}

void Texture::_prepareRowForTextureGeneration(float x, float y, float z,
                                              Vector3& t)
{
  x *= uScale;
  y *= vScale;

  x -= uRotationCenter * uScale;
  y -= vRotationCenter * vScale;
  z -= wRotationCenter;

  Vector3::TransformCoordinatesFromFloatsToRef(x, y, z, *_rowGenerationMatrix,
                                               t);

  t.x += uRotationCenter * uScale + uOffset;
  t.y += vRotationCenter * vScale + vOffset;
  t.z += wRotationCenter;
}

Matrix* Texture::getTextureMatrix()
{
  if (stl_util::almost_equal(uOffset, _cachedUOffset)
      && stl_util::almost_equal(vOffset, _cachedVOffset)
      && stl_util::almost_equal(uScale, _cachedUScale)
      && stl_util::almost_equal(vScale, _cachedVScale)
      && stl_util::almost_equal(uAng, _cachedUAng)
      && stl_util::almost_equal(vAng, _cachedVAng)
      && stl_util::almost_equal(wAng, _cachedWAng)) {
    return _cachedTextureMatrix.get();
  }

  _cachedUOffset = uOffset;
  _cachedVOffset = vOffset;
  _cachedUScale  = uScale;
  _cachedVScale  = vScale;
  _cachedUAng    = uAng;
  _cachedVAng    = vAng;
  _cachedWAng    = wAng;

  if (!_cachedTextureMatrix) {
    _cachedTextureMatrix = ::std::make_unique<Matrix>(Matrix::Zero());
    _rowGenerationMatrix = ::std::make_unique<Matrix>();
    _t0                  = Vector3::Zero();
    _t1                  = Vector3::Zero();
    _t2                  = Vector3::Zero();
  }

  Matrix::RotationYawPitchRollToRef(vAng, uAng, wAng, *_rowGenerationMatrix);

  _prepareRowForTextureGeneration(0.f, 0.f, 0.f, _t0);
  _prepareRowForTextureGeneration(1.f, 0.f, 0.f, _t1);
  _prepareRowForTextureGeneration(0.f, 1.f, 0.f, _t2);

  _t1.subtractInPlace(_t0);
  _t2.subtractInPlace(_t0);

  Matrix::IdentityToRef(*_cachedTextureMatrix);
  _cachedTextureMatrix->m[0] = _t1.x;
  _cachedTextureMatrix->m[1] = _t1.y;
  _cachedTextureMatrix->m[2] = _t1.z;

  _cachedTextureMatrix->m[4] = _t2.x;
  _cachedTextureMatrix->m[5] = _t2.y;
  _cachedTextureMatrix->m[6] = _t2.z;

  _cachedTextureMatrix->m[8]  = _t0.x;
  _cachedTextureMatrix->m[9]  = _t0.y;
  _cachedTextureMatrix->m[10] = _t0.z;

  auto scene = getScene();
  if (!scene) {
    return _cachedTextureMatrix.get();
  }

  scene->markAllMaterialsAsDirty(
    Material::TextureDirtyFlag(),
    [this](Material* mat) { return mat->hasTexture(this); });

  return _cachedTextureMatrix.get();
}

Matrix* Texture::getReflectionTextureMatrix()
{
  auto scene = getScene();
  if (!scene) {
    return _cachedTextureMatrix.get();
  }

  if (stl_util::almost_equal(uOffset, _cachedUOffset)
      && stl_util::almost_equal(vOffset, _cachedVOffset)
      && stl_util::almost_equal(uScale, _cachedUScale)
      && stl_util::almost_equal(vScale, _cachedVScale)
      && (coordinatesMode() == _cachedCoordinatesMode)) {
    if (coordinatesMode() == TextureConstants::PROJECTION_MODE) {
      if (_cachedProjectionMatrixId
          == scene->getProjectionMatrix().updateFlag) {
        return _cachedTextureMatrix.get();
      }
    }
    else {
      return _cachedTextureMatrix.get();
    }
  }

  if (!_cachedTextureMatrix) {
    _cachedTextureMatrix = ::std::make_unique<Matrix>(Matrix::Zero());
  }

  if (!_projectionModeMatrix) {
    _projectionModeMatrix = ::std::make_unique<Matrix>(Matrix::Zero());
  }

  _cachedUOffset         = uOffset;
  _cachedVOffset         = vOffset;
  _cachedUScale          = uScale;
  _cachedVScale          = vScale;
  _cachedCoordinatesMode = coordinatesMode();

  switch (coordinatesMode()) {
    case TextureConstants::PLANAR_MODE:
      Matrix::IdentityToRef(*_cachedTextureMatrix);
      _cachedTextureMatrix->m[0]  = uScale;
      _cachedTextureMatrix->m[5]  = vScale;
      _cachedTextureMatrix->m[12] = uOffset;
      _cachedTextureMatrix->m[13] = vOffset;
      break;
    case TextureConstants::PROJECTION_MODE: {
      Matrix::IdentityToRef(*_projectionModeMatrix);

      _projectionModeMatrix->m[0]  = 0.5f;
      _projectionModeMatrix->m[5]  = -0.5f;
      _projectionModeMatrix->m[10] = 0.f;
      _projectionModeMatrix->m[12] = 0.5f;
      _projectionModeMatrix->m[13] = 0.5f;
      _projectionModeMatrix->m[14] = 1.f;
      _projectionModeMatrix->m[15] = 1.f;

      auto projectionMatrix     = scene->getProjectionMatrix();
      _cachedProjectionMatrixId = projectionMatrix.updateFlag;
      projectionMatrix.multiplyToRef(*_projectionModeMatrix,
                                     *_cachedTextureMatrix);
    } break;
    default:
      Matrix::IdentityToRef(*_cachedTextureMatrix);
      break;
  }

  scene->markAllMaterialsAsDirty(
    Material::TextureDirtyFlag(), [this](Material* mat) {
      return stl_util::contains(mat->getActiveTextures(), this);
    });

  return _cachedTextureMatrix.get();
}

Texture* Texture::clone() const
{
  auto newTexture = Texture::New(_texture ? _texture->url : nullptr, getScene(),
                                 _noMipmap, _invertY, _samplingMode);

  // Base texture
  newTexture->hasAlpha         = hasAlpha();
  newTexture->level            = level;
  newTexture->wrapU            = wrapU;
  newTexture->wrapV            = wrapV;
  newTexture->coordinatesIndex = coordinatesIndex;
  newTexture->coordinatesMode  = coordinatesMode();

  // Texture
  newTexture->uOffset = uOffset;
  newTexture->vOffset = vOffset;
  newTexture->uScale  = uScale;
  newTexture->vScale  = vScale;
  newTexture->uAng    = uAng;
  newTexture->vAng    = vAng;
  newTexture->wAng    = wAng;

  return newTexture;
}

Nullable<Observable<Texture>>& Texture::get_onLoadObservable()
{
  return _onLoadObservable;
}

Json::object Texture::serialize() const
{
  return Json::object();
}

void Texture::dispose()
{
  BaseTexture::dispose();

  if (_onLoadObservable) {
    auto observable = *_onLoadObservable;
    observable.clear();
    _onLoadObservable.resetValue();
  }

  _delayedOnLoad  = nullptr;
  _delayedOnError = nullptr;
}

Texture* Texture::CreateFromBase64String(const string_t& /*data*/,
                                         const string_t& name, Scene* scene,
                                         bool noMipmap, bool invertY,
                                         unsigned int samplingMode,
                                         const ::std::function<void()>& onLoad,
                                         const ::std::function<void()>& onError,
                                         unsigned int format)
{
  return Texture::New("data:" + name, scene, noMipmap, invertY, samplingMode,
                      onLoad, onError, nullptr, false, format);
}

unique_ptr_t<BaseTexture> Texture::Parse(const Json::value& /*parsedTexture*/,
                                         Scene* /*scene*/,
                                         const string_t& /*rootUrl*/)
{
  return nullptr;
}

Texture* Texture::LoadFromDataString(
  const string_t& name, const Nullable<Variant<ArrayBuffer, Image>>& buffer,
  Scene* scene, bool deleteBuffer, bool noMipmap, bool invertY,
  unsigned int samplingMode, const ::std::function<void()>& onLoad,
  const ::std::function<void()>& onError, unsigned int format)
{
  string_t _name = name;
  if (_name.substr(0, 5) != "data:") {
    _name = "data:" + name;
  }

  return Texture::New(name, scene, noMipmap, invertY, samplingMode, onLoad,
                      onError, buffer, deleteBuffer, format);
}

} // end of namespace BABYLON
