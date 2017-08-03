#include "Material.h"

namespace open
{
	Material::MaterailMap Material::s_materailMap = Material::MaterailMap();

	Material* Material::getOrCreateMaterial(const char* materialName)
	{
		MaterailMap::iterator it = s_materailMap.find(materialName);
		if (it == s_materailMap.end())
		{
			Material* material = new Material();
			material->setName(materialName);
			s_materailMap[materialName] = material;
			return material;
		}
		else
			return it->second;
	}
}