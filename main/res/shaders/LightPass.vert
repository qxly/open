#if (defined POINT_LIGHT || defined SPOT_LIGHT)

layout(location = 0) in vec3 _vertexPosition;
uniform mat4 _mvp;

void main()
{
	gl_Position = _mvp * vec4(_vertexPosition, 1.0);
}

#elif (defined DIRECT_LIGHT || defined EMISSIVE_RENDER)

in vec3 _screenPosition;

void main()
{
	gl_Position.w = 1;
	gl_Position.xyz = _screenPosition.xyz;
}

#endif