#ifndef RENDERSYSTEM_MATERIAL_INTERFACE_H
#define RENDERSYSTEM_MATERIAL_INTERFACE_H

#include <open/Maths.h>
#include <open/openDef.h>

namespace open
{
	class TextureUnitSet;
	enum ShaderType : uint16;
	enum DataType : uint16;
	enum PixelInternalFormat : uint16;
	enum PixelFormat : uint16;
	enum TextureWrapParameter : uint8;
	enum TextureWrapMode : uint16;
	enum TextureFilterParameter : uint8;
	enum TextureFilterMode : uint16;

	class IProgram
	{
	public:

		virtual ~IProgram() {}

		virtual bool loadFromFile(const char* vs_filePath, const char* ps_filePath, const char* defines = nullptr, const char* version = "#version 330\n") = 0;

		virtual bool loadProgramFromBuffer(const char* vs, const char* ps, const char* defines = nullptr, const char* version = "#version 330\n") = 0;

		virtual void setFragDataLocation(int32 index, const char* outName) = 0;

		virtual int32 getUniformID(const char* uniformName) = 0;

		virtual void setUniformi(int32 id, int32 value) = 0;

		virtual void setUniformf(int32 id, Real value) = 0;

		virtual void setUniform2f(int32 id, const Real2& value) = 0;

		virtual void setUniform3f(int32 id, const Real3& value) = 0;

		virtual void setUniform4f(int32 id, const Real4& value) = 0;

		virtual void setUniform16f(int32 id, const RMatrix& value) = 0;

		virtual void setUniformi(const char* uniformName, int32 value) = 0;

		virtual void setUniformf(const char* uniformName, Real value) = 0;

		virtual void setUniform2f(const char* uniformName, const Real2& value) = 0;

		virtual void setUniform3f(const char* uniformName, const Real3& value) = 0;

		virtual void setUniform4f(const char* uniformName, const Real4& value) = 0;

		virtual void setUniform16f(const char* uniformName, const RMatrix& value) = 0;

#if DOUBLE_PRECISION

		virtual void setUniformf(int32 id, float value) = 0;

		virtual void setUniform2f(int32 id, const Vec2f& value) = 0;

		virtual void setUniform3f(int32 id, const Vec3f& value) = 0;

		virtual void setUniform4f(int32 id, const Vec4f& value) = 0;

		virtual void setUniform16f(int32 id, const Matrixf& value) = 0;

		virtual void setUniformf(const char* uniformName, float value) = 0;

		virtual void setUniform2f(const char* uniformName, const Vec2f& value) = 0;

		virtual void setUniform3f(const char* uniformName, const Vec3f& value) = 0;

		virtual void setUniform4f(const char* uniformName, const Vec4f& value) = 0;

		virtual void setUniform16f(const char* uniformName, const Matrixf& value) = 0;

#endif

		//virtual void setSubroutine(ShaderType shaderType, int32 count, const char* funName, ...) = 0;
	};

	struct Uniform
	{
	public:

		Uniform() = delete;

		Uniform(const Uniform&) = delete;

		void operator = (const Uniform&) = delete;

		Uniform(const char* name, IProgram* program)
			:_program(program)
		{
			id = _program->getUniformID(name);
		}
		inline void operator = (const float& t)
		{
			_program->setUniformf(id, t);
		}

		inline void operator = (const int32& t)
		{
			_program->setUniformi(id, t);
		}

		inline void operator = (const Real2& t)
		{
			_program->setUniform2f(id, t);
		}

		inline void  operator = (const Real3& t)
		{
			_program->setUniform3f(id, t);
		}

		inline void  operator = (const Real4& t)
		{
			_program->setUniform4f(id, t);
		}

		inline void  operator = (const RMatrix& t)
		{
			_program->setUniform16f(id, t);
		}

	private:
		IProgram* _program;
		int32 id;
	};

	class ITexture
	{
	public:

		virtual ~ITexture() {}

		virtual int32 addRef() = 0;

		virtual int32 release() = 0;

		virtual void releaseRefCountOnly() = 0;

		virtual void dispose() = 0;

		virtual int32 getWidth() = 0;

		virtual int32 getHeight() = 0;

		virtual int32 getDepth() = 0;

		virtual DataType getDataType() = 0;

		virtual PixelInternalFormat getPixelInternalFormat() = 0;

		virtual PixelFormat getPixeFormat() = 0;

		virtual void alloc(PixelInternalFormat internalFormat, int32 width, int32 height, int32 depth,
			PixelFormat pixeformat, DataType dataType, void* data, int32 packing = 1) = 0;

		virtual void subLoad(int32 startX, int32 startY, int32 width, int32 height, void* data) = 0;

		virtual bool bind(uint32 unit) = 0;

		virtual bool apply() = 0;

		virtual Handle getHandle() = 0;
	};

	class ITextureUnit
	{
	public:

		virtual ~ITextureUnit() {}

		virtual int32 addRef() = 0;

		virtual int32 release() = 0;

		virtual void releaseRefCountOnly() = 0;

		virtual void dispose() = 0;

		virtual void setTexture(ITexture* texture) = 0;

		virtual void setWrap(TextureWrapParameter which, TextureWrapMode wrap) = 0;

		virtual void setFilter(TextureFilterParameter which, TextureFilterMode mode) = 0;

		virtual void setBorderColor(const Real4& color) = 0;

		virtual void setMatrix(const RMatrix& matrix) = 0;

		virtual void enable(bool value) = 0;

		virtual bool getEnable() = 0;

		virtual ITexture* getTexture() = 0;

		virtual TextureWrapMode getWrap(TextureWrapParameter which) = 0;

		virtual TextureFilterMode getFilter(TextureFilterParameter which) = 0;

		virtual const Real4& getBorderColor() = 0;

		virtual const RMatrix& getMatrix() = 0;
	};

	class ITextureUnitSet
	{
	public:

		virtual ~ITextureUnitSet() {}

		virtual ITextureUnit* createTextureUnit(uint8 unitIndex, const char* name = nullptr) = 0;

		virtual ITextureUnit* getTextureUnit(uint8 unitIndex) = 0;

		virtual bool hasTextureUnit(uint8 unitIndex) = 0;

		virtual void setTextureUnitNam(uint8 unit, const char* name) = 0;

		virtual const char* getTextureUnitNam(uint8 unit) = 0;

		virtual void replace(uint8 unitIndex, ITextureUnit* unit) = 0;

		virtual int32 getVaildUnitNum() = 0;
	};

	enum StateMode : uint16;
	enum Face : uint16;
	enum FrontFace : uint16;
	enum PolygonMode : uint16;
	enum CompareFunction : uint16;
	enum StencilOperation : uint16;
	enum BlendEquation : uint16;
	enum BlendFun : uint16;

	struct COMMON_API RenderStateSet
	{
		RenderStateSet();

		///各个状态的开关
		void setMode(StateMode m, bool value);

		bool getMode(StateMode m) const;

		///colorMask相关
		void setColorMask(uint8 colorMask);

		///背面剔除相关
		void setCullFace(Face face);

		void setFrontFace(FrontFace frontFace);

		///填充模式
		void setPolygonMode(Face face, PolygonMode mode);

		///深度相关
		void setDepthMask(bool value);

		void setDepthFunction(CompareFunction func);

		////模板缓存相关
		void setStencilFunction(CompareFunction fun);

		void setStencilFunRef(uint32 ref);

		void setStencilFunMask(uint32 mask);

		void setStencilMask(uint32 mask);

		void setStencilOperation(StencilOperation sfail, StencilOperation zfail, StencilOperation zpass, Face face);

		////光照相关
		void setShininess(int32 shininess);
		int32 getShininess();

		uint32 getAmbientColor();
		void setAmbientColor(uint32 color);

		uint32 getDiffuseColor();
		void setDiffuseColor(uint32 color);

		uint32 getEmissiveColor();
		void setEmissiveColor(uint32 color);

		///混合相关
		void setBlendEquationSeparate(BlendEquation rgb, BlendEquation alpha);

		void setBlendFunSeparate(BlendFun srcRGB, BlendFun destRGB, BlendFun srcAlpha, BlendFun destAlpha);

		bool isTransparence();

		bool mode[5];
		uint8 colorMask;

		Face cullFace;
		FrontFace cullFrontFace;

		PolygonMode polygonMode;
		Face polygonModeFace;

		bool depthMask;
		CompareFunction depthFunction;

		CompareFunction stencilFunction;
		uint32 stencilRef;
		uint32 stencilFunMask;
		uint32 stencilMask;

		StencilOperation sfail_front;
		StencilOperation zfail_front;
		StencilOperation zpass_front;

		StencilOperation sfail_back;
		StencilOperation zfail_back;
		StencilOperation zpass_back;

		BlendEquation rgbBlendEquation;
		BlendEquation alphaBlendEquation;
		BlendFun srcRGB;
		BlendFun srcAlpha;
		BlendFun destRGB;
		BlendFun destAlpha;

		uint8 shininess;
		uint32 diffuseColor;
		uint32 emissiveColor;
		uint32 ambientColor;
	};
}

#endif