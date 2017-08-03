
in vec4 _color;
in mat2 _rotationMatrix;

uniform bool _hasTexture;
uniform sampler2D _textureUnit0;

out vec4 _fragColor;

void main()
{
	vec2 pt = gl_PointCoord - vec2(0.5);
	if(_hasTexture)
	{
		_fragColor = texture(_textureUnit0 , pt * _rotationMatrix + vec2(0.5)) * _color;
	}
	else
	{
		_fragColor = _color;
	}
}