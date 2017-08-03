
layout(location = 0) in vec3 _vertexPosition;
layout(location = 4) in mat4 _mvp;

uniform mat4 _lightShdowMapMVP;

out vec2 _coord;

void main()
{
	gl_Position = _mvp * vec4(_vertexPosition,1.0);

	vec4 coord_4 = _lightShdowMapMVP * vec4(_vertexPosition,1.0);
	coord_4.xyz /= coord_4.w;

	_coord.xy = coord_4.xy * 0.5 + vec2(0.5);
}