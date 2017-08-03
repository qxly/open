#ifndef SCENE_SCENEFACTORYINTERFACE_H
#define SCENE_SCENEFACTORYINTERFACE_H


namespace open
{
	class INode;
	class IMesh;
	class ISolidGeometry;
	class IDrawable;
	class ICamera;
	class ICommonDrawable;
	class ISceneManager;
	class IText;
	class Config;
	class COMMON_API SceneFactory
	{
	public:
		static SceneFactory* s_factory;

		virtual IMesh* createMesh() = 0;

		virtual ISolidGeometry* createSolidGeometry() = 0;

		virtual ICommonDrawable* createCommonDrawable() = 0;

		virtual IText* createText() = 0;

		virtual IDrawable* createDrawable(const char* file, const Config& config) = 0;

		virtual ICamera* createCamera() = 0;

		virtual INode* createNode() = 0;

		virtual ISceneManager* createSceneManager() = 0;
	};
}

#endif