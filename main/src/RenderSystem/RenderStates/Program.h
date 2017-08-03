#ifndef	RENDERSYSTEM_PROGRAM_H
#define RENDERSYSTEM_PROGRAM_H

#include <RenderSystem/RenderStatesInterface.h>
#include <open/Memory.h>

namespace open
{
	class Program : public IProgram PUBLIC_OBJECT_DEBUG
	{
	public:

		Program();

		~Program();

		virtual bool loadFromFile(const char* vs_filePath, const char* ps_filePath, const char* defines, const char* version);
		
		virtual bool loadProgramFromBuffer(const char* vs, const char* ps, const char* defines, const char* version);

		void releaseGPUContent();

	public:

		virtual void apply();

		virtual void setFragDataLocation(int32 index,const char* outName);

		int32 getUniformID(const char* uniformName);

		void setUniformi(int32 id, int32 value);

		void setUniformf(int32 id, Real value);

		void setUniform2f(int32 id, const Real2& value);

		void setUniform3f(int32 id, const Real3& value);

		void setUniform4f(int32 id, const Real4& value);

		void setUniform16f(int32 id, const RMatrix& value);

		void setUniformi(const char* uniformName,int32 value);

		void setUniformf(const char* uniformName,Real value);
		
		void setUniform2f(const char* uniformName,const Real2& value);

		void setUniform3f(const char* uniformName,const Real3& value);

		void setUniform4f(const char* uniformName,const Real4& value);

		void setUniform16f(const char* uniformName,const RMatrix& value);

		void setSubroutine(ShaderType shaderType,int32 count,const char* funName,...);
	private:

		void link();

		bool reset(const char* vertShader,const char* frageShader);

		void createProgram(const char* vertShader, const char* frageShader, const char* defines, const char* version);

		uint32 createShader(const char* shaderBuffer, bool vertex, const char* defins, const char* version);

		char* readFile(const char* file);

		int32 checkUniformName(const char* uniformName);

	private:
		uint32 _program;
	};
}

#endif