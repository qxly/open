#include "Light.h"

namespace open
{
	Light::~Light()
	{
		if (_shadowData)
			delete _shadowData;
		_shadowData = nullptr;

		if (_lightManager->_mainLight == this)
			_lightManager->_mainLight = nullptr;
	}

	LightManager::~LightManager()
	{
		_lights.clear_destory();
		_currentLight = nullptr;
	}
}