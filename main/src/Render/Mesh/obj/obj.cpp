#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <functional>

#include "obj.h"
#include <algorithm>
#include <string.h>

namespace open
{
	static std::string strip(const std::string& ss)
	{
		std::string result;
		result.assign(std::find_if(ss.begin(), ss.end(), std::not1(std::ptr_fun< int, int >(isspace))),
			std::find_if(ss.rbegin(), ss.rend(), std::not1(std::ptr_fun< int, int >(isspace))).base());
		return(result);
	}

	static ObjMaterial::Map parseTextureMap(const std::string& ss, ObjMaterial::Map::TextureMapType type)
	{
		ObjMaterial::Map map;
		std::string s(ss);
		for (;;)
		{
			if (s[0] != '-')
				break;

			int n;
			if (s[1] == 's' || s[1] == 'o')
			{
				float x, y, z;
				if (sscanf(s.c_str(), "%*s %f %f %f%n", &x, &y, &z, &n) != 3)
				{
					break;
				}

				if (s[1] == 's')
				{
					// texture scale
					map.uScale = x;
					map.vScale = y;
				}
				else if (s[1] == 'o')
				{
					// texture offset
					map.uOffset = x;
					map.vOffset = y;
				}
			}
			else if (s.compare(1, 2, "mm") == 0)
			{
				// texture color offset and gain
				float base, gain;
				if (sscanf(s.c_str(), "%*s %f %f%n", &base, &gain, &n) != 2)
				{
					break;
				}
				// UNUSED
			}
			else if (s.compare(1, 2, "bm") == 0)
			{
				// blend multiplier
				float mult;
				if (sscanf(s.c_str(), "%*s %f%n", &mult, &n) != 2)
				{
					break;
				}
				// UNUSED
			}
			else if (s.compare(1, 5, "clamp") == 0)
			{
				char c[4];
				if (sscanf(s.c_str(), "%*s %3s%n", c, &n) != 1)
				{
					break;
				}
				if (strncmp(c, "on", 2) == 0) map.clamp = true;
				else map.clamp = false;    // default behavioud
			}
			else
				break;

			s = strip(s.substr(n));
		}

		map.name = s;
		map.type = type;
		return map;
	}

	bool Model::readline(std::istream& fin, char* line, const int LINE_SIZE)
	{
		if (LINE_SIZE<1) return false;

		bool eatWhiteSpaceAtStart = true;
		bool changeTabsToSpaces = true;

		char* ptr = line;
		char* end = line + LINE_SIZE - 1;
		bool skipNewline = false;
		while (fin && ptr<end)
		{

			int c = fin.get();
			int p = fin.peek();
			if (c == '\r')
			{
				if (p == '\n')
				{
					fin.get();
					if (skipNewline)
					{
						skipNewline = false;
						*ptr++ = ' ';
						continue;
					}
					else break;
				}
				if (skipNewline)
				{
					skipNewline = false;
					*ptr++ = ' ';
					continue;
				}
				else break;
			}
			else if (c == '\n')
			{
				if (skipNewline)
				{
					*ptr++ = ' ';
					continue;
				}
				else break;
			}
			else if (c == '\\' && (p == '\r' || p == '\n'))
			{
				// need to keep return;
				skipNewline = true;
			}
			else if (c != std::ifstream::traits_type::eof()) // don't copy eof.
			{
				skipNewline = false;

				if (!eatWhiteSpaceAtStart || (c != ' ' && c != '\t'))
				{
					eatWhiteSpaceAtStart = false;
					*ptr++ = c;
				}
			}


		}

		// strip trailing spaces
		while (ptr>line && *(ptr - 1) == ' ')
		{
			--ptr;
		}

		*ptr = 0;

		if (changeTabsToSpaces)
		{

			for (ptr = line; *ptr != 0; ++ptr)
			{
				if (*ptr == '\t') *ptr = ' ';
			}
		}

		return true;
	}

	std::string Model::lastComponent(const char* linep)
	{
		std::string line = std::string(linep);
		std::size_t space = line.find_last_of(" ");
		if (space >= 0) {
			line = line.substr(space + 1);
		}
		return line;
	}

	bool Model::readMTL(std::istream& fin)
	{
		const int LINE_SIZE = 4096;
		char line[LINE_SIZE];
		float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;

		ObjMaterial* material = 0;// &(materialMap[""]);
		std::string filename;

		while (fin)
		{
			readline(fin, line, LINE_SIZE);
			if (line[0] == '#' || line[0] == '$')
			{
				std::cout <<"Comment: "<<line<<std::endl;
			}
			else if (strlen(line)>0)
			{
				if (strncmp(line, "newmtl ", 7) == 0)
				{
					std::string materialName(line + 7);
					material = &materialMap[materialName];
					material->name = materialName;
				}
				else if (material)
				{
					if (strncmp(line, "Ka ", 3) == 0)
					{
						unsigned int fieldsRead = sscanf(line + 3, "%f %f %f %f", &r, &g, &b, &a);

						if (fieldsRead == 1)
						{
							material->ambient[0] = r;
						}
						else if (fieldsRead == 2)
						{
							material->ambient[0] = r;
							material->ambient[1] = g;
						}
						else if (fieldsRead == 3)
						{
							material->ambient[0] = r;
							material->ambient[1] = g;
							material->ambient[2] = b;
						}
						else if (fieldsRead == 4)
						{
							material->ambient[0] = r;
							material->ambient[1] = g;
							material->ambient[2] = b;
							material->ambient[3] = a;
						}
					}
					else if (strncmp(line, "Kd ", 3) == 0)
					{
						unsigned int fieldsRead = sscanf(line + 3, "%f %f %f %f", &r, &g, &b, &a);

						if (fieldsRead == 1)
						{
							material->diffuse[0] = r;
						}
						else if (fieldsRead == 2)
						{
							material->diffuse[0] = r;
							material->diffuse[1] = g;
						}
						else if (fieldsRead == 3)
						{
							material->diffuse[0] = r;
							material->diffuse[1] = g;
							material->diffuse[2] = b;
						}
						else if (fieldsRead == 4)
						{
							material->diffuse[0] = r;
							material->diffuse[1] = g;
							material->diffuse[2] = b;
							material->diffuse[3] = a;
						}
					}
					else if (strncmp(line, "Ks ", 3) == 0)
					{
						unsigned int fieldsRead = sscanf(line + 3, "%f %f %f %f", &r, &g, &b, &a);

						if (fieldsRead == 1)
						{
							material->specular[0] = r;
						}
						else if (fieldsRead == 2)
						{
							material->specular[0] = r;
							material->specular[1] = g;
						}
						else if (fieldsRead == 3)
						{
							material->specular[0] = r;
							material->specular[1] = g;
							material->specular[2] = b;
						}
						else if (fieldsRead == 4)
						{
							material->specular[0] = r;
							material->specular[1] = g;
							material->specular[2] = b;
							material->specular[3] = a;
						}
					}
					else if (strncmp(line, "Ke ", 3) == 0)
					{
						unsigned int fieldsRead = sscanf(line + 3, "%f %f %f %f", &r, &g, &b, &a);

						if (fieldsRead == 1)
						{
							material->emissive[0] = r;
						}
						else if (fieldsRead == 2)
						{
							material->emissive[0] = r;
							material->emissive[1] = g;
						}
						else if (fieldsRead == 3)
						{
							material->emissive[0] = r;
							material->emissive[1] = g;
							material->emissive[2] = b;
						}
						else if (fieldsRead == 4)
						{
							material->emissive[0] = r;
							material->emissive[1] = g;
							material->emissive[2] = b;
							material->emissive[3] = a;
						}
					}
					else if (strncmp(line, "Tf ", 3) == 0)
					{
						unsigned int fieldsRead = sscanf(line + 3, "%f %f %f %f", &r, &g, &b, &a);

						if (fieldsRead == 1)
						{
							material->Tf[0] = r;
						}
						else if (fieldsRead == 2)
						{
							material->Tf[0] = r;
							material->Tf[1] = g;
						}
						else if (fieldsRead == 3)
						{
							material->Tf[0] = r;
							material->Tf[1] = g;
							material->Tf[2] = b;
						}
						else if (fieldsRead == 4)
						{
							material->Tf[0] = r;
							material->Tf[1] = g;
							material->Tf[2] = b;
							material->Tf[3] = a;
						}
					}
					else if (strncmp(line, "sharpness ", 10) == 0)
					{
						float sharpness = 0.0f;
						unsigned int fieldsRead = sscanf(line + 10, "%f", &sharpness);

						if (fieldsRead == 1) material->sharpness = sharpness;
					}
					else if (strncmp(line, "illum ", 6) == 0)
					{
						int illum = 0;
						unsigned int fieldsRead = sscanf(line + 6, "%d", &illum);

						if (fieldsRead == 1) material->illum = illum;
					}
					else if (strncmp(line, "Ns ", 3) == 0)
					{
						int Ns = 0;
						unsigned int fieldsRead = sscanf(line + 3, "%d", &Ns);

						if (fieldsRead == 1) material->Ns = Ns;
					}
					else if (strncmp(line, "Ni ", 3) == 0)
					{
						int Ni = 0;
						unsigned int fieldsRead = sscanf(line + 3, "%d", &Ni);

						if (fieldsRead == 1) material->Ni = Ni;
					}
					else if (strncmp(line, "Tr ", 3) == 0)
					{
						float alpha = 1.0f;
						unsigned int fieldsRead = sscanf(line + 3, "%f", &alpha);

						if (fieldsRead == 1)
						{
							material->ambient[3] = alpha;
							material->diffuse[3] = alpha;
							material->specular[3] = alpha;
							material->emissive[3] = alpha;
						}
					}
					else if (strncmp(line, "d ", 2) == 0)
					{
						float alpha = 1.0f;
						unsigned int fieldsRead = sscanf(line + 2, "%f", &alpha);

						if (fieldsRead == 1)
						{
							material->ambient[3] = alpha;
							material->diffuse[3] = alpha;
							material->specular[3] = alpha;
							material->emissive[3] = alpha;
						}
					}
					/*else if (strncmp(line, "map_Ka ", 7) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 7), ObjMaterial::Map::AMBIENT));
					}*/
					// diffuse map
					else if (strncmp(line, "map_Kd ", 7) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 7), ObjMaterial::Map::DIFFUSE_LAYER));
					}
					// specular colour/level map
					else if (strncmp(line, "map_Ks ", 7) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 7), ObjMaterial::Map::SPECULAR));
					}
					// map_opacity doesn't exist in the spec, but was already in the plugin
					// so leave it or plugin will break for some users
					else if (strncmp(line, "map_opacity ", 12) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 12), ObjMaterial::Map::OPACITY_LAYER));
					}
					// proper dissolve/opacity map
					else if (strncmp(line, "map_d ", 6) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 6), ObjMaterial::Map::OPACITY_LAYER));
					}
					// specular exponent map
					else if (strncmp(line, "map_Ns ", 7) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 7), ObjMaterial::Map::SPECULAR_EXPONENT));
					}
					// modelling tools and convertors variously produce bump, map_bump, and map_Bump so parse them all
					else if (strncmp(line, "bump ", 5) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 5), ObjMaterial::Map::BUMP_LAYER));
					}
					else if (strncmp(line, "map_bump ", 9) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 9), ObjMaterial::Map::BUMP_LAYER));
					}
					else if (strncmp(line, "map_Bump ", 9) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 9), ObjMaterial::Map::BUMP_LAYER));
					}
					// displacement map
					else if (strncmp(line, "disp ", 5) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 5), ObjMaterial::Map::DISPLACEMENT));
					}
					// reflection map (the original code had the possibility of a blank "refl" line
					// which isn't correct according to the spec, so this bit might break for some
					// modelling packages...
					else if (strncmp(line, "refl ", 5) == 0)
					{
						material->maps.push_back(parseTextureMap(strip(line + 5), ObjMaterial::Map::REFLECTION));
					}
					else
					{
						std::cout << "*** line not handled *** :" << line << std::endl;
					}
				}
				else
				{
					std::cout << "*** line not handled *** :" << line << std::endl;
				}
			}
		}

		return true;
	}

	std::string trim(const std::string& s)
	{
		if (s.length() == 0)
			return s;
		std::size_t b = s.find_first_not_of(" \t");
		std::size_t e = s.find_last_not_of(" \t");
		if (b == -1) // No non-spaces
			return "";
		return std::string(s, b, e - b + 1);
	}

	bool Model::readOBJ(std::istream& fin)
	{
		const int LINE_SIZE = 4096;
		char line[LINE_SIZE];
		float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

		while (fin)
		{
			readline(fin, line, LINE_SIZE);
			if (line[0] == '#' || line[0] == '$')
			{
			}
			else if (strlen(line)>0)
			{
				if (strncmp(line, "v ", 2) == 0)
				{
					unsigned int fieldsRead = sscanf(line + 2, "%f %f %f %f", &x, &y, &z, &w);

					if (fieldsRead == 1)
						vertices.push_back(Real3(x, 0.0f, 0.0f));
					else if (fieldsRead == 2)
						vertices.push_back(Real3(x, y, 0.0f));
					else if (fieldsRead == 3)
						vertices.push_back(Real3(x, y, z));
					else if (fieldsRead >= 4)
						vertices.push_back(Real3(x / w, y / w, z / w));
				}
				else if (strncmp(line, "vn ", 3) == 0)
				{
					unsigned int fieldsRead = sscanf(line + 3, "%f %f %f", &x, &y, &z);

					if (fieldsRead == 1)
						normals.push_back(Real3(x, 0.0f, 0.0f));
					else if (fieldsRead == 2)
						normals.push_back(Real3(x, y, 0.0f));
					else if (fieldsRead == 3)
						normals.push_back(Real3(x, y, z));
				}
				else if (strncmp(line, "vt ", 3) == 0)
				{
					unsigned int fieldsRead = sscanf(line + 3, "%f %f %f", &x, &y, &z);

					if (fieldsRead == 1)
						texcoords.push_back(Real2(x, 0.0f));
					else if (fieldsRead == 2)
						texcoords.push_back(Real2(x, y));
					else if (fieldsRead == 3)
						texcoords.push_back(Real2(x, y));
				}
				else if (strncmp(line, "l ", 2) == 0 ||
					strncmp(line, "p ", 2) == 0 ||
					strncmp(line, "f ", 2) == 0)
				{
					char* ptr = line + 2;

					Ptr<Element> element = new Element((line[0] == 'p') ? Element::POINTS :
						(line[0] == 'l') ? Element::POLYLINE :
						Element::POLYGON);

					int vi = 0, ti = 0, ni = 0;
					while (*ptr != 0)
					{
						while (*ptr == ' ')
							++ptr;

						if (sscanf(ptr, "%d/%d/%d", &vi, &ti, &ni) == 3)
						{
							element->vertexIndices.push_back((int32)remapVertexIndex(vi));
							element->normalIndices.push_back((int32)remapNormalIndex(ni));
							element->texCoordIndices.push_back((int32)remapTexCoordIndex(ti));
						}
						else if (sscanf(ptr, "%d//%d", &vi, &ni) == 2)
						{
							element->vertexIndices.push_back((int32)remapVertexIndex(vi));
							element->normalIndices.push_back((int32)remapNormalIndex(ni));
						}
						else if (sscanf(ptr, "%d/%d", &vi, &ti) == 2)
						{
							element->vertexIndices.push_back((int32)remapVertexIndex(vi));
							element->texCoordIndices.push_back((int32)remapTexCoordIndex(ti));
						}
						else if (sscanf(ptr, "%d", &vi) == 1)
						{
							element->vertexIndices.push_back((int32)remapVertexIndex(vi));
						}

						// skip to white space or end of line
						while (*ptr != ' ' && *ptr != 0)
							++ptr;
					}

					if (!element->normalIndices.empty() && element->normalIndices.size() != element->vertexIndices.size())
					{
						element->normalIndices.clear();
					}

					if (!element->texCoordIndices.empty() && element->texCoordIndices.size() != element->vertexIndices.size())
					{
						element->texCoordIndices.clear();
					}

					if (!element->vertexIndices.empty())
					{
						Element::CoordinateCombination coordateCombination = element->getCoordinateCombination();
						if (coordateCombination != currentElementState.coordinateCombination)
						{
							currentElementState.coordinateCombination = coordateCombination;
							currentElementList = 0; // reset the element list to force a recompute of which ElementList to use
						}
						addElement(element);
					}
				}
				else if (strncmp(line, "usemtl ", 7) == 0)
				{
					std::string materialName(line + 7);
					if (currentElementState.materialName != materialName)
					{
						currentElementState.materialName = materialName;
						currentElementList = 0; // reset the element list to force a recompute of which ElementList to use
					}
				}
				else if (strncmp(line, "mtllib ", 7) == 0)
				{
					std::string materialFileName = trim(line + 7);
					std::string fullPathFileName = databasePath + materialFileName;
					if (!fullPathFileName.empty())
					{
						std::ifstream mfin(fullPathFileName.c_str());
						if (mfin.is_open())
						{
							readMTL(mfin);
						}
					}
				}
				else if (strncmp(line, "o ", 2) == 0)
				{
					std::string objectName(line + 2);
					if (currentElementState.objectName != objectName)
					{
						currentElementState.objectName = objectName;
						currentElementList = 0; // reset the element list to force a recompute of which ElementList to use
					}
				}
				else if (strcmp(line, "o") == 0)
				{
					std::string objectName(""); // empty name
					if (currentElementState.objectName != objectName)
					{
						currentElementState.objectName = objectName;
						currentElementList = 0; // reset the element list to force a recompute of which ElementList to use
					}
				}
				else if (strncmp(line, "g ", 2) == 0)
				{
					std::string groupName(line + 2);
					if (currentElementState.groupName != groupName)
					{
						currentElementState.groupName = groupName;
						currentElementList = 0; // reset the element list to force a recompute of which ElementList to use
					}
				}
				else if (strcmp(line, "g") == 0)
				{
					std::string groupName(""); // empty name
					if (currentElementState.groupName != groupName)
					{
						currentElementState.groupName = groupName;
						currentElementList = 0; // reset the element list to force a recompute of which ElementList to use
					}
				}
				else if (strncmp(line, "s ", 2) == 0)
				{
					int smoothingGroup = 0;
					if (strncmp(line + 2, "off", 3) == 0) smoothingGroup = 0;
					else sscanf(line + 2, "%d", &smoothingGroup);

					if (currentElementState.smoothingGroup != smoothingGroup)
					{
						currentElementState.smoothingGroup = smoothingGroup;
						currentElementList = 0; // reset the element list to force a recompute of which ElementList to use
					}
				}
				else
				{
				}
			}
		}
		return true;
	}


	void Model::addElement(Element* element)
	{
		if (!currentElementList)
		{
			currentElementList = &(elementStateMap[currentElementState]);
		}
		currentElementList->push_back(element);

	}

	Real3 Model::averageNormal(const Element& element) const
	{
		Real3 normal;
		for (Element::IndexList::const_iterator itr = element.normalIndices.begin();
			itr != element.normalIndices.end();
			++itr)
		{
			normal += normals[*itr];
		}
		normal.normalize();

		return normal;
	}

	Real3 Model::computeNormal(const Element& element) const
	{
		Real3 normal;
		for (unsigned int i = 0; i<element.vertexIndices.size() - 2; ++i)
		{
			Real3 a = vertices[element.vertexIndices[i]];
			Real3 b = vertices[element.vertexIndices[i + 1]];
			Real3 c = vertices[element.vertexIndices[i + 2]];
			Real3 localNormal = (b - a) ^ (c - b);
			normal += localNormal;
		}
		normal.normalize();

		return normal;
	}

	bool Model::needReverse(const Element& element) const
	{
		if (element.normalIndices.empty()) return false;

		return computeNormal(element)*averageNormal(element) < 0.0f;
	}
}