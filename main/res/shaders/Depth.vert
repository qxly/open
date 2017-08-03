layout(location = 0) in vec3 _vertexPosition;
layout(location = 1) in vec3 _vertexNormal;
layout(location = 2) in vec3 _vertexTangent;
layout(location = 3) in vec2 _vertexTextureCoord;
layout(location = 4) in mat4 _mvp;

void main()
{
	vec4 p = _mvp * vec4(_vertexPosition,1.0); 
	gl_Position = p;
}