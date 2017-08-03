#ifndef SCENE_READWRITE_OBJ_H
#define SCENE_READWRITE_OBJ_H

#include <string>
#include <vector>
#include <map>
#include <istream>

#include <open/Maths.h>
#include <open/Ptr.h>
#include <open/Memory.h>

namespace open
{
	class ObjMaterial
	{
	public:
		ObjMaterial() :
			ambient(0.2f, 0.2f, 0.2f),
			diffuse(0.8f, 0.8f, 0.8f),
			specular(0.0f, 0.0f, 0.0f),
			emissive(0.0f, 0.0f, 0.0f),
			sharpness(0.0f),
			illum(2),
			Tf(0.0f, 0.0f, 0.0f, 1.0f),
			Ni(0),
			Ns(0),
			// textureReflection(false),
			alpha(1.0f) {}

		std::string name;

		Real3   ambient;
		Real3   diffuse;
		Real3   specular;
		Real3   emissive;
		Real       sharpness;
		int32         illum;            // read but not implemented (would need specific shaders or state manipulation)

		Real4   Tf;
		int32         Ni;
		int32         Ns; // shininess 0..1000

		// bool        textureReflection;
		Real       alpha;

		class Map
		{
			// -o and -s (offset and scale) options supported for the maps
			// -clamp <on|off> is supported
			// -blendu, -blendv, -imfchan, not supported
			// -mm <base> <gain> is parsed but not actually used
			// -bm <bump_multiplier> is parsed but not used
		public:
			enum TextureMapType
			{
				DIFFUSE_LAYER,
				EMISSIVE_LAYER,
				OPACITY_LAYER,
				BUMP_LAYER,

				UNKOWN_LAYER1,
				UNKOWN_LAYER2,

				SPECULAR,
				SPECULAR_EXPONENT,
				DISPLACEMENT,
				REFLECTION,        // read of a reflection map will also apply spherical texgen coordinates

				UNKNOWN_NULL
			};
			Map() :
				type(DIFFUSE_LAYER),
				name(""),
				uScale(1.0f),
				vScale(1.0f),
				uOffset(0.0f),
				vOffset(0.0f),
				clamp(false) {}


			TextureMapType type;
			std::string name;

			// Texture scale and offset, used for creating the texture matrix.
			// Reader only picks u and v from -s u v w, although all u v and w all need to be specified!
			// e.g. "map_Kd -s u v w <name>" is OK but "map_Kd -s u v <name>" is not, even though tex is only 2D
			Real       uScale;
			Real       vScale;
			Real       uOffset;
			Real       vOffset;

			// According to the spec, if clamping is off (default), the effect is a texture repeat
			// if clamping is on, then the effect is a decal texture; i.e. the border is transparent
			bool        clamp;
		};

		std::vector<Map> maps;

	protected:
	};

	struct B_E
	{
	};

	class Element : public TPtr<B_E>
	{
	public:

		typedef std::vector<int> IndexList;

		enum DataType
		{
			POINTS,
			POLYLINE,
			POLYGON
		};

		Element(DataType type) :
			dataType(type){}

		enum CoordinateCombination
		{
			VERTICES,
			VERTICES_NORMALS,
			VERTICES_TEXCOORDS,
			VERTICES_NORMALS_TEXCOORDS
		};

		CoordinateCombination getCoordinateCombination() const
		{
			if (vertexIndices.size() == normalIndices.size())
				return (vertexIndices.size() == texCoordIndices.size()) ? VERTICES_NORMALS_TEXCOORDS : VERTICES_NORMALS;
			else
				return (vertexIndices.size() == texCoordIndices.size()) ? VERTICES_TEXCOORDS : VERTICES;
		}

		DataType  dataType;
		IndexList vertexIndices;
		IndexList normalIndices;
		IndexList texCoordIndices;
	};

	class ElementState
	{
	public:

		ElementState() :
			coordinateCombination(Element::VERTICES),
			smoothingGroup(0) {}

		bool operator < (const ElementState& rhs) const
		{
			if (materialName<rhs.materialName)
				return true;
			else if (rhs.materialName<materialName)
				return false;

			if (objectName<rhs.objectName)
				return true;
			else if (rhs.objectName<objectName)
				return false;

			if (groupName<rhs.groupName)
				return true;
			else if (rhs.groupName<groupName)
				return false;

			if (coordinateCombination<rhs.coordinateCombination)
				return true;
			else if (rhs.coordinateCombination<coordinateCombination)
				return false;

			return (smoothingGroup<rhs.smoothingGroup);
		}


		std::string                     objectName;
		std::string                     groupName;
		std::string                     materialName;
		Element::CoordinateCombination  coordinateCombination;
		int                             smoothingGroup;
	};

	class Model
	{
	public:
		Model() :
			currentElementList(0) {}

		void setDatabasePath(const std::string& path) { databasePath = path; }
		const std::string& getDatabasePath() const { return databasePath; }

		std::string lastComponent(const char* linep);
		bool readMTL(std::istream& fin);
		bool readOBJ(std::istream& fin);

		bool readline(std::istream& fin, char* line, const int LINE_SIZE);
		void addElement(Element* element);

		Real3 averageNormal(const Element& element) const;
		Real3 computeNormal(const Element& element) const;
		bool needReverse(const Element& element) const;

		std::size_t remapVertexIndex(int32 vi) { return (vi<0) ? vertices.size() + vi : vi - 1; }
		std::size_t remapNormalIndex(int32 vi) { return (vi<0) ? normals.size() + vi : vi - 1; }
		std::size_t remapTexCoordIndex(int32 vi) { return (vi<0) ? texcoords.size() + vi : vi - 1; }

		typedef std::map<std::string, ObjMaterial>          MaterialMap;
		typedef std::vector< Vec2f >                Vec2Array;
		typedef std::vector< Vec3f >                Vec3Array;
		typedef std::vector< Ptr<Element> >    ElementList;
		typedef std::map< ElementState, ElementList >    ElementStateMap;


		std::string     databasePath;
		MaterialMap     materialMap;

		Vec3Array       vertices;
		Vec3Array       normals;
		Vec2Array       texcoords;

		ElementState    currentElementState;

		ElementStateMap elementStateMap;
		ElementList*    currentElementList;
	};

}

#endif
