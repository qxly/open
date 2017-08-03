#ifndef RENDER_FORWARDRENDER_H
#define RENDER_FORWARDRENDER_H 1

namespace open
{
	class ILight;
	class IMaterial;
	class IRenderPool;

	class ForwardShader
	{
		class DefaultRenderScheme;
	public:

		ForwardShader();

		void begin(IRenderPool* renderPool);

		void apply(ILight* light, IMaterial* material);

		IApplyMaterialFun& getApplyMaterialFun();

	private:
		DefaultRenderScheme* _renderScheme;
	};
}

#endif