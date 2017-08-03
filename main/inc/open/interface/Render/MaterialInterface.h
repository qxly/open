#ifndef RENDER_MATERIALINTERFACE_H
#define RENDER_MATERIALINTERFACE_H 1

#include <functional>
#include <open/Maths.h>

namespace open
{
	class IProgram;
	class IVertexStream;
	class IRenderPool;
	class IRenderTarget;
	struct RenderStateSet;
	class ITextureUnitSet;
	class IMaterial;
	class ICamera;

	struct Value
	{
		uint8 data[64];

		inline const int32& toInt32() const { return *((int*)(data)); }
		inline const Real& toReal() const { return *((Real*)(data)); }
		inline const Real2& toReal2() const { return *((Real2*)(data)); }
		inline const Real3& toReal3() const { return *((Real3*)(data)); }
		inline const Real4& toReal4() const { return *((Real4*)(data)); }
		inline const RMatrix& toReal16() const { return *((RMatrix*)(data)); }

		inline void set(int32 v) { *((int*)(data)) = v; }
		inline void set(Real v) { *((Real*)(data)) = v; }
		inline void set(const Real2& v) { *((Real2*)(data)) = v; }
		inline void set(const Real3& v) { *((Real3*)(data)) = v; }
		inline void set(const Real4& v) { *((Real4*)(data)) = v; }
		inline void set(const RMatrix& v) { *((RMatrix*)(data)) = v; }
	};

	struct PreInstancData
	{
		RMatrix matrix;
		void* userData;
	};

	struct Shader
	{
		const char* vert = nullptr;
		const char* frag = nullptr;
		const char* define = nullptr;

		Shader() = default;

		inline Shader(int) {}

		inline Shader(const char* v, const char* f, const char* d = nullptr)
			:vert(v), frag(f), define(d) {}

		bool isNull() { return !vert && !frag; }
	};

	struct OutFrag
	{
		const char* name = nullptr;
		uint8 fragPositon = 0;

		OutFrag() = default;

		inline OutFrag(int) {}

		inline OutFrag(const char* n, uint8 p)
			:name(n), fragPositon(p) {}

		bool isNull() { return !name; }
	};

	struct UniformDetial
	{
		typedef std::function<void(void*, IMaterial*, IRenderPool*, Value&)> UnifromHandle;

		const char* name = nullptr;
		int type;
		UnifromHandle uniformHandle;

		UniformDetial() = default;

		inline UniformDetial(int) {}

		inline UniformDetial(const char* n, int t, UnifromHandle handle)
			:name(n), type(t), uniformHandle(handle) {}

		inline bool isNull() const { return !name && !uniformHandle; }
	};

	struct RenderScript
	{
		typedef std::function<void(IRenderPool*, IVertexStream*, const PreInstancData*, int)> GenInstanceHandle;
		typedef std::function<void*(void)> BeginHandle;
		typedef std::function<void(void*)> EndHandle;

		const char* name = nullptr;
		const Shader* shader = nullptr;
		const OutFrag* outFrags = nullptr;
		const UniformDetial* uniformDetials = nullptr;

		GenInstanceHandle genInstance;
		BeginHandle begin;
		EndHandle end;
	};

	class IRenderTechnique
	{
	public:
		virtual void setCamera(ICamera* camera) = 0;

		virtual ICamera* getCamera() = 0;

		virtual void setRenderTarget(IRenderTarget* renderTarget) = 0;

		virtual IRenderTarget* getRenderTarget() = 0;

		virtual void setRenderScript(const RenderScript& renderScript) = 0;

		virtual const RenderScript& getRenderScript() = 0;

		virtual IProgram* getProgram() = 0;
	};

	class IMaterial
	{
	public:

		virtual RenderStateSet& getRenderStaeSet() = 0;

		virtual ITextureUnitSet* getTextureUnitSet() = 0;

		virtual void setRenderTechnique(IRenderTechnique* technique) = 0;

		virtual IRenderTechnique* getRenderTechnique() = 0;

		virtual Value& getRenderParameter(const char* name) = 0;

		virtual IMaterial* clone() = 0;

		virtual int32 addRef() = 0;

		virtual int32 release() = 0;

		virtual void dispose() = 0;
	};
}

#endif