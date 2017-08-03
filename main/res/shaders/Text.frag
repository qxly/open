in vec2 _varyVertexCoord;
in vec4 _color;

out vec4 _fragColor;

uniform sampler2D _fontTexture;

void main()
{
	float p = texture(_fontTexture, _varyVertexCoord).x;
	_fragColor = _color * p;
}