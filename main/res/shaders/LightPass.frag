
#define PI 3.14159265

float unpackUnorm12(float u) {
	return u / 4095.0;
}

float packUnorm12Float(float f) {
	return round(clamp(f, 0.0, 1.0) * float(4095));
}

vec2 twoNorm12sEncodedAsUVec3InVec3FormatToPackedVec2(vec3 v) {
	vec2 s;
	// Roll the (*255s) in during the quasi bit shifting. This causes two of the three multiplications to happen at compile time
	float temp = v.y * (255.0 / 16.0);
	s.x = v.x * (255.0*16.0) + floor(temp);
	s.y = fract(temp) * (16 * 256) + (v.z * 255.0);
	return s;
}


vec2 twoUnorm12sEncodedAsUVec3InVec3FormatToVec2(vec3 v) {
	vec2 s = twoNorm12sEncodedAsUVec3InVec3FormatToPackedVec2(v);
	return vec2(unpackUnorm12(s.x), unpackUnorm12(s.y));
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

vec3 sphericalDecode(in vec2 p) {
	float theta = p.y * PI;
	float phi = (p.x * (2.0 * PI) - PI);

	float sintheta = sin(theta);
	return vec3(sintheta * sin(phi), cos(theta), sintheta * cos(phi));
}

vec2 sphericalEncode(in vec3 v) {
	// Assuming rho is always 1 (normalized v)
	float thetaNormalized = acos(v.y) / PI;
	float phiNormalized = (atan(v.x, v.z) / PI) * 0.5 + 0.5;
	return vec2(phiNormalized, thetaNormalized);
}

vec3 decode24(in vec3 p) {
	vec2 v = twoUnorm12sEncodedAsUVec3InVec3FormatToVec2(p);
	return sphericalDecode(v);
}

vec3 encode24(in vec3 p) {
	return vec2To2Unorm12sEncodedAs3Unorm8sInVec3Format(sphericalEncode(p));
}

void unProj(float depth, vec2 sp, mat4 mvpInverse, out vec4 worldPos)
{
	///世界坐标
	depth = 2.0 * depth - 1;///映射到 -1,1
	worldPos = mvpInverse * vec4(sp * 2 - 1, depth, 1);
	worldPos.xyzw /= worldPos.w;
}

layout(location = 0) out vec3 _color;

uniform vec2 _screenSize;

#if defined EMISSIVE_RENDER

uniform sampler2D _depthTexture;
uniform sampler2D _emissiveTexture;

void main()
{
	vec2 sp = gl_FragCoord.xy / _screenSize;///0-1

	///深度
	float depth = texture(_depthTexture, sp).x;
	gl_FragDepth = depth;

	if (depth <= 0 || depth >= 1)
		discard;

	_color = texture(_emissiveTexture, sp).xyz;
}

#else

uniform sampler2D _depthTexture;
uniform sampler2D _normal_shininessTexture;
uniform sampler2D _diffuse_specularTexture;

uniform vec3 _light_ambientColor = vec3(0, 0, 0);
uniform vec3 _light_diffuseColor = vec3(1, 1, 1);

#if (defined POINT_LIGHT || defined SPOT_LIGHT)

uniform mat4 _mv;
uniform mat4 _mvpInverse;
uniform vec3 _lightPoint_eye;
uniform float _lightRange;

float computeLightDir(float depth, vec2 sp, out vec3 lightDir)
{
	///世界坐标
	depth = 2.0 * depth - 1;///映射到 -1,1
	vec4 vertexPosition = _mvpInverse * vec4(sp * 2 - 1, depth, 1);
	vertexPosition.xyzw /= vertexPosition.w;

	///视图下的坐标
	vertexPosition = _mv * vertexPosition;
	lightDir = normalize(vertexPosition.xyz - _lightPoint_eye);

	float range = distance(_lightPoint_eye, vertexPosition.xyz);
	float dist = range < _lightRange/2.0 ? 1.0 - pow((clamp(range, 0.0, _lightRange) / _lightRange), 2.0) : -1;

	dist = dist > 0 ? 1 : dist;///去掉衰减
	return dist;
}

#elif(defined DIRECT_LIGHT)

uniform vec3 _lightDir_eye;

float computeLightDir(float depth, vec2 sp, out vec3 lightDir)
{
	lightDir = _lightDir_eye;
	return 1;
}

#endif

vec3 getNormal(vec4 normal_shininess)
{
	return decode24(normal_shininess.xyz);
}

void main()
{
	vec2 sp = gl_FragCoord.xy / _screenSize;///0-1

	///深度
	float depth = texture(_depthTexture, sp).x;
	gl_FragDepth = depth;

	if (depth <= 0 || depth >= 1)
		discard;

	vec3 lightDir;
	float dist = computeLightDir(depth, sp, lightDir);

	if(dist == -1)
		discard;

	vec4 normal_shininess = texture(_normal_shininessTexture, sp);
	vec4 diffuse_specular = texture(_diffuse_specularTexture, sp);

	vec3 eyeNormal = getNormal(normal_shininess);
	vec3 diffuseColor = diffuse_specular.xyz;
	float shininess = normal_shininess.w;
	
	///环境光
	_color += _light_ambientColor;

	////漫反射
	float diff = max(0.0, dot(eyeNormal, lightDir));

	_color += diffuseColor * _light_diffuseColor * dist * diff;

	if (diff > 0)
	{
		vec3 ref = normalize(reflect(-lightDir, eyeNormal));
		float spec = max(0.0, dot(eyeNormal, ref));

		float spec_ = pow(spec, shininess * 128);
		_color += dist * vec3(spec_, spec_, spec_);
	}
}

#endif