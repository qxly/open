#ifndef RENDERSYSTEM_LIGHT_H
#define RENDERSYSTEM_LIGHT_H

#include <RenderSystem/RenderInterface.h>
#include <RenderSystem/RenderEnumeration.h>
#include <open/IntrusiveList.h>
#include <open/Memory.h>

namespace open
{
	class LightManager;
	class Light : public ILight PUBLIC_OBJECT_DEBUG
	{
	public:
		Light(LightManager* lm) :_renderMask(0xffffffff), _shadowData(nullptr), _lightType(LIGHT_DIRECT), _near(0), _far(1000),
			_ambientColor(0.08f, 0.08f, 0.08f), _diffuseColor(1, 1, 1), _lightManager(lm){}

		~Light();

		virtual void setLightType(LightType type) { _lightType = type; }
		virtual LightType getLightType() { return _lightType; }

		virtual void setLightRange(Real n, Real f) { _near = n; _far = f; }
		virtual void getLightRange(Real& n, Real& f) { n = _near; f = _far; }

		virtual void setLightPosition(const Real3& position) { _position = position; }
		virtual const Real3& getLightPosition() { return _position; }

		virtual void setLightDirection(const Real3& direction) { _direction = direction; }
		virtual const Real3& getLightDirection() { return _direction; }

		virtual void setAmbientColor(const Real3& color) { _ambientColor = color; }
		virtual const Real3& getAmbientColor() { return _ambientColor; }

		virtual void setDiffuseColor(const Real3& color) { _diffuseColor = color; }
		virtual const Real3& getDiffuseColor() { return _diffuseColor; }

		virtual void setRenderMask(uint32 renderMask) { _renderMask = renderMask; }
		virtual uint32 getRenderMask() { return _renderMask; }

		virtual IShadowData* getShadowData() { return _shadowData; }
		virtual void setShadowData(IShadowData* shadowData) { _shadowData = shadowData; }

		TLink<Light> link;
	private:
		Real3 _position;
		Real3 _direction;
		Real3 _ambientColor;
		Real3 _diffuseColor;
		uint32 _renderMask;

		Real _near;
		Real _far;

		IShadowData* _shadowData;
		LightType _lightType;

		LightManager* _lightManager;
	};

	class LightManager : public ILightManager PUBLIC_OBJECT_DEBUG
	{
	public:

		LightManager()
			:_currentLight(nullptr), _mainLight(nullptr)
		{
		}

		~LightManager();

		virtual ILight* createLight() { return new Light(this); }

		virtual void addLight(ILight* light) { if (_lights.empty())_mainLight = light; _lights.push_back((Light*)light); }

		virtual void removeLight(ILight* light) { _lights.erase((Light*)light); if (_mainLight == light)_mainLight = nullptr; }

		virtual void begin() { _currentLight = nullptr; }

		virtual void setMainLight(ILight* light) { _mainLight = light; }

		virtual ILight* getMainLight() { return _mainLight; }

		virtual ILight* nextIterator()
		{
			if (_currentLight)
			{
				_currentLight = _lights.next(_currentLight);
			}
			else
				_currentLight = _lights.front();
			return _currentLight;
		}

		INTRUSIVELIST_DECLARE(Light, link) _lights;

		Light* _currentLight;
		ILight* _mainLight;
	};
}

#endif