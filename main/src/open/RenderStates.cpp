#include <open/interface/RenderSystem/RenderStatesInterface.h>
#include <open/interface/RenderSystem/RenderEnumeration.h>

#include <assert.h>

namespace open
{
	struct DefalutRenderStates
	{
		bool mode[5] = { true,false,false,true,true };
		uint8 colorMask = COLORMASK_ALL;

		Face cullFace = FACE_BACK;
		FrontFace cullFrontFace = FRONTFACE_CCW;

		PolygonMode polygonMode = POLYGONMODE_FILL;
		Face polygonModeFace = FACE_FRONT_AND_BACK;

		bool depthMask = true;
		CompareFunction depthFunction = LEQUAL;

		CompareFunction stencilFunction = ALWAYS;
		uint32 stencilRef = 0;
		uint32 stencilFunMask = ~0u;
		uint32 stencilMask = ~0u;

		StencilOperation sfail_front = STENCILOPERATION_KEEP;
		StencilOperation zfail_front = STENCILOPERATION_KEEP;
		StencilOperation zpass_front = STENCILOPERATION_KEEP;

		StencilOperation sfail_back = STENCILOPERATION_KEEP;
		StencilOperation zfail_back = STENCILOPERATION_KEEP;
		StencilOperation zpass_back = STENCILOPERATION_KEEP;

		BlendEquation rgbBlendEquation = BLENDEQUATION_ADD;
		BlendEquation alphaBlendEquation = BLENDEQUATION_ADD;
		BlendFun srcRGB = BLENDFUN_SRC_ALPHA;
		BlendFun srcAlpha = BLENDFUN_SRC_ALPHA;
		BlendFun destRGB = BLENDFUN_ONE_MINUS_SRC_ALPHA;
		BlendFun destAlpha = BLENDFUN_ONE_MINUS_SRC_ALPHA;

		uint8 shininess = 128;

		uint32 diffuseColor = Real3(0, 0,0).asRGB();
		uint32 emissiveColor = Real3(0, 0, 0).asRGB();
		uint32 ambientColor = Real3(0.2f, 0.2f, 0.2f).asRGB();
	};
	static const DefalutRenderStates s_defalut_s;

	RenderStateSet::RenderStateSet()
	{
		static_assert(sizeof(RenderStateSet) == sizeof(DefalutRenderStates),"RenderStateSet");
		memcpy(this, &s_defalut_s, sizeof(DefalutRenderStates));
	}

	///各个状态的开关
	inline void RenderStateSet::setMode(StateMode m, bool value)
	{
		switch (m)
		{
		case DEPTH_TEST:
			mode[0] = value;
			break;
		case STENCIL_TEST:
			mode[1] = value;
			break;
		case BLEND:
			mode[2] = value;
			break;
		case CULL_FACE:
			mode[3] = value;
			break;
		case LIGHT:
			mode[4] = value;
			break;
		}
	}

	inline bool RenderStateSet::getMode(StateMode m) const
	{
		switch (m)
		{
		case DEPTH_TEST:
			return mode[0];
		case STENCIL_TEST:
			return mode[1];
		case BLEND:
			return mode[2];
		case CULL_FACE:
			return mode[3];
		case LIGHT:
			return mode[4];
		}
		return false;
	}

	///colorMask相关
	inline void RenderStateSet::setColorMask(uint8 colorMask) { colorMask = colorMask; }

	///背面剔除相关
	inline void RenderStateSet::setCullFace(Face face) { cullFace = face; }

	inline void RenderStateSet::setFrontFace(FrontFace frontFace) { cullFrontFace = frontFace; }

	///填充模式
	inline void RenderStateSet::setPolygonMode(Face face, PolygonMode mode) { polygonMode = mode; polygonModeFace = face; }

	///深度相关
	inline void RenderStateSet::setDepthMask(bool value) { depthMask = value; }

	inline void RenderStateSet::setDepthFunction(CompareFunction func) { depthFunction = func; }

	////模板缓存相关
	inline void RenderStateSet::setStencilFunction(CompareFunction fun) { stencilFunction = fun; }

	inline void RenderStateSet::setStencilFunRef(uint32 ref) { stencilRef = ref; }

	inline void RenderStateSet::setStencilFunMask(uint32 mask) { stencilFunMask = mask; }

	inline void RenderStateSet::setStencilMask(uint32 mask) { stencilMask = mask; }

	inline void RenderStateSet::setStencilOperation(StencilOperation sfail, StencilOperation zfail, StencilOperation zpass, Face face)
	{
		switch (face)
		{
		case FACE_FRONT:
			sfail_front = sfail; zfail_front = zfail; zpass_front = zpass;
			break;
		case FACE_BACK:
			sfail_back = sfail; zfail_back = zfail; zpass_back = zpass;
			break;
		case FACE_FRONT_AND_BACK:
			sfail_front = sfail; zfail_front = zfail; zpass_front = zpass;
			sfail_back = sfail; zfail_back = zfail; zpass_back = zpass;
			break;
		}
	}

	////光照相关
	inline void RenderStateSet::setShininess(int32 shininess) { shininess = shininess; }
	inline int32 RenderStateSet::getShininess() { return shininess; }

	inline uint32 RenderStateSet::getAmbientColor() { return ambientColor; }
	inline void RenderStateSet::setAmbientColor(uint32 color) { ambientColor = color; }

	inline uint32 RenderStateSet::getDiffuseColor() { return diffuseColor; }
	inline void RenderStateSet::setDiffuseColor(uint32 color) { diffuseColor = color; }

	inline uint32 RenderStateSet::getEmissiveColor() { return emissiveColor; }
	inline void RenderStateSet::setEmissiveColor(uint32 color) { emissiveColor = color; }

	///混合相关
	inline void RenderStateSet::setBlendEquationSeparate(BlendEquation rgb, BlendEquation alpha)
	{
		rgbBlendEquation = rgb; alphaBlendEquation = alpha;
	}

	inline void RenderStateSet::setBlendFunSeparate(BlendFun sr, BlendFun dr, BlendFun sa, BlendFun da)
	{
		srcRGB = sr;
		srcAlpha = sa; 
		destRGB = dr; 
		destAlpha = da;
	}

	inline bool RenderStateSet::isTransparence() { return getMode(BLEND); }
}