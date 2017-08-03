layout(location = 0) in vec3 _vertexPosition;
layout(location = 1) in vec3 _vertexNormal;
layout(location = 2) in int _color;

layout(location = 3) in vec2 _vertexTextureCoord;
//layout(location = 4) in vec2 _vertexTextureCoord1;
//layout(location = 5) in vec2 _vertexTextureCoord2;
//layout(location = 6) in vec2 _vertexTextureCoord3;

layout(location = 7) in mat4 _mvp;
layout(location = 11) in mat4 _normalMatrix;

out vec2 _varyVertexCoord;
out float _diff;
out float _spec;
out float _dist;

uniform float _shininess;
uniform mat4 _projectInverse;
uniform vec3 _lightPosition;
uniform float _lightRange;

vec3 ComputeLightDir_PointLight(vec3 lightPosition, float lightRange, out float dist)
{
	vec4 point4 = _projectInverse * _mvp * vec4(_vertexPosition, 1.0);
	vec3 point = point4.xyz / point4.w;

	float range = distance(lightPosition, point);
	dist = 1.0 - pow((clamp(range, 0.0, lightRange) / lightRange), 2.0);

	///眼坐标系下的光照方向
	return normalize(lightPosition - point);///定向光
}

vec3 computeLightDir(vec3 lightPosition, float lightRange, out float dist)
{
	vec3 p = lightRange > 0 ? ComputeLightDir_PointLight(lightPosition, lightRange, dist) : lightPosition;
	return p;
}

void main()
{
	gl_Position = _mvp * vec4(_vertexPosition, 1.0);
	_varyVertexCoord = _vertexTextureCoord;

	////计算眼坐标系下的法线、和顶点
	vec3 eyeNormal = vec4(_normalMatrix * vec4(_vertexNormal, 1.0f)).xyz;
	vec3 lightdir = computeLightDir(_lightPosition, _lightRange, _dist);
	_diff = max(0.0, dot(eyeNormal, lightdir));

	//高光
	vec3 ref = normalize(reflect(-lightdir, eyeNormal));
	_spec = max(0.0, dot(eyeNormal, ref));
	_spec = pow(_spec, _shininess * 128);
	_spec = _diff > 0 ? _spec : 0;
}