
in vec3 _normal;
in vec2 _textureCoord;

layout(location = 0) out vec4 _normal_shininess;
layout(location = 1) out vec4 _diffuse_specular;
layout(location = 2) out vec3 _emissive_materailMask;

uniform vec3 _diffuseColor = vec3(0, 0, 0);
uniform vec3 _emissiveColor = vec3(0.2, 0.2, 0.2);

uniform sampler2D _diffuseTexture;
uniform sampler2D _emissiveTexture;

uniform float _shininess;

vec3 Compute_Color_Texture(vec3 color, sampler2D t)
{
	return color + texture(t, _textureCoord).xyz;
}

#define PI 3.14159265

float packUnorm12Float(float f) {
	return round(clamp(f, 0.0, 1.0) * float(4095));
}

vec3 twoNorm12sEncodedAs3Unorm8sInVec3Format(vec2 s) {
	vec3 u;
	u.x = s.x * (1.0 / 16.0);
	float t = floor(s.y*(1.0 / 256));
	u.y = (fract(u.x) * 256) + t;
	u.z = s.y - (t * 256);
	// Instead of a floor, you could just add vec3(-0.5) to u, 
	// and the hardware will take care of the flooring for you on save to an RGB8 texture
	return floor(u) * (1.0 / 255.0);
}

vec3 vec2To2Unorm12sEncodedAs3Unorm8sInVec3Format(vec2 v) {
	vec2 s = vec2(packUnorm12Float(v.x), packUnorm12Float(v.y));
	return twoNorm12sEncodedAs3Unorm8sInVec3Format(s);
}

vec2 sphericalEncode(in vec3 v) {
	// Assuming rho is always 1 (normalized v)
	float thetaNormalized = acos(v.y) / PI;
	float phiNormalized = (atan(v.x, v.z) / PI) * 0.5 + 0.5;
	return vec2(phiNormalized, thetaNormalized);
}

vec3 encode24(in vec3 p) {
	return vec2To2Unorm12sEncodedAs3Unorm8sInVec3Format(sphericalEncode(p));
}

void main()
{
	_normal_shininess.xyz = encode24(_normal);
	_normal_shininess.w = _shininess;

	_diffuse_specular.xyz = Compute_Color_Texture(_diffuseColor, _diffuseTexture);
	_diffuse_specular.w = _shininess;

	_emissive_materailMask.xyz = Compute_Color_Texture(_emissiveColor, _emissiveTexture);
}