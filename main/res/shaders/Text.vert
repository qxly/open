layout(location = 0) in vec3 _vertexPosition;
layout(location = 7) in vec3 _position_size;
layout(location = 8) in vec4 _minWHCoords;
layout(location = 9) in vec4 _textColor;
layout(location = 10) in mat4 _mvp;

out vec2 _varyVertexCoord;
out vec4 _color;

void main()
{
	vec2 p = _vertexPosition.xy * _position_size.z + _position_size.xy;

	gl_Position = _mvp * vec4(p, 0,1.0);

	_color = _textColor;
	_varyVertexCoord = (_vertexPosition.xy + vec2(0.5, 0.5)) * _minWHCoords.zw + _minWHCoords.xy;
}