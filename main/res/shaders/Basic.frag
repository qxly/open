in vec2 _varyVertexCoord;
in float _diff;
in float _spec;

out vec4 _fragColor;

uniform sampler2D _diffuseTexture;
uniform sampler2D _emissiveTexture;
uniform sampler2D _opacityTexture;

uniform int _hasOpacityTexture;

uniform vec3 _light_ambientColor = vec3(0.2, 0.2, 0.2);
uniform vec3 _light_diffuseColor = vec3(0.8, 0.8, 0.8);

uniform vec3 _diffuseColor = vec3(0, 0, 0);
uniform vec3 _emissiveColor = vec3(0, 0, 0);

vec4 ComputeColorTexture(vec3 color, sampler2D t)
{
	return vec4(color, 0) + texture(t, _varyVertexCoord);
}

void applyOpacity(inout vec4 color, sampler2D t)
{
	vec4 o = texture(t, _varyVertexCoord);
	color.w = color.w < o.x ? color.w : o.x;
}

void main()
{
	//环境光
	_fragColor.xyz = _light_ambientColor;

	///漫反射
	vec4 color = ComputeColorTexture(_diffuseColor, _diffuseTexture);
	_fragColor.xyz += (_diff * _light_diffuseColor * color.xyz);
	_fragColor.w = color.w;

	/////自发光
	color = ComputeColorTexture(_emissiveColor, _emissiveTexture);
	_fragColor.xyz += color.xyz;
	_fragColor.w = _fragColor.w < color.w ? _fragColor.w : color.w;

	//高光
	_fragColor.xyz += vec3(_spec, _spec, _spec);

	if (_hasOpacityTexture != 0)
		applyOpacity(_fragColor, _opacityTexture);

	if (_fragColor.w == 0)
		discard;
}