#include "Material.h"
#include <Render/RenderEngine.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>

namespace open
{
	Material::Material()
	{
		_textureUnits = RenderSystemFactory::s_factory->createTextureUnitSet();
	}

	Material::~Material()
	{
		delete _textureUnits;
	}

	Value& Material::getRenderParameter(const char* name) {
		return _parameters[name];
	}

	IMaterial * Material::clone()
	{
		throw "not support";

		Material* ma = new Material;
		ma->_rss = _rss;
		ma->_textureUnits = _textureUnits;
		ma->_technique = _technique;

		return ma;
	}

	void RenderTechnique::setRenderScript(const RenderScript & renderScript)
	{
		_renderScript = renderScript;
	}

	IProgram* RenderTechnique::getProgram() {
		if (!_program) {
			_program = RenderSystemFactory::s_factory->createProgram();
			_program->loadFromFile(_renderScript.shader->vert, _renderScript.shader->frag, _renderScript.shader->define);
		}
		return _program;
	}
}