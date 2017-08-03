#include "Text.h"
#include "Font.h"
#include "DefaultFont.h"
#include "Glyph.h"
#include "freetype/FreeTypeLibrary.h"

#include <RenderSystem/BufferInterface.h>
#include <RenderSystem/RenderEnumeration.h>
#include <RenderSystem/RenderInterface.h>
#include <RenderSystem/RenderStatesInterface.h>

#include <Render/Materialinterface.h>
#include <Render/RenderFactoryInterface.h>
#include <Render/RenderEngine.h>

namespace open
{
	Ptr<IGeometry> Text::s_text_geometry = []() {
		IMesh* geometry = RenderFactory::s_factory->createMesh();
		geometry->begin(4);

		Vec3f* coords = new Vec3f[4];
		float c = 0.5f;
		coords[0] = Vec3f(-c, c, 0);
		coords[1] = Vec3f(-c, -c, 0);
		coords[2] = Vec3f(c, c, 0);
		coords[3] = Vec3f(c, -c, 0);

		geometry->setVertexArray(coords);

		geometry->end();
		geometry->addIndexArray(TRIANGLE_STRIP, (uint32)0, 4);

		return geometry;
	}();

	IRenderTechnique* Text::renderTextSchema() {

		static Shader shader[] = {
			Shader("Shaders/Text.vert", "Shaders/Text.frag")
		};
		static UniformDetial uds[] = {
			UniformDetial("_fontTexture",1,[](void* ,IMaterial* material ,IRenderPool* renderPool, Value& value) {
			value.set(0);
		}),
			UniformDetial(0)
		};

		static RenderScript rps;
		rps.name = "TextRender";
		rps.shader = shader;
		rps.uniformDetials = uds;
		rps.genInstance = [](IRenderPool* renderPool, IVertexStream* instanceStream, const PreInstancData* datas, int size) {

			struct TextDrawInstance {
				Vec3f position;
				Vec4f textureCoord;
				Vec4f color;
				Matrixf mvp;
			};

			RMatrix vp = renderPool->getProjectMatrix() * renderPool->getViewMatrix();

			RQuat rotate = renderPool->getViewMatrix().getRotate();
			rotate.inverse();
			RMatrix rotateMatrix;
			rotateMatrix.makeRotate(rotate);

			uint64 vdt[] = { IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT3,IVertexStream::VERXT_INSTANCE0),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE1),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE2),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE3),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE4),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE5),
				IVertexStream::genVertexType(IVertexStream::VERXTEXDATATYPE_FLOAT4,IVertexStream::VERXT_INSTANCE6)
			};
			instanceStream->add(vdt, 7);

			std::vector<TextDrawInstance> drawInstances;

			TextDrawInstance tdi;
			RMatrix transeMatrix;

			for (int i = 0; i < size; i++) {
				transeMatrix.makeIdentity();

				const PreInstancData& data = datas[i];

				Real3 transe = data.matrix.getTrans();
				transeMatrix.makeTranslate(transe);

				tdi.mvp = (vp * transeMatrix * rotateMatrix).getTranspose(); ;

				Text::TextInstance* text_data = (Text::TextInstance*)data.userData;
				Text* text = text_data->text;

				tdi.color = text->getColor();
				tdi.textureCoord = text_data->textureCoord;
				tdi.position = text_data->position;

				drawInstances.push_back(tdi);
			}
			instanceStream->addVerxtxData(&drawInstances.front(), (uint32)(drawInstances.size()) * sizeof(TextDrawInstance));
			instanceStream->setSize((uint32)drawInstances.size());
		};

		IRenderTechnique* renderTechnique = RenderFactory::s_factory->createRenderTechnique();
		renderTechnique->setRenderScript(rps);

		return renderTechnique;
	}
	IRenderTechnique* g_text_renderTechnique = Text::renderTextSchema();

	Text::Text()
	{
		_font = new DefaultFont();
	}

	const RBoundingBox& Text::getBoudingBox()
	{
		return _aabb;
	}

	uint32 Text::getNumGeometry()
	{
		return 0U;
	}

	IGeometry* Text::getGeometry(uint32 index)
	{
		return nullptr;
	}

	IMaterial* Text::getMaterial(uint32 index)
	{
		return nullptr;
	}

	void Text::accept(VisitGeometryVertex dv)
	{
	}

	void Text::accept(VisitGeometry vg)
	{
		resetGeometries();

		if (_text.empty())
			return;

		int32 index = 0;
		FontResolution resolution = { _fontHeight ,_fontHeight };

		for (uint32 i = 0; i < _text.size(); i++)
		{
			const char& byte = _text[i];
			if (byte == '\n')
				continue;

			Glyph* glph = _font->getGlyph(resolution, byte);
			glph->getTexture()->applySubGlyph();

			IMaterial* material = glph->getTexture()->getMaterial();
			material->setRenderTechnique(g_text_renderTechnique);

			vg(s_text_geometry, material, &(_char_instances[index++]));
		}
	}

	void Text::setFont(const char* path)
	{
		_font = FreeTypeLibrary::instance()->getFont(path);
		_reset = true;
	}

	void Text::setText(const char* text)
	{
		_text = text;
		_reset = true;
	}

	void Text::setColor(const Real4& color)
	{
		_color = color;
		_reset = true;
	}

	const Real4& Text::getColor()
	{
		return _color;
	}

	void Text::setCharacterSizeMode(CharacterSizeMode mode)
	{
		_characterSizeMode = mode;
		_reset = true;
	}

	IText::CharacterSizeMode Text::getCharacterSizeMode()
	{
		return _characterSizeMode;
	}

	void Text::setCharacterSize(Real size)
	{
		_characterSize = size;
		_reset = true;
	}

	void Text::setCharacterSize(Real size, Real aspectRatio)
	{
		_characterSize = size;
		_aspectRatio = aspectRatio;
		_reset = true;
	}

	RBoundingBox Text::computeBound()
	{
		return RBoundingBox();
	}

	void Text::resetGeometries()
	{
		if (!_reset)
			return;

		_reset = false;
		_char_instances.clear();

		int row = 0;
		int col = 0;
		size_t lg = strlen(_text.c_str());

		TextInstance instance;
		instance.text = this;
		FontResolution resolution = { _fontHeight ,_fontHeight };

		for (size_t i = 0; i < lg; i++) {
			const char& byte = _text[i];
			if (byte == '\n') {
				row++;
				col = 0;
				continue;
			}
			Glyph* glph = _font->getGlyph(resolution, byte);
			Vec2f mintc = glph->getMinTexCoord();
			Vec2f maxtc = glph->getMaxTexCoord();
			Vec2f vDiff = maxtc - mintc;

			instance.position = Vec3f(col * _characterSize, row * _characterSize, row * _characterSize);
			instance.textureCoord = Vec4f(mintc.x(), mintc.y(), vDiff.x(), vDiff.y());
			_char_instances.push_back(instance);

			col++;
		}
	}
}