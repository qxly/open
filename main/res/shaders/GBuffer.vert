layout(location = 0) in vec3 _vertexPosition;
layout(location = 1) in vec3 _vertexNormal;
layout(location = 2) in int _color;

layout(location = 3) in vec2 _vertexTextureCoord;
//layout(location = 4) in vec2 _vertexTextureCoord1;
//layout(location = 5) in vec2 _vertexTextureCoord2;
//layout(location = 6) in vec2 _vertexTextureCoord3;

layout(location = 7) in mat4 _mvp;
layout(location = 11) in mat4 _normalMatrix;

out vec3 _normal;
out vec2 _textureCoord;

uniform sampler2D _normalFitTexture;

void main()
{
	gl_Position = _mvp * vec4(_vertexPosition, 1.0);

	_normal = normalize(_normalMatrix * vec4(_vertexNormal, 0.0f)).xyz;
	_textureCoord = _vertexTextureCoord;
}