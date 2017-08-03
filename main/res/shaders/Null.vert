
layout(location = 0) in vec3 _vertexPosition;

uniform mat4 _mvp;

void main()
{
	gl_Position = _mvp * vec4(_vertexPosition, 1.0);
}