#include "obj.h"

#include <RenderSystem/RenderEnumeration.h>
#include <RenderSystem/RenderStatesInterface.h>
#include <RenderSystem/RenderSystemFactoryInterface.h>

#include <Render/DrawableInterface.h>
#include <Render/MaterialInterface.h>
#include <Render/RenderFactoryInterface.h>

#include "../../Mesh/Drawable.h"

#include <sstream>
#include <open/Array.h>
#include <open/Config.h>

namespace open
{
	class ReaderWriterOBJ : public DrawableReadWriteHelper
	{
	public:

		virtual const char* support() const { return "obj"; }

		virtual IDrawable* readDrawable(const char* buffer, uint32 size, const Config& config)
		{
			Model model;
			char v[128];
			config.getStringValue("Path", "DataPath", v);

			model.setDatabasePath(v);
			std::string ss;
			ss.append(buffer, size);
			std::stringstream sss;
			sss << ss;

			model.readOBJ(sss);

			ObjOptionsStruct localOptions;
			return convertModel(model, localOptions, v);
		}

	protected:

		struct ObjOptionsStruct
		{
			bool rotate;
			bool noTesselateLargePolygons;
			bool noTriStripPolygons;
			bool generateFacetNormals;
			bool fixBlackMaterials;
			typedef std::vector< std::pair<int, ObjMaterial::Map::TextureMapType> > TextureAllocationMap;
			TextureAllocationMap textureUnitAllocation;
		};

		typedef std::map< std::string, Ptr<IMaterial> > MaterialToStateSetMap;

		inline Real3 transformVertex(const Real3& vec, const bool rotate)
		{
			if (rotate == true)
			{
				return Real3(vec.x(), -vec.z(), vec.y());
			}
			else
			{
				return vec;
			}
		}

		inline Real3 transformNormal(const Real3& vec, const bool rotate)
		{
			if (rotate == true)
			{
				return Real3(vec.x(), -vec.z(), vec.y());
			}
			else
			{
				return vec;
			}
		}

		void load_material_texture(Model &model, ObjMaterial::Map &map, IMaterial *stateset, uint32 texture_unit, const char* dataPath)
		{
			std::string filename = std::string(dataPath) + map.name;
			if (!filename.empty())
			{
				Ptr<ITexture> texture = RenderSystemFactory::s_factory->createTexture(filename.c_str());
				if (texture)
				{
					ITextureUnit* textureUnit = stateset->getTextureUnitSet()->createTextureUnit(texture_unit);
					textureUnit->setTexture(texture);
					TextureWrapMode textureWrapMode;
					if (map.clamp == true)
					{
						textureWrapMode = TextureWrapMode::CLAMP_TO_BORDER;
						textureUnit->setBorderColor(Real4(0.0, 0.0, 0.0, 0.0));
					}
					else
					{
						textureWrapMode = TextureWrapMode::REPEAT;
					}
					textureUnit->setWrap(TextureWrapParameter::WRAP_S, textureWrapMode);
					textureUnit->setWrap(TextureWrapParameter::WRAP_R, textureWrapMode);
					textureUnit->setWrap(TextureWrapParameter::WRAP_T, textureWrapMode);

					if (map.type == ObjMaterial::Map::OPACITY_LAYER)
					{
						stateset->getRenderStaeSet().setBlendFunSeparate(BLENDFUN_SRC_COLOR, BLENDFUN_ONE_MINUS_DST_COLOR, BLENDFUN_SRC_ALPHA, BLENDFUN_ONE_MINUS_DST_ALPHA);
						stateset->getRenderStaeSet().setMode(BLEND, true);
						stateset->getRenderStaeSet().setDepthMask(false);
					}
				}
			}

			///ÔÝÎ´Ö§³ÖÎÆÀí¾ØÕó
			if (map.uScale != 1.0f || map.vScale != 1.0f || map.uOffset != 0.0f || map.vOffset != 0.0f)
			{
				/*RMatrix mat;
				if (map.uScale != 1.0f || map.vScale != 1.0f)
				{
				mat *= RMatrix::scale(map.uScale, map.vScale, 1.0);
				}
				if (map.uOffset != 0.0f || map.vOffset != 0.0f)
				{
				mat *= RMatrix::translate(map.uOffset, map.vOffset, 0.0);
				}*/
			}
		}

		void buildMaterialToStateSetMap(Model& model, MaterialToStateSetMap& materialToStateSetMap, ObjOptionsStruct& localOptions, const char* dataPath)
		{
			if (localOptions.fixBlackMaterials)
			{
				// hack to fix Maya exported models that contian all black materials.
				int numBlack = 0;
				int numNotBlack = 0;
				Model::MaterialMap::iterator itr;
				for (itr = model.materialMap.begin();itr != model.materialMap.end();++itr)
				{
					ObjMaterial& material = itr->second;
					if (material.ambient == Real3(0.0f, 0.0f, 0.0f) && material.diffuse == Real3(0.0f, 0.0f, 0.0f))
					{
						++numBlack;
					}
					else
					{
						++numNotBlack;
					}
				}

				if (numNotBlack == 0 && numBlack != 0)
				{
					for (itr = model.materialMap.begin(); itr != model.materialMap.end(); ++itr)
					{
						ObjMaterial& material = itr->second;
						if (material.ambient == Real3(0.0f, 0.0f, 0.0f) && material.diffuse == Real3(0.0f, 0.0f, 0.0f))
						{
							material.ambient.set(0.3f, 0.3f, 0.3f);
							material.diffuse.set(1.0f, 1.0f, 1.0f);
						}
					}
				}
			}

			for (Model::MaterialMap::iterator itr = model.materialMap.begin(); itr != model.materialMap.end(); ++itr)
			{
				ObjMaterial& material = itr->second;

				IMaterial* material_interface = RenderFactory::s_factory->createMaterial();
				bool isTransparent = false;

				if (material.illum != 0)
				{
					//material->setAmbientColor(material.ambient);
					material_interface->getRenderStaeSet().setDiffuseColor(material.diffuse.asRGB());
					material_interface->getRenderStaeSet().setEmissiveColor(material.emissive.asRGB());

					/*if (material.illum == 2) {
					material_interface->setSpecular(material.specular);
					}
					else
					{
					material_interface->setSpecular(Real3(0, 0, 0, 1));
					}*/
					material_interface->getRenderStaeSet().setShininess(int32((material.Ns / 1000.0f) * 128)); // note OBJ shiniess is 0..1000
				}

				// If the user has explicitly set the required texture type to unit map via the options
				// string, then we load ONLY those textures that are in the map.
				if (localOptions.textureUnitAllocation.size() > 0)
				{
					for (unsigned int i = 0; i < localOptions.textureUnitAllocation.size(); i++)
					{
						// firstly, get the option set pair
						int unit = localOptions.textureUnitAllocation[i].first;
						ObjMaterial::Map::TextureMapType type = localOptions.textureUnitAllocation[i].second;
						// secondly, see if this texture type (e.g. DIFFUSE) is one of those in the material
						int index = -1;
						for (unsigned int j = 0; j < material.maps.size(); j++)
						{
							if (material.maps[j].type == type)
							{
								index = (int)j;
								break;
							}
						}
						if (index >= 0)
							load_material_texture(model, material.maps[index], material_interface, unit, dataPath);
					}
				}
				// If the user has set no options, then we load them up in the order contained in the enum. This
				// latter method is an attempt not to break user's existing code
				else
				{
					int unit = 0;
					for (int i = 0; i < (int)ObjMaterial::Map::UNKNOWN_NULL; i++) // for each type
					{
						ObjMaterial::Map::TextureMapType type = (ObjMaterial::Map::TextureMapType) i;
						// see if this texture type (e.g. DIFFUSE) is one of those in the material
						int index = -1;
						for (unsigned int j = 0; j < material.maps.size(); j++)
						{
							if (material.maps[j].type == type)
							{
								index = (int)j;
								break;
							}
						}
						if (index >= 0)
						{
							load_material_texture(model, material.maps[index], material_interface, unit, dataPath);
							unit++;
						}
					}
				}
				materialToStateSetMap[material.name] = material_interface;
			}
		}

		IMesh* convertElementListToGeometry(Model& model, Model::ElementList& elementList, ObjOptionsStruct& localOptions)
		{
			std::size_t numVertexIndices = 0;
			std::size_t numNormalIndices = 0;
			std::size_t numTexCoordIndices = 0;

			std::size_t numPointElements = 0;
			std::size_t numPolylineElements = 0;
			std::size_t numPolygonElements = 0;

			Model::ElementList::iterator itr;

			if (localOptions.generateFacetNormals == true)
			{
				for (itr = elementList.begin(); itr != elementList.end(); ++itr)
				{
					Element& element = *(*itr);
					if (element.dataType == Element::POINTS || element.dataType == Element::POLYLINE)
						continue;

					if (element.normalIndices.size() == 0)
					{
						// fill in the normals
						int a = element.vertexIndices[0];
						int b = element.vertexIndices[1];
						int c = element.vertexIndices[2];

						Real3 ab(model.vertices[b]);
						Real3 ac(model.vertices[c]);

						ab -= model.vertices[a];
						ac -= model.vertices[a];

						Vec3f Norm(ab ^ ac);
						Norm.normalize();
						std::size_t normal_idx = model.normals.size();
						model.normals.push_back(Norm);

						for (std::size_t i = 0; i < element.vertexIndices.size(); i++)
							element.normalIndices.push_back((int32)normal_idx);
					}
				}
			}

			for (itr = elementList.begin(); itr != elementList.end(); ++itr)
			{
				Element& element = *(*itr);

				numVertexIndices += element.vertexIndices.size();
				numNormalIndices += element.normalIndices.size();
				numTexCoordIndices += element.texCoordIndices.size();

				numPointElements += (element.dataType == Element::POINTS) ? 1 : 0;
				numPolylineElements += (element.dataType == Element::POLYLINE) ? 1 : 0;
				numPolygonElements += (element.dataType == Element::POLYGON) ? 1 : 0;
			}

			if (numVertexIndices == 0) return 0;

			if (numNormalIndices != 0 && numNormalIndices != numVertexIndices)
			{
				numNormalIndices = 0;
			}

			if (numTexCoordIndices != 0 && numTexCoordIndices != numVertexIndices)
			{
				numTexCoordIndices = 0;
			}

			Model::Vec3Array* vertices = numVertexIndices ? new Model::Vec3Array : nullptr;
			Model::Vec3Array* normals = numNormalIndices ? new Model::Vec3Array : nullptr;
			Model::Vec2Array* texcoords = numTexCoordIndices ? new Model::Vec2Array : nullptr;

			if (vertices)
				vertices->reserve(numVertexIndices);
			if (normals)
				normals->reserve(numNormalIndices);
			if (texcoords)
				texcoords->reserve(numTexCoordIndices);

			IMesh* mesh = RenderFactory::s_factory->createMesh();
			if (numPointElements > 0)
			{
				size_t startPos = vertices->size();
				size_t numPoints = 0;
				for (itr = elementList.begin(); itr != elementList.end(); ++itr)
				{
					Element& element = *(*itr);
					if (element.dataType == Element::POINTS)
					{
						for (Element::IndexList::iterator index_itr = element.vertexIndices.begin();
						index_itr != element.vertexIndices.end(); ++index_itr)
						{
							vertices->push_back(transformVertex(model.vertices[*index_itr], localOptions.rotate));
							++numPoints;
						}
						if (numNormalIndices)
						{
							for (Element::IndexList::iterator index_itr = element.normalIndices.begin();
							index_itr != element.normalIndices.end(); ++index_itr)
							{
								normals->push_back(transformNormal(model.normals[*index_itr], localOptions.rotate));
							}
						}
						if (numTexCoordIndices)
						{
							for (Element::IndexList::iterator index_itr = element.texCoordIndices.begin();
							index_itr != element.texCoordIndices.end(); ++index_itr)
							{
								texcoords->push_back(model.texcoords[*index_itr]);
							}
						}
					}
				}
				mesh->addIndexArray(POINT_LIST, (uint32)startPos, (uint32)numPoints);
			}

			if (numPolylineElements > 0)
			{
				std::size_t startPos = vertices->size();
				std::vector<uint16> indexs;

				for (itr = elementList.begin(); itr != elementList.end(); ++itr)
				{
					Element& element = *(*itr);
					if (element.dataType == Element::POLYLINE)
					{
						indexs.push_back(element.vertexIndices.size());

						for (Element::IndexList::iterator index_itr = element.vertexIndices.begin();
						index_itr != element.vertexIndices.end(); ++index_itr)
						{
							vertices->push_back(transformVertex(model.vertices[*index_itr], localOptions.rotate));
						}
						if (numNormalIndices)
						{
							for (Element::IndexList::iterator index_itr = element.normalIndices.begin();
							index_itr != element.normalIndices.end(); ++index_itr)
							{
								normals->push_back(transformNormal(model.normals[*index_itr], localOptions.rotate));
							}
						}
						if (numTexCoordIndices)
						{
							for (Element::IndexList::iterator index_itr = element.texCoordIndices.begin();
							index_itr != element.texCoordIndices.end(); ++index_itr)
							{
								texcoords->push_back(model.texcoords[*index_itr]);
							}
						}
					}
				}
				mesh->addIndexArray(LINE_LIST, &(indexs.front()), (uint32)(indexs.size()));
			}

			DynamicArray<uint32> indexs_strip;
			DynamicArray<uint32> indexs_ts;
			if (numPolygonElements > 0)
			{
				uint32 startPos = (uint32)(vertices->size());
				for (itr = elementList.begin(); itr != elementList.end(); ++itr)
				{
					Element& element = *(*itr);
					if (element.dataType == Element::POLYGON)
					{
						if (element.vertexIndices.size() > 4)
						{
							for (uint32 i = 0; i < element.vertexIndices.size(); i++)
							{
								indexs_strip.push_back(startPos + i);
							}
							//mesh->addIndexArray(TRIANGLE_STRIP, startPos, element.vertexIndices.size());
							startPos += (uint32)(element.vertexIndices.size());
						}
						else
						{
							for (uint32 i = 0; i < element.vertexIndices.size(); i++)
							{
								indexs_ts.push_back(startPos + i);
							}
							//mesh->addIndexArray(TRIANGLE_FAN, startPos, element.vertexIndices.size());
							startPos += (uint32)(element.vertexIndices.size());
						}

						if (model.needReverse(element))
						{
							// need to reverse so add to OSG arrays in same order as in OBJ, as OSG assume anticlockwise ordering.
							for (Element::IndexList::reverse_iterator index_itr = element.vertexIndices.rbegin();
							index_itr != element.vertexIndices.rend(); ++index_itr)
							{
								vertices->push_back(transformVertex(model.vertices[*index_itr], localOptions.rotate));
							}
							if (numNormalIndices)
							{
								for (Element::IndexList::reverse_iterator index_itr = element.normalIndices.rbegin();
								index_itr != element.normalIndices.rend(); ++index_itr)
								{
									normals->push_back(transformNormal(model.normals[*index_itr], localOptions.rotate));
								}
							}


							if (numTexCoordIndices)
							{
								for (Element::IndexList::reverse_iterator index_itr = element.texCoordIndices.rbegin();
								index_itr != element.texCoordIndices.rend();++index_itr)
								{
									texcoords->push_back(model.texcoords[*index_itr]);
								}
							}
						}
						else
						{
							for (Element::IndexList::iterator index_itr = element.vertexIndices.begin();
							index_itr != element.vertexIndices.end(); ++index_itr)
							{
								vertices->push_back(transformVertex(model.vertices[*index_itr], localOptions.rotate));
							}
							if (numNormalIndices)
							{
								for (Element::IndexList::iterator index_itr = element.normalIndices.begin();
								index_itr != element.normalIndices.end(); ++index_itr)
								{
									normals->push_back(transformNormal(model.normals[*index_itr], localOptions.rotate));
								}
							}
							if (numTexCoordIndices)
							{
								for (Element::IndexList::iterator index_itr = element.texCoordIndices.begin();
								index_itr != element.texCoordIndices.end(); ++index_itr)
								{
									texcoords->push_back(model.texcoords[*index_itr]);
								}
							}
						}
					}
				}
			}

			mesh->begin((uint32)(vertices->size()));

			if (vertices)
				mesh->setVertexArray(&(vertices->front()));

			if (normals)
				mesh->setNormalArray(&(normals->front()));

			if (texcoords)
				mesh->setTextureCoordArray(0, &(texcoords->front()));

			if (!indexs_strip.empty())
			{
				mesh->addIndexArray(TRIANGLE_STRIP, indexs_strip.getDataPointer(), indexs_strip.size());
			}
			if (!indexs_ts.empty())
			{
				mesh->addIndexArray(TRIANGLE_LIST, indexs_ts.getDataPointer(), indexs_ts.size());
			}

			mesh->end();

			indexs_strip.detachPointer();
			indexs_ts.detachPointer();

			return mesh;
		}

		IDrawable* convertModel(Model& model, ObjOptionsStruct& localOptions, const char* dataPath)
		{
			if (model.elementStateMap.empty())
				return nullptr;

			IDrawableBase* drawable = RenderFactory::s_factory->createDrawable();

			// set up the materials
			MaterialToStateSetMap materialToStateSetMap;
			buildMaterialToStateSetMap(model, materialToStateSetMap, localOptions, dataPath);

			// go through the groups of related elements and build geometry from them.
			for (Model::ElementStateMap::iterator itr = model.elementStateMap.begin(); itr != model.elementStateMap.end(); ++itr)
			{
				const ElementState& es = itr->first;
				Model::ElementList& el = itr->second;

				IMesh* mesh = convertElementListToGeometry(model, el, localOptions);

				if (mesh)
				{
					MaterialToStateSetMap::const_iterator it = materialToStateSetMap.find(es.materialName);
					if (it == materialToStateSetMap.end())
					{
						//OSG_WARN << "Obj unable to find material '" << es.materialName << "'" << std::endl;
					}

					IMaterial* material = materialToStateSetMap[es.materialName].get();
					if (!material)
						material = RenderFactory::s_factory->getOrCreateMaterial("error");

					drawable->addGeometry(mesh, material);
				}
			}
			return drawable;
		}
	};

	REGISTE_DRAWABLEREADWRITEHELPER(ReaderWriterOBJ)
}