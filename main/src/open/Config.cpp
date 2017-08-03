#include <open/Config.h>
#include <vector>
#include <map>
#include <string>

namespace open
{
	struct Config::ConfigNode
	{
		virtual ConfigNode* asConfigSection() = 0;
		virtual ConfigNode* asConfigProperty() = 0;
		virtual ConfigNode* asConfigComment() = 0;
	};

	struct Config::ConfigSection : public Config::ConfigNode
	{
		ConfigSection(){}
		~ConfigSection()
		{
			for (std::vector<ConfigNode*>::iterator it = _children.begin(); it != _children.end();)
			{
				delete *it;
				it = _children.erase(it);
			}
		}

		std::string _sectionName;

		std::vector<ConfigNode*> _children;//vector�е�Ԫ�ز�δ�����ǲ���˳�򣨼��ļ��е���Ȼ˳��
		std::map<std::string, int32> _indexMap; //���������ֺ�����vector�е���������map�У�����map������Ŀ���ʵ�ֿ��ٲ���

		virtual ConfigNode* asConfigSection(){ return this; }
		virtual ConfigNode* asConfigProperty(){ return nullptr; }
		virtual ConfigNode* asConfigComment(){ return nullptr; }
	};

	struct Config::ConfigProperty : public Config::ConfigNode
	{
		ConfigProperty(){}
		~ConfigProperty(){}

		std::string _propName;
		std::string _propValue;

		virtual ConfigNode* asConfigSection(){ return nullptr; }
		virtual ConfigNode* asConfigProperty(){ return this; }
		virtual ConfigNode* asConfigComment(){ return nullptr; }
	};

	struct Config::ConfigComment : public Config::ConfigNode
	{
		ConfigComment(){}
		~ConfigComment(){}

		std::string _commentContent;

		virtual ConfigNode* asConfigSection(){ return nullptr; }
		virtual ConfigNode* asConfigProperty(){ return nullptr; }
		virtual ConfigNode* asConfigComment(){ return this; }
	};

	struct Config::ConfigDocoment
	{
		ConfigDocoment(){}
		~ConfigDocoment()
		{
			for (std::vector<ConfigNode*>::iterator it = _nodes.begin(); it != _nodes.end();)
			{
				delete *it;
				it = _nodes.erase(it);
			}
		}

		std::vector<ConfigNode*> _nodes;//vector�е�Ԫ�ز�δ�����ǲ���˳�򣨼��ļ��е���Ȼ˳��

		std::map<std::string, int32> _indexMap; //��section���ֺ�����vector�е���������map�У�����map������Ŀ���ʵ�ֿ��ٲ���
	};


	const char* sm_reservedCharacters = "<>&[]="; // including Util::sm_xssReservedCharacters

	Config::Config()
	{
		_document = new ConfigDocoment;
	}

	Config::~Config()
	{
		delete _document;
	}

	bool Config::loadConfig(const char* file)
	{
		//int index = 0;
		//int propIndex = 0;

		//ConfigSection* sectionNode = nullptr;

		//char* content = loadFile(file);
		//if (content)
		//{
		//	std::vector<std::string> lines = Util::split(fileContent, L"\r\n");

		//	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
		//	{
		//		std::string line = Util::trim(*it);

		//		if (line.empty())
		//			continue;

		//		size_t pos = 0;
		//		if (line[0] == L'#')
		//		{
		//			ConfigComment* commentNode = new ConfigComment;
		//			commentNode->_commentContent = *it;
		//			if (!sectionNode)
		//			{
		//				_document->_nodes.push_back(commentNode);
		//				index++;
		//			}
		//			else
		//			{
		//				sectionNode->_children.push_back(commentNode);
		//				propIndex++;
		//			}
		//		}

		//		if (Util::startsWith(line, L"[") && Util::endsWith(line, L"]"))
		//		{
		//			//��ǰһ���Ѿ��������section����ӵ�_document��
		//			if (sectionNode)
		//			{
		//				_document->_nodes.push_back(sectionNode);
		//				_document->_indexMap[sectionNode->_sectionName] = index++;
		//				propIndex = 0;
		//			}
		//			sectionNode = new ConfigSection;

		//			sectionNode->_sectionName = line.substr(1, line.length() - 2);

		//			if (Util::checkReservedCharacters(sectionNode->_sectionName, sm_reservedCharacters))
		//			{
		//				delete[] content;
		//				return false;
		//			}
		//		}

		//		if (line[0] != L'#' && (pos = line.find(L"=")) != std::string::npos)
		//		{
		//			ConfigProperty* propNode = new ConfigProperty;

		//			propNode->_propName = Util::trim(line.substr(0, pos));
		//			propNode->_propValue = Util::trim(line.substr(pos + 1, line.length() - pos - 1));

		//			if (Util::checkReservedCharacters(propNode->_propName, sm_reservedCharacters))
		//			{
		//				delete[] content;
		//				return false;
		//			}
		//			if (Util::checkReservedCharacters(propNode->_propValue, sm_reservedCharacters))
		//			{
		//				delete[] content;
		//				return false;
		//			}

		//			if (sectionNode)
		//			{
		//				sectionNode->_children.push_back(propNode);
		//				sectionNode->_indexMap[propNode->_propName] = propIndex++;
		//			}
		//			else
		//			{
		//				delete[] content;
		//				return false;
		//			}
		//		}
		//	}

		//	if (sectionNode)
		//	{
		//		//������һ��section�鵽_document��
		//		_document->_nodes.push_back(sectionNode);
		//		_document->_indexMap[sectionNode->_sectionName] = index;
		//	}

		//	delete[] content;
		//	return true;
		//}

		//delete[] content;
		return false;
	}

	char* Config::loadFile(const char* fileName)
	{
		FILE* f;
		char* content = nullptr;
		std::size_t count = 0;

		f = fopen(fileName, "rt");

		if (f != nullptr)
		{
			fseek(f, 0, SEEK_END);
			count = ftell(f);
			rewind(f);

			if (count > 0)
			{
				content = new char[sizeof(char)* (count)];
				count = fread(content, sizeof(char), count, f);
			}
			fclose(f);
			return content;
		}
		else
			return nullptr;
	}

	bool Config::getValue(const char* sectionName, const char* propName, char(&value)[128]) const
	{
		//�����Ƿ����section�飬���ڻ�ȡsection��_document��vector�е�����
		if (_document->_indexMap.count(sectionName))
		{
			ConfigNode* configNode = _document->_nodes[_document->_indexMap[sectionName]];
			if (configNode->asConfigSection())
			{
				ConfigSection* section = static_cast<ConfigSection*>(configNode);

				//section�д�������������������ȡ����
				if (section->_indexMap.count(propName))
				{
					ConfigNode* configNode2 = section->_children[section->_indexMap[propName]];
					if (configNode2->asConfigProperty())
					{
						ConfigProperty* prop = static_cast<ConfigProperty*>(configNode2);
						memset(value, 0, sizeof(value));
						memcpy(value, prop->_propValue.c_str(), prop->_propValue.size());
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	void Config::getStringValue(const char* sectionName, const char* propName, char(&value)[128], const char* defaultValue) const
	{
		if (!getValue(sectionName, propName, value))
			strcpy(value, defaultValue);
	}

	void Config::getDoubleValue(const char* sectionName, const char* propName, double& propValue, double defaultValue) const
	{
		char tempValue[128];
		if (getValue(sectionName, propName, tempValue))
			propValue = atof(tempValue);
		else
			propValue = defaultValue;
	}

	void Config::getIntValue(const char* sectionName, const char* propName, int32& propValue, int32 defaultValue) const
	{
		char tempValue[128];
		if (getValue(sectionName, propName, tempValue))
			propValue = atoi(tempValue);
		else
			propValue = defaultValue;
	}

	void Config::getBoolValue(const char* sectionName, const char* propName, bool& propValue, bool defaultValue) const
	{
		char tempValue[128];
		if (getValue(sectionName, propName, tempValue))
			propValue = (atoi(tempValue) != 0);
		else
			propValue = defaultValue;
	}

	bool Config::setProperty(const char* sectionName, const char* propName, const char* describeContent)
	{
		//_document���Ѿ�����sectionName����ֻ��Ҫ������ԶԾ�����
		if (_document->_indexMap.count(sectionName))
		{
			ConfigNode* configNode = _document->_nodes[_document->_indexMap[sectionName]];
			if (configNode->asConfigSection())
			{
				ConfigSection* section = static_cast<ConfigSection*>(configNode);

				//����sectionName�ﻹ�и�����
				if (section->_indexMap.count(propName))
				{
					ConfigNode* configNode2 = section->_children[section->_indexMap[propName]];
					if (configNode2->asConfigProperty())
					{
						ConfigProperty* prop = static_cast<ConfigProperty*>(configNode2);

						//�޸ĸ����Ե�ֵ
						prop->_propValue = describeContent;
					}
					//����map����ڸ����ԣ����ǻ�ȡ�����ǿ�ֵ����ConfigProperty���ͣ������ϲ���������������
					//��������������˵�������ļ����ص�������
					else
					{
						return false;
					}
				}
				//���û��������ԣ�����Ӹ����Զ�
				else
				{
					ConfigProperty* prop = new ConfigProperty;
					prop->_propName = propName;
					prop->_propValue = describeContent;

					section->_indexMap[prop->_propName] = section->_children.size();
					section->_children.push_back(prop);
				}

			}
			//����map�����sectionName�����ǻ�ȡ�����ǿ�ֵ����ConfigSection���ͣ������ϲ���������������
			//��������������˵�������ļ����ص�������
			else
			{
				return false;
			}
		}
		//_document�в�����sectionName�������section����������Զ�
		else
		{
			ConfigSection* section = new ConfigSection;

			section->_sectionName = sectionName;

			ConfigComment* comment1 = new ConfigComment;
			comment1->_commentContent = "# *****************************************************************************";
			section->_children.push_back(comment1);

			ConfigComment* comment2 = new ConfigComment;
			std::string sectionNameComment = sectionName;
			comment2->_commentContent = sectionNameComment;
			section->_children.push_back(comment2);

			ConfigComment* comment3 = new ConfigComment;
			comment3->_commentContent = std::string("#") + describeContent;
			section->_children.push_back(comment3);

			ConfigComment* comment4 = new ConfigComment;
			comment4->_commentContent = "# *****************************************************************************";
			section->_children.push_back(comment4);

			ConfigProperty* prop = new ConfigProperty;
			prop->_propName = propName;
			prop->_propValue = describeContent;

			section->_indexMap[prop->_propName] = section->_children.size();
			section->_children.push_back(prop);

			_document->_indexMap[sectionName] = _document->_nodes.size();
			_document->_nodes.push_back(section);
		}

		return true;
	}

	//bool Config::removeProperties( CREFstd::string sectionName, std::vector<std::string> propNames )
	//{
	//	//���_document�д���sectionName����������Ҫ���κδ���ֱ�ӷ���true
	//	if(_document->_indexMap.count(sectionName))
	//	{
	//		ConfigNode* configNode = _document->_nodes[_document->_indexMap[sectionName]];
	//		if(configNode->asConfigSection())
	//		{
	//			ConfigSection* section = static_cast<ConfigSection*>(configNode);
	//			//���û��ָ������������ɾ������sectionName
	//			if(propNames.size() == 0)
	//			{
	//				delete section;
	//				_document->_nodes.erase(_document->_nodes.begin() + _document->_indexMap[sectionName]);
	//				_document->_indexMap.erase(sectionName);
	//			}
	//			//����ָ��������������ɾ��ָ�������Զ�
	//			else
	//			{
	//				for(std::vector<std::string>::iterator it = propNames.begin(); it != propNames.end(); ++it)
	//				{
	//					//���sectionName�д��ڸ���������������Ҫ���κδ���ֱ�ӷ���true
	//					if(section->_indexMap.count(*it))
	//					{
	//						ConfigNode* configNode2 = section->_children[section->_indexMap[*it]];
	//						if(configNode2->asConfigProperty())
	//						{
	//							ConfigProperty* prop = static_cast<ConfigProperty*>(configNode2);
	//							//����������ҵ��ڵ㣬ֱ�Ӵ�sectionName���Ƴ�

	//							delete prop;
	//							section->_children.erase(section->_children.begin() + section->_indexMap[sectionName]);
	//							section->_indexMap.erase(*it);
	//						}
	//						//����map����ڸ����ԣ����ǻ�ȡ�����ǿ�ֵ����ConfigProperty���ͣ������ϲ���������������
	//						//��������������˵�������ļ����ص������⣬���������Ƴ�����������ֱ�Ӵ�vector���Ƴ���δ����false
	//						else
	//						{
	//							std::std::string error("Config::getValue ");
	//							error.append(Util::wideCharToMultiByte(*it));
	//							error.append(" internal type error");

	//							ZW_LOG_ERROR(__WFILE__,__LINE__,error.c_str());
	//							return false;
	//						}

	//					}
	//				}
	//			}
	//		}
	//		//����map�����sectionName�����ǻ�ȡ�����ǿ�ֵ����ConfigSection���ͣ������ϲ���������������
	//		//��������������˵�������ļ����ص������⣬���������Ƴ�����������ֱ�Ӵ�vector���Ƴ���δ����false
	//		else
	//		{
	//			std::std::string error("Config::getValue ");
	//			error.append(Util::wideCharToMultiByte(sectionName));
	//			error.append(" internal type error");

	//			ZW_LOG_ERROR(__WFILE__,__LINE__,error.c_str());
	//			return false;
	//		}
	//	}

	//	return true;
	//}

	//void Config::saveProperty( ConfigProperty* configNode, FILE* file )
	//{
	//	wchar_t str[1024];
	//	swprintf(str, 1024, L"%-35s= %s\r\n", (configNode->_propName).c_str(), (configNode->_propValue).c_str());

	//	fputws(str, file);
	//}

	//bool Config::saveSection( ConfigSection* configSection, FILE* file )
	//{
	//	std::string str = std::string(L"\r\n") + L"[" + configSection->_sectionName + L"]" + L"\r\n";
	//	fputws(str.c_str(), file);

	//	for(std::vector<ConfigNode*>::iterator it = configSection->_children.begin(); it != configSection->_children.end(); ++it)
	//	{
	//		if((*it)->asConfigComment())
	//		{
	//			ConfigComment* comment = static_cast<ConfigComment*>(*it);

	//			std::string commentStr = comment->_commentContent + L"\r\n";
	//			fputws(commentStr.c_str(), file);
	//		}
	//		else if((*it)->asConfigProperty())
	//		{
	//			ConfigProperty* prop = static_cast<ConfigProperty*>(*it);
	//			saveProperty(prop, file);
	//		}
	//		else
	//		{
	//			std::std::string error("Config::saveSection ");
	//			error.append(Util::wideCharToMultiByte(configSection->_sectionName));
	//			error.append(" internal type error");

	//			ZW_LOG_ERROR(__WFILE__,__LINE__,error.c_str());

	//			fclose(file);
	//			return false;
	//		}
	//	}

	//	return true;
	//}

	//bool Config::saveConfig(const char* fileName)
	//{
	//	FILE* file = fopen(fileName, "w");
	//
	//	for (std::vector<ConfigNode*>::iterator it = _document->_nodes.begin(); it != _document->_nodes.end(); ++it)
	//	{
	//		if ((*it)->asConfigComment())
	//		{
	//			ConfigComment* comment = static_cast<ConfigComment*>(*it);
	//			std::string str = comment->_commentContent + "\r\n";
	//			fputs(str.c_str(), file);
	//		}
	//		else if ((*it)->asConfigSection())
	//		{
	//			ConfigSection* section = static_cast<ConfigSection*>(*it);
	//			bool flag = saveSection(section, file);
	//
	//			if (!flag)
	//			{
	//				fclose(file);
	//				return false;
	//			}
	//		}
	//		else
	//		{
	//			fclose(file);
	//			return false;
	//		}
	//	}
	//	fclose(file);
	//	return true;
}
