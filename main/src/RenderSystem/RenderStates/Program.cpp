#include "Program.h"
#include "../GLState.h"
#include <stdarg.h>

using namespace open::gl;

namespace open
{
	Program::Program()
		:_program(0)
	{
	}

	Program::~Program()
	{
	}

	void Program::releaseGPUContent()
	{
		gl_DeleteProgram(_program);
	}

	bool Program::loadProgramFromBuffer(const char* vs, const char* ps, const char* defines, const char* version)
	{
		if (_program != 0)
			gl_DeleteProgram(_program);

		createProgram(vs, ps, defines, version);
		if (_program == 0)
			return false;
		return true;
	}

	bool Program::loadFromFile(const char* vertFile, const char* frageFile, const char* defines, const char* version)
	{
		if (_program != 0)
			gl_DeleteProgram(_program);

		char* vert_buffer = readFile(vertFile);
		if (!vert_buffer)
			return false;

		char* frag_buffer = readFile(frageFile);
		if (!frag_buffer)
		{
			delete[] vert_buffer;
			return false;
		}

		createProgram(vert_buffer, frag_buffer, defines, version);

		delete[] vert_buffer;
		delete[] frag_buffer;

		if (_program == 0)
			return false;

		return true;
	}

	void Program::apply()
	{
		if (_program != 0)
			gl_UseProgram(_program);
	}

	uint32 Program::createShader(const char* shaderBuffer, bool vertex, const char* defins, const char* version)
	{
		if (!shaderBuffer)
			return 0;

		uint32 shader;
		if (vertex)
			shader = gl_CreateShader(OPEN_GL_VERTEX_SHADER);
		else
			shader = gl_CreateShader(OPEN_GL_FRAGMENT_SHADER);

		const char* array[3] = { nullptr, nullptr, nullptr };
		int32 a_c = 0;
		if (version)
			array[a_c++] = version;
		if (defins)
			array[a_c++] = defins;

		array[a_c++] = shaderBuffer;

		gl_ShaderSource(shader, a_c, array, nullptr);

		gl_CompileShader(shader);

		int32 compileResult;
		gl_GetShaderiv(shader, OPEN_GL_COMPILE_STATUS, &compileResult);

		if (OPEN_GL_FALSE == compileResult)
		{
			int32 logLen;
			gl_GetShaderiv(shader, OPEN_GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0)
			{
				char* log = new char[logLen];
				int32 written;
				gl_GetShaderInfoLog(shader, logLen, &written, log);

				printf("createShader error: %s\n", log);
				delete[] log;
				ASSERT_(false);
			}

			gl_DeleteShader(shader);
			return 0;
		}
		return shader;
	}

	void Program::createProgram(const char* vertShader, const char* frageShader, const char* defines, const char* version)
	{
		uint32 vertex_shader = createShader(vertShader, true, defines, version);
		if (vertex_shader == 0)
			return;

		uint32 fragment_shader = createShader(frageShader, false, defines, version);
		if (fragment_shader == 0)
			return;

		_program = gl_CreateProgram();
		gl_AttachShader(_program, vertex_shader);
		gl_AttachShader(_program, fragment_shader);

		gl_DeleteShader(vertex_shader);
		gl_DeleteShader(fragment_shader);

		link();
	}

	void Program::link()
	{
		if (!_program)
			return;

		gl_LinkProgram(_program);

#if _DEBUG
		int32 linkResult;
		gl_GetProgramiv(_program, OPEN_GL_LINK_STATUS, &linkResult);
		if (OPEN_GL_TRUE == linkResult)
			return;

		int32 logLen;
		gl_GetProgramiv(_program, OPEN_GL_INFO_LOG_LENGTH, &logLen);

		char* log = new char[logLen];
		int32 written;
		gl_GetProgramInfoLog(_program, logLen, &written, log);
		printf("createShader error: %s\n", log);
		delete[] log;
		ASSERT_(false);
#endif
	}

	char* Program::readFile(const char* file)
	{
		FILE* f;
		char* content = nullptr;
		int count = 0;

		f = fopen(file, "rt");

		if (f != nullptr)
		{

			fseek(f, 0, SEEK_END);
			count = ftell(f);
			rewind(f);

			if (count > 0)
			{
				content = new char[sizeof(char)* (count + 1)];
				count = fread(content, sizeof(char), count, f);
				content[count] = '\0';
			}
			fclose(f);
		}
		return content;
	}

	void Program::setFragDataLocation(int32 index, const char* outName)
	{
		if (!_program)
			return;

		link();
	}

#define PROGRAM_SET_UNIFORM_LOG(file,line,uniformName)\
	char error_port[256] = { 0 }; \
	sprintf(error_port, "%s in line %d : program %d is not find uniformName-- %s", file, line, _program, uniformName); \
	printf("%s\n", error_port); \
	ASSERT_(0)\

	int32 Program::checkUniformName(const char* uniformName)
	{
		int32 mm = gl_GetUniformLocation(_program, uniformName);
		if (mm == -1)
		{
			PROGRAM_SET_UNIFORM_LOG(__FILE__, __LINE__, uniformName);
		}
		return mm;
	}

	int32 Program::getUniformID(const char* uniformName)
	{
		int32 mm = gl_GetUniformLocation(_program, uniformName);
		return mm;
	}

	void Program::setUniformi(int32 id, int32 value)
	{
		if (id == -1)
			return;
		gl_Uniform1i(id, value);
	}

	void Program::setUniformf(int32 id, Real value)
	{
		if (id == -1)
			return;
#if DOUBLE_PRECISION
		gl_Uniform1f(id, static_cast<float>(value));
#else
		gl_Uniform1f(id, value);
#endif
	}

	void Program::setUniform2f(int32 id, const Real2& value)
	{
		if (id == -1)
			return;
#if DOUBLE_PRECISION
		gl_Uniform2fv(id, 1, Vec2f(static_cast<float>(value.x()), static_cast<float>(value.y())).ptr());
#else
		gl_Uniform2fv(id, 1, value.ptr());
#endif
	}

	void Program::setUniform3f(int32 id, const Real3& value)
	{
		if (id == -1)
			return;
#if DOUBLE_PRECISION
		gl_Uniform3fv(id, 1, Vec3f(static_cast<float>(value.x()), static_cast<float>(value.y()), static_cast<float>(value.z())).ptr());
#else
		gl_Uniform3fv(id, 1, value.ptr());
#endif
	}

	void Program::setUniform4f(int32 id, const Real4& value)
	{
		if (id == -1)
			return;
#if DOUBLE_PRECISION
		gl_Uniform4fv(id, 1, Vec4f(static_cast<float>(value.x()), static_cast<float>(value.y()),
			static_cast<float>(value.z()), static_cast<float>(value.w())).ptr());
#else
		gl_Uniform4fv(id, 1, value.ptr());
#endif
	}

	void Program::setUniform16f(int32 id, const RMatrix& value)
	{
		if (id == -1)
			return;
#if DOUBLE_PRECISION
		float swap[16];
		for (int i = 0; i < 16; i++)
			swap[i] = static_cast<float>(value.ptr()[i]);

		gl_UniformMatrix4fv(id, 1, GL_TRUE, swap);
#else
		gl_UniformMatrix4fv(id, 1, OPEN_GL_TRUE, value.ptr());
#endif
	}

	void Program::setUniformi(const char* uniformName, int32 value)
	{
		int32 mm = checkUniformName(uniformName);
		if (mm == -1)
			return;

		gl_Uniform1i(mm, value);
	}

	void Program::setUniformf(const char* uniformName, Real value)
	{
		int32 mm = checkUniformName(uniformName);
		if (mm == -1)
			return;

#if DOUBLE_PRECISION
		gl_Uniform1f(mm, static_cast<float>(value));
#else
		gl_Uniform1f(mm, value);
#endif

	}

	void Program::setUniform2f(const char* uniformName, const Real2& value)
	{
		int32 mm = checkUniformName(uniformName);
		if (mm == -1)
			return;

#if DOUBLE_PRECISION
		gl_Uniform2fv(mm, 1, Vec2f(static_cast<float>(value.x()), static_cast<float>(value.y())).ptr());
#else
		gl_Uniform2fv(mm, 1, value.ptr());
#endif

	}

	void Program::setUniform3f(const char* uniformName, const Real3& value)
	{
		int32 mm = checkUniformName(uniformName);
		if (mm == -1)
			return;

#if DOUBLE_PRECISION
		gl_Uniform3fv(mm, 1, Vec3f(static_cast<float>(value.x()), static_cast<float>(value.y()), static_cast<float>(value.z())).ptr());
#else
		gl_Uniform3fv(mm, 1, value.ptr());
#endif
	}

	void Program::setUniform4f(const char* uniformName, const Real4& value)
	{
		int32 mm = checkUniformName(uniformName);
		if (mm == -1)
			return;

#if DOUBLE_PRECISION
		gl_Uniform4fv(mm, 1, Vec4f(static_cast<float>(value.x()), static_cast<float>(value.y()),
			static_cast<float>(value.z()), static_cast<float>(value.w())).ptr());
#else
		gl_Uniform4fv(mm, 1, value.ptr());
#endif
	}

	void Program::setUniform16f(const char* uniformName, const RMatrix& value)
	{
		int32 mm = checkUniformName(uniformName);
		if (mm == -1)
			return;

#if DOUBLE_PRECISION
		float swap[16];
		for (int i = 0; i < 16; i++)
			swap[i] = static_cast<float>(value.ptr()[i]);

		gl_UniformMatrix4fv(mm, 1, OPEN_GL_TRUE, swap);
#else
		gl_UniformMatrix4fv(mm, 1, OPEN_GL_TRUE, value.ptr());
#endif
	}

	void Program::setSubroutine(ShaderType shaderType, int32 count, const char* funName, ...)
	{
		/*uint32 names[64];

		va_list ap;
		va_start(ap, funName);

		int32 i = 0;
		while (count > 0)
		{
			uint32 funDir = glGetSubroutineIndex(_program, shaderType, funName);
			names[i++] = funDir;
			funName = va_arg(ap, const char*);
			count--;
		}
		va_end(ap);

		glUniformSubroutinesuiv(shaderType, i, names);*/
	}
}