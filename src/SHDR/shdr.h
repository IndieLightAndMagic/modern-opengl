#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <OpenGL/gl.h>
#endif
#include <glm/glm.hpp>

class ShaderSource{
public:
	/* Shader State */
	enum ShaderSourceState{
		SOURCE_STATE_INVALID,
		SOURCE_STATE_VALID
	};
	
private:
	ShaderSourceState m_state;

	/* Shader Path */
	std::string * m_shdrPath;
	
	/* Source Code */
	std::string m_shaderCode;
	const GLchar * m_shdrSrc;
	

public:
	ShaderSource(const GLchar*shdrPath);
	ShaderSource(const std::string &shdrPath);
	virtual ~ShaderSource(){ delete m_shdrPath;};
	const GLchar* operator()();
	GLenum getShaderType();
	ShaderSourceState state();
};



class Shader{

public:
	enum ShaderState{
		SHADER_STATE_INVALID,
		SHADER_STATE_CREATED,
		SHADER_STATE_COMPILED,
	};
	/*!
		@brief Will create a shader out of valid ShaderSource object
		@param shaderSource Pointer to a ShaderSource object. 
		@param shaderType For a Vertex Shader: GL_VERTEX_SHADER, for a fragment shader: GL_FRAGMENT_SHADER, for a geometry shader: GL_GEOMETRY_SHADER.
	*/
	Shader(ShaderSource*pxShaderSource, GLenum shaderType);
	const GLuint operator()();

	/*! @brief State of the Shader
		@return Shader::SHADER_STATE_INVALID for invalid shader usually when shader creation fails. Shader::SHADER_STATE_CREATED Shader was created but the source was not loaded properly. Shader::SHADER_STATE_COMPILED the shader was created, loaded from source properly and compiled OK. 
	*/
	ShaderState state();
private:
	GLuint m_shader;
	Shader::ShaderState m_state;
	/*! Shader Assasination will not occur until it is not detached */
	void killShader();


};

class Program {
	GLuint m_program;
	
public:
	Program();
	
	void setInt(const GLchar* pucUniformName, GLint value);
	void setFloat(const GLchar* pucUniformName, GLfloat value);
	void setVec2(const GLchar* pucUniformName, const glm::vec2 &value);
	void setVec2(const GLchar* pucUniformName, float x, float y);
	void setVec3(const GLchar* pucUniformName, const glm::vec3 &value);
	void setVec3(const GLchar* pucUniformName, float x, float y, float z);
	void setVec4(const GLchar* pucUniformName, const glm::vec4 &value);
	void setVec4(const GLchar* pucUniformName, float x, float y, float z, float w);
	void setMat2(const GLchar* pucUniformName, const glm::mat2 &value);
	void setMat3(const GLchar* pucUniformName, const glm::mat3 &value);
	void setMat4(const GLchar* pucUniformName, const glm::mat4 &value);

	void use();
	/*! 
		Shader will not be pushed in if shader is already attached to this program
	
	Description
	In order to create a complete shader program, there must be a way to specify the list of things that will be linked together. Program objects provide this mechanism. Shaders that are to be linked together in a program object must first be attached to that program object. glAttachShader attaches the shader object specified by shader to the program object specified by program. This indicates that shader will be included in link operations that will be performed on program.
	All operations that can be performed on a shader object are valid whether or not the shader object is attached to a program object. It is permissible to attach a shader object to a program object before source code has been loaded into the shader object or before the shader object has been compiled. It is permissible to attach multiple shader objects of the same type because each may contain a portion of the complete shader. It is also permissible to attach a shader object to more than one program object. If a shader object is deleted while it is attached to a program object, it will be flagged for deletion, and deletion will not occur until glDetachShader is called to detach it from all program objects to which it is attached.
	
	Errors
	GL_INVALID_VALUE is generated if either program or shader is not a value generated by OpenGL.
	GL_INVALID_OPERATION is generated if program is not a program object.
	GL_INVALID_OPERATION is generated if shader is not a shader object.
	GL_INVALID_OPERATION is generated if shader is already attached to program.
	
	On Pushing Non Compiled Shaders.
	--------------------------------

	Shaders can have one of three states: SHADER_STATE_s, where s is one of INVALID, CREATED or COMPILED. Shaders may be pushed into program only when they are in COMPILED state. Any attempt of pushing a shader in other state rather than COMPILED, will break the Program Shader and will invalidate it. 
	

	
	*/
	void pushShader(Shader * s);
	
	/*!
		
		A shader of type shaderType will be compiled out of source ss and on success will be pushed and linked into Shader program.

	On Pushing Invalid Sources.
	---------------------------

	Shader sources may have one of two states: SOURCE_STATE_s, where s is one of VALID or INVALID. Sources may be pushed into the program only when they are on a VALID state. Any attempt of pushing an invalid source will break the shader program.  

	*/
	void pushShader(ShaderSource * ss, GLenum shaderType);

	/*!
		
		A shader of type shaderType will be compiled usign glsl shader source code within a file whose name is stored in a character string pointed by pfilename. The function will also attempt to compile the shader, push it and link it into the program. On failure the whole Program will fail. 

	*/

	void pushShader(const char * pfilename, GLenum shaderType);
	/*!
		Description
	
	glLinkProgram links the program object specified by program. If any shader objects of type GL_VERTEX_SHADER are attached to program, they will be used to create an executable that will run on the programmable vertex processor. If any shader objects of type GL_GEOMETRY_SHADER are attached to program, they will be used to create an executable that will run on the programmable geometry processor. If any shader objects of type GL_FRAGMENT_SHADER are attached to program, they will be used to create an executable that will run on the programmable fragment processor.
	The status of the link operation will be stored as part of the program object's state. This value will be set to GL_TRUE if the program object was linked without errors and is ready for use, and GL_FALSE otherwise. It can be queried by calling glGetProgram with arguments program and GL_LINK_STATUS.
	As a result of a successful link operation, all active user-defined uniform variables belonging to program will be initialized to 0, and each of the program object's active uniform variables will be assigned a location that can be queried by calling glGetUniformLocation. Also, any active user-defined attribute variables that have not been bound to a generic vertex attribute index will be bound to one at this time.
	
	Linking of a program object can fail for a number of reasons as specified in the OpenGL Shading Language Specification. The following lists some of the conditions that will cause a link error.
	
	The number of active attribute variables supported by the implementation has been exceeded.
	The storage limit for uniform variables has been exceeded.
	The number of active uniform variables supported by the implementation has been exceeded.
	The main function is missing for the vertex, geometry or fragment shader.
	A varying variable actually used in the fragment shader is not declared in the same way (or is not declared at all) in the vertex shader, or geometry shader shader if present.
	A reference to a function or variable name is unresolved.
	A shared global is declared with two different types or two different initial values.
	One or more of the attached shader objects has not been successfully compiled.
	Binding a generic attribute matrix caused some rows of the matrix to fall outside the allowed maximum of GL_MAX_VERTEX_ATTRIBS.
	Not enough contiguous vertex attribute slots could be found to bind attribute matrices.
	The program object contains objects to form a fragment shader but does not contain objects to form a vertex shader.
	The program object contains objects to form a geometry shader but does not contain objects to form a vertex shader.
	The program object contains objects to form a geometry shader and the input primitive type, output primitive type, or maximum output vertex count is not specified in any compiled geometry shader object.
	The program object contains objects to form a geometry shader and the input primitive type, output primitive type, or maximum output vertex count is specified differently in multiple geometry shader objects.
	The number of active outputs in the fragment shader is greater than the value of GL_MAX_DRAW_BUFFERS.
	The program has an active output assigned to a location greater than or equal to the value of GL_MAX_DUAL_SOURCE_DRAW_BUFFERS and has an active output assigned an index greater than or equal to one.
	More than one varying out variable is bound to the same number and index.
	The explicit binding assigments do not leave enough space for the linker to automatically assign a location for a varying out array, which requires multiple contiguous locations.
	The count specified by glTransformFeedbackVaryings is non-zero, but the program object has no vertex or geometry shader.
	Any variable name specified to glTransformFeedbackVaryings in the varyings array is not declared as an output in the vertex shader (or the geometry shader, if active).
	Any two entries in the varyings array given glTransformFeedbackVaryings specify the same varying variable.
	The total number of components to capture in any transform feedback varying variable is greater than the constant GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS and the buffer mode is GL_SEPARATE_ATTRIBS.
	When a program object has been successfully linked, the program object can be made part of current state by calling glUseProgram. Whether or not the link operation was successful, the program object's information log will be overwritten. The information log can be retrieved by calling glGetProgramInfoLog.
	glLinkProgram will also install the generated executables as part of the current rendering state if the link operation was successful and the specified program object is already currently in use as a result of a previous call to glUseProgram. If the program object currently in use is relinked unsuccessfully, its link status will be set to GL_FALSE , but the executables and associated state will remain part of the current state until a subsequent call to glUseProgram removes it from use. After it is removed from use, it cannot be made part of current state until it has been successfully relinked.
	If program contains shader objects of type GL_VERTEX_SHADER, and optionally of type GL_GEOMETRY_SHADER, but does not contain shader objects of type GL_FRAGMENT_SHADER, the vertex shader executable will be installed on the programmable vertex processor, the geometry shader executable, if present, will be installed on the programmable geometry processor, but no executable will be installed on the fragment processor. The results of rasterizing primitives with such a program will be undefined.
	The program object's information log is updated and the program is generated at the time of the link operation. After the link operation, applications are free to modify attached shader objects, compile attached shader objects, detach shader objects, delete shader objects, and attach additional shader objects. None of these operations affects the information log or the program that is part of the program object.


	Notes
	If the link operation is unsuccessful, any information about a previous link operation on program is lost (i.e., a failed link does not restore the old state of program ). Certain information can still be retrieved from program even after an unsuccessful link operation. See for instance glGetActiveAttrib and glGetActiveUniform.

	*/
	void link();
	bool isLinked();
	
	void invalidateProgram();
	const GLuint operator()();



private:
	enum ProgramState{
		PROGRAM_STATE_INVALID,
		PROGRAM_STATE_VOID,
		PROGRAM_STATE_SHADERS_ATTACHED,
		PROGRAM_STATE_LINKED_NOT_USED,	//The program shaders are linked but not used.
		PROGRAM_STATE_LINKED_AND_USED,	//The program shaders are linked and are used.
	};
	ProgramState m_state;
public:
	ProgramState state();
};


#endif /* _SHADER_H_ */