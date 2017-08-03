#include "ForwardShader.h"

#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>

#include <Scene/MeshInterface.h>
#include <Scene/NodeInterface.h>
#include <Scene/SceneFactoryInterface.h>

namespace open
{
	class ForwardShader::DefaultRenderScheme : public IApplyMaterialFun
	{
	public:
		IProgram* _program;
		IRenderPool* _renderPool;

		DefaultRenderScheme() :_program(NULL)
		{
		}

		void begin(IRenderPool* renderPool)
		{
			if (!_program)
			{
				_program = RenderSystemFactory::s_factory->createProgram();
				_program->loadFromFile("Shaders/Basic.vert", "Shaders/Basic.frag");
			}

			_renderPool = renderPool;
			renderPool->applyProgram(_program);

			RMatrix mm(renderPool->getProjectMatrix());
			mm.inverse();
			_program->setUniform16f("_projectInverse", mm);
		}

		inline void setSubroutine(int32 flag, int32 i, const char** color_texture, const char* name)
		{
			int32 f = flag & (1 << i);
			if (f == 0)
				return;
			color_texture[i] = name;
		}

		void applyMaterial(IMaterial* materail)
		{
			int32 flag = materail->getTextureUnitSet()->apply(_program);
			const char* color = "Compute_Color";
			const char* texture = "Compute_Color_Texture";
			const char* opacity_Null = "ApplyOpacity_Null";
			const char* opacity_texture = "ApplyOpacity_Texture";

			const char* color_texture[3] = { color, color, opacity_Null };

			setSubroutine(flag, 0, color_texture, texture);
			setSubroutine(flag, 1, color_texture, texture);
			setSubroutine(flag, 2, color_texture, opacity_texture);

			_program->setSubroutine(FRAGMENT_SHADER, 3, color_texture[0], color_texture[1], color_texture[2]);
		}

		void apply(ILight* light, IMaterial* material)
		{
			Real f, n;
			light->getLightRange(n, f);

			const RMatrix& viewMatrix = _renderPool->getViewMatrix();
			if (light->getLightType() == LIGHT_DIRECT)
			{
				Real3 lp_3 = light->getLightDirection();
				lp_3 *= -1;

				RQuat nn = viewMatrix.getRotate();
				lp_3 = nn * lp_3;
				lp_3.normalize();
				_program->setUniform3f("_lightPosition", lp_3);
				_program->setSubroutine(VERTEX_SHADER, 1, "ComputeLightDir_DirLight");
			}
			else if (light->getLightType() == LIGHT_POINT)
			{
				RMatrix pro = _renderPool->getProjectMatrix();
				pro.inverse();
				_program->setUniform16f("_projectInverse", pro);

				Real3 lp_3 = light->getLightPosition();
				lp_3 = viewMatrix * lp_3;
				_program->setUniform3f("_lightPosition", lp_3);
				_program->setSubroutine(VERTEX_SHADER, 1, "ComputeLightDir_PointLight");

				_program->setUniformf("_lightRange", f);
			}
			else
			{
			}
			_program->setUniform3f("_light_diffuseColor", light->getDiffuseColor());
			_program->setUniform3f("_light_ambientColor", light->getAmbientColor());

			_program->setUniform3f("_diffuseColor", material->getDiffuseColor());
			_program->setUniform3f("_emissiveColor", material->getEmissiveColor());
		}

		virtual void applyMaterial(IRenderPool* renderPool, IProgram* program, IMaterial* material)
		{
			applyMaterial(material);
		}
	};

	ForwardShader::ForwardShader()
		:_renderScheme(new DefaultRenderScheme)
	{
	}

	void ForwardShader::begin(IRenderPool* renderPool)
	{
		_renderScheme->begin(renderPool);
	}

	IApplyMaterialFun& ForwardShader::getApplyMaterialFun()
	{
		return *_renderScheme;
	}

	void ForwardShader::apply(ILight* light, IMaterial* material)
	{
		_renderScheme->apply(light, material);
	}
}
