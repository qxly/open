#include "GBufferRender.h"
#include <RenderSystem/RenderStatesInterface.h>
#include <common/interface/Scene/MeshInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>
#include <RenderSystem/RenderInterface.h>

#include <common/InputInterface.h>
#include <Engine/EngineInterface.h>
#include <Scene/SceneFactoryInterface.h>

namespace open
{
	struct GBufferRender::GBufferShow : public InputListener
	{
		int32 _type;

		IMaterial* _material;
		IProgram* _program;
		IMesh* _screenRect;

		GBufferShow()
			:_program(NULL), _material(NULL), _screenRect(NULL)
		{
			guiListener = functor(*this, &GBufferShow::show);
			IEngine::s_engine->addInputListener(this);
		}

		~GBufferShow()
		{
			delete _material;
			delete _program;
			delete _screenRect;
		}

		void init(GBuffer* gbuffer)
		{
			///shader
			_program = RenderSystemFactory::s_factory->createProgram();
			_program->loadFromFile("Shaders/ShowGBuffer.vert", "Shaders/ShowGBuffer.frag");

			///ÆÁÄ»¿ò
			_screenRect = SceneFactory::s_factory->createMesh();

			Vec3f* vertex = new Vec3f[4];
			uint16* index = new uint16[6];

			vertex[0] = Vec3f(-1, -1, 0);
			vertex[1] = Vec3f(1, -1, 0);
			vertex[2] = Vec3f(1, 1, 0);
			vertex[3] = Vec3f(-1, 1, 0);

			index[0] = 0; index[1] = 1; index[2] = 2;
			index[3] = 0; index[4] = 2; index[5] = 3;

			_screenRect->begin(4);

			_screenRect->setVertexArray(vertex);
			_screenRect->addIndexArray(TRIANGLE_LIST, index, 6);

			_screenRect->end();

			///²ÄÖÊ
			_material = RenderSystemFactory::s_factory->createMaterial();
			ITextureUnitSet* us = _material->getTextureUnitSet();
			ITextureUnit* tu1 = us->getTextureUnit(TextureLayer(0));
			ITextureUnit* tu2 = us->getTextureUnit(TextureLayer(1));
			ITextureUnit* tu3 = us->getTextureUnit(TextureLayer(2));
			ITextureUnit* tu4 = us->getTextureUnit(TextureLayer(3));
			tu1->setTexture(gbuffer->depthTexture);
			tu2->setTexture(gbuffer->normal_shininessTexture);
			tu3->setTexture(gbuffer->diffuse_specularTexture);
			tu4->setTexture(gbuffer->emissive_materialTexture);
			us->setTexutreUnitName(TextureLayer(0), "_depthTexture");
			us->setTexutreUnitName(TextureLayer(1), "_normal_shininessTexture");
			us->setTexutreUnitName(TextureLayer(2), "_diffuse_specularTexture");
			us->setTexutreUnitName(TextureLayer(3), "_emissiveTexture");


			_material->setMode(DEPTH_TEST, false);
		}

		void show(const Input* input)
		{
			if (input->eventType == KEYUP)
			{
				switch (input->key)
				{
				case 112:
					_type = 0;
					break;
				case 113:
					_type = 1;
					break;
				case 114:
					_type = 2;
					break;
				case 115:
					_type = 3;
					break;
				case 116:
					_type = 4;
					break;
				case 117:
					_type = 5;
					break;;
					break;
				}
			}
		}

		void draw(IRenderPool* renderPool, GBuffer* gbuffer)
		{
			if (!_program)
				init(gbuffer);

			renderPool->applyProgram(_program);
			_program->setUniformi("_showType", _type);

			RMatrix matrix = renderPool->getProjectMatrix() * renderPool->getViewMatrix();
			matrix.inverse();
			_program->setUniform16f("_vpInverse", matrix);

			renderPool->applyMaterial(_material);
			_material->getTextureUnitSet()->apply(_program);

			_screenRect->draw(renderPool);
		}
	};
	struct GBufferRender::GBufferRenderScheme : public IApplyMaterialFun
	{
		GBufferRenderScheme()
		:_program(NULL), _renderPool(NULL), _normalFitTextures(NULL)
		{
		}

		~GBufferRenderScheme()
		{
			delete _program;
			delete _normalFitTextures;
		}

		IProgram* _program;
		IRenderPool* _renderPool;
		ITextureUnitSet* _normalFitTextures;

		void initProgram()
		{
			_program = RenderSystemFactory::s_factory->createProgram();

			_program->loadFromFile("Shaders/GBuffer.vert", "Shaders/GBuffer.frag", "#define LIGHT_DIR\n");
			_program->setFragDataLocation(0, "_normal_shininess");
			_program->setFragDataLocation(1, "_diffuse_specular");

			/*_normalFitTextures = RenderSystemFactory::s_factory->createTextureUnitSet();
			ITextureUnit* tu = RenderSystemFactory::s_factory->createTextureUnit();
			ITexture* normalFitTexture = RenderSystemFactory::s_factory->createTexture("res/NormalsFittingTexture_2.dds");
			tu->setTexture(normalFitTexture);*/
			//_normalFitTextures->addTextureUnit(tu,"_normalFitTexture");
		}

		virtual void begin(IRenderPool* renderPool)
		{
			if (!_program)
			{
				initProgram();
			}
			renderPool->resetRenderState();
			renderPool->clear(DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT | COLOR_BUFFER_BIT);
			renderPool->applyProgram(_program);
			//_normalFitTextures->apply(_program);

			_renderPool = renderPool;
		}

		inline void setSubroutine(int32 flag, int32 i, const char** color_texture, const char* name)
		{
			int32 f = flag & (1 << i);
			if (f == 0)
				return;
			color_texture[i] = name;
		}

		virtual void applyMaterial(IMaterial* materail)
		{
			_program->setUniformf("_shininess", materail->getShininess() / 128.0f);
			_program->setUniform3f("_diffuseColor", materail->getDiffuseColor());
			_program->setUniform3f("_emissiveColor", materail->getEmissiveColor());

			int32 flag = materail->getTextureUnitSet()->apply(_program);

			const char* color = "Compute_Color";
			const char* texture = "Compute_Color_Texture";

			const char* color_texture[] = { color, color };

			setSubroutine(flag, 0, color_texture, texture);
			setSubroutine(flag, 1, color_texture, texture);
			_program->setSubroutine(FRAGMENT_SHADER, 2, color_texture[0], color_texture[1]);

			//_program->setSubroutine(FRAGMENT_SHADER, 1, color_texture[0]);
		}

		virtual void applyMaterial(IRenderPool* renderPool, IProgram* program, IMaterial* material)
		{
			applyMaterial(material);
		}
	};

	GBufferRender::GBufferRender()
		:_width(0), _height(0), _frameBuffer(NULL)
	{
		_renderScheme = new GBufferRenderScheme();
		_gbufferShow = new GBufferShow();
	}

	GBufferRender::~GBufferRender()
	{
		delete _frameBuffer;
		delete _renderScheme;
		delete _gbufferShow;
	}

	void GBufferRender::begin(IRenderPool* renderPool)
	{
		renderPool->applyRenderTarget(_frameBuffer);
		_renderScheme->begin(renderPool);
	}

	IRenderTarget* GBufferRender::getRenderTarget()
	{
		if (!_frameBuffer)
			resetFrameBuffer();

		return _frameBuffer;
	}

	IApplyMaterialFun& GBufferRender::getApplyMaterialFun()
	{
		return *_renderScheme;
	}

	void GBufferRender::resizeGBuffer(int32 width, int32 height)
	{
		if (_width == width && _height == height)
			return;

		_width = width > 0 ? width : 1;
		_height = height > 0 ? height : 1;
		resetFrameBuffer();
	}

	void GBufferRender::resetFrameBuffer()
	{
		ASSERT_(_width > 0 && _height > 0);

		if (!_frameBuffer)
		{
			_frameBuffer = RenderSystemFactory::s_factory->createFrameBuffer();

			_gBuffer.depthTexture = RenderSystemFactory::s_factory->createTexture();
			_gBuffer.normal_shininessTexture = RenderSystemFactory::s_factory->createTexture();
			_gBuffer.diffuse_specularTexture = RenderSystemFactory::s_factory->createTexture();
			_gBuffer.emissive_materialTexture = RenderSystemFactory::s_factory->createTexture();

			_frameBuffer->attach(DEPTH_ATTACHMENT, _gBuffer.depthTexture);
			_frameBuffer->attach(COLOR_ATTACHMENT0, _gBuffer.normal_shininessTexture);
			_frameBuffer->attach(COLOR_ATTACHMENT1, _gBuffer.diffuse_specularTexture);
			_frameBuffer->attach(COLOR_ATTACHMENT2, _gBuffer.emissive_materialTexture);
		}
		_gBuffer.depthTexture->alloc(PIXELINTERNALFORMAT_DEPTH_COMPONENT32, _width, _height, 0, PIXEFORMAT_DEPTH_COMPONENT, DATATYPE_FLOAT, NULL);
		_gBuffer.normal_shininessTexture->alloc(PIXELINTERNALFORMAT_RGBA, _width, _height, 0, PIXEFORMAT_RGBA, DATATYPE_UNSIGNED_BYTE, NULL);
		_gBuffer.diffuse_specularTexture->alloc(PIXELINTERNALFORMAT_RGBA, _width, _height, 0, PIXEFORMAT_RGBA, DATATYPE_UNSIGNED_BYTE, NULL);
		_gBuffer.emissive_materialTexture->alloc(PIXELINTERNALFORMAT_RGBA, _width, _height, 0, PIXEFORMAT_RGBA, DATATYPE_UNSIGNED_BYTE, NULL);
	}

	void GBufferRender::renderGBuffer()
	{
		this->_gbufferShow->draw(_renderScheme->_renderPool, &_gBuffer);
	}
}