#ifndef RENDER_RENDERABLEINTERFACE_H
#define RENDER_RENDERABLEINTERFACE_H 1

#include <functional>
#include <open/Maths.h>
#include <open/openDef.h>

namespace open
{
	enum DrawMode : uint8;
	class IGPUBuffer;
	class IVertexStream;
	class IVertexArrayObject;
	class IRenderPool;
	class IMaterial;

	struct GeometryVertex
	{
		DrawMode drawMode;
		Vec3f* vertexs;
		uint32 size;

		int32 indexType;
		union
		{
			struct
			{
				void* indexs;
				uint32 size;
			}drawElements;

			struct
			{
				uint32 start;
				uint32 count;
			}drawArray;
		}indexs;
	};

	class IBone;
	class IGeometry;
	typedef std::function<void(const GeometryVertex&, IGeometry*)> VisitGeometryVertex;
	typedef std::function<void(IGeometry*, IMaterial*, void*)> VisitGeometry;

	class IDrawableBase;
	class IText;

	class IDrawable
	{
	public:

		virtual ~IDrawable() {}

		virtual int32 addRef() = 0;
		virtual int32 release() = 0;
		virtual void dispose() = 0;

		virtual uint32 getNumGeometry() = 0;
		virtual IGeometry* getGeometry(uint32 index) = 0;
		virtual IMaterial* getMaterial(uint32 index) = 0;

		virtual const RBoundingBox& getBoudingBox() = 0;

		virtual void accept(VisitGeometryVertex dv) = 0;

		virtual void accept(VisitGeometry vg) = 0;

		virtual IDrawableBase* asDrawableBase() { return nullptr; }
		virtual IText* asText() { return nullptr; }
	};

	class IDrawableBase : public IDrawable
	{
	public:
		virtual void setBone(IBone* bone) = 0;
		virtual IBone* getBone() = 0;

		virtual void addGeometry(IGeometry* geometry, IMaterial* material) = 0;
		virtual void setMaterial(uint32 index, IMaterial* material) = 0;

		virtual void removeGeometry(uint32 index) = 0;

		virtual IDrawableBase* asDrawableBase() { return this; }
	};

	class IText : public IDrawable
	{
	public:
		enum CharacterSizeMode : uint8
		{
			OBJECT_COORDS,
			SCREEN_COORDS,
			OBJECT_COORDS_WITH_MAXIMUM_SCREEN_SIZE_CAPPED_BY_FONT_HEIGHT
		};

		enum AlignmentType : uint8
		{
			LEFT_TOP,
			LEFT_CENTER,
			LEFT_BOTTOM,

			CENTER_TOP,
			CENTER_CENTER,
			CENTER_BOTTOM,

			RIGHT_TOP,
			RIGHT_CENTER,
			RIGHT_BOTTOM,

			LEFT_BASE_LINE,
			CENTER_BASE_LINE,
			RIGHT_BASE_LINE,
		};

		enum Layout : uint8
		{
			LEFT_TO_RIGHT,
			RIGHT_TO_LEFT,
			VERTICAL
		};

		virtual void setFont(const char* path) = 0;
		virtual void setText(const char* text) = 0;

		virtual void setColor(const Real4& color) = 0;
		virtual const Real4& getColor() = 0;

		virtual void setFontSize(uint32 size) = 0;
		virtual uint32 getFontSize() = 0;

		virtual void setCharacterSizeMode(CharacterSizeMode mode) = 0;
		virtual CharacterSizeMode getCharacterSizeMode() = 0;

		virtual void setCharacterSize(Real height) = 0;
		virtual void setCharacterSize(Real height, Real aspectRatio) = 0;

		virtual Real getCharacterSize() = 0;
	};

	class IGeometry
	{
	public:

		virtual ~IGeometry() {}

		virtual int32 addRef() = 0;

		virtual int32 release() = 0;

		virtual void dispose() = 0;

		virtual const RBoundingBox& getBoudingBox() = 0;

		virtual void draw(IRenderPool* renderPool) = 0;

		virtual void draw(IRenderPool* renderPool, IVertexStream* instanceStream) = 0;

		virtual void accept(VisitGeometryVertex& dv) = 0;
	};

	class IBone
	{
	public:

		virtual ~IBone() {}

		virtual int32 addRef() = 0;

		virtual int32 release() = 0;

		virtual void dispose() = 0;

		virtual uint8 getID() = 0;

		virtual IBone* createChild() = 0;

		virtual IBone* getParent() = 0;

		virtual void setBoneMatrix(const RMatrix& materix) = 0;

		virtual const RMatrix& getBoneMatrix() = 0;

		virtual const RMatrix& getMatrix() = 0;
	};

	struct Skin
	{
		uint8 bone[4];
		uint8 boneWidth[4];
	};

	class IMesh : public IGeometry
	{
	public:

		////顶点
		virtual void begin(uint32 vertexNum) = 0;

		virtual void setVertexArray(Vec3f* vertexs) = 0;

		virtual void setNormalArray(Vec3f* normals) = 0;

		virtual void setSkinArray(Skin* skins) = 0;

		virtual void setTextureCoordArray(uint32 index, Vec2f* coords) = 0;

		virtual void end() = 0;

		///包围盒
		virtual void autoBoundingBox(bool value) = 0;

		virtual bool isAutoBoundingBox() = 0;

		virtual void setBoudingBox(const RBoundingBox& box) = 0;

		////顶点索引
		virtual int32 getNumIndexArray() = 0;

		virtual void addIndexArray(DrawMode mode, uint8* indexs, uint32 size) = 0;

		virtual void addIndexArray(DrawMode mode, uint16* indexs, uint32 size) = 0;

		virtual void addIndexArray(DrawMode mode, uint32* indexs, uint32 size) = 0;

		virtual void addIndexArray(DrawMode mode, uint32 first, uint32 count) = 0;

		virtual void setIndexArray(uint32 index, DrawMode mode, uint8* indexs, uint32 size) = 0;

		virtual void setIndexArray(uint32 index, DrawMode mode, uint16* indexs, uint32 size) = 0;

		virtual void setIndexArray(uint32 index, DrawMode mode, uint32* indexs, uint32 size) = 0;

		virtual void setIndexArray(uint32 index, DrawMode mode, uint32 first, uint32 count) = 0;
	};

	struct SolidShape
	{
		enum ShapeType : uint8
		{
			TYPE_BOX,
			TYPE_SPHERE,
			TYPE_PLANE,
			TYPE_FRUSTUM,
			TYPE_HEIGHTMAP
		};

		union Shape
		{
			struct Box
			{
				Real center[3];
				Real length;
				Real width;
				Real height;
			} box;

			struct Sphere
			{
				Real center[3];
				Real radius;
			} sphere;

			struct Plane
			{
				Real center[3];
				Real normal[3];
				Real width;
				Real height;
			} plane;

			struct HeightMap
			{
				uint32 columns;
				uint32 rows;

				Real origin[3];
				Real dx;
				Real dy;

				Real* heights;
			} heightMap;
		} shape;
		ShapeType type;
	};

	class ISolidGeometry : public IGeometry
	{
	public:
		virtual void setSoildShape(const SolidShape& solid) = 0;

		virtual const SolidShape& getSolidShape() = 0;
	};
}

#endif