
uniform int _showType;
uniform sampler2D _depthTexture;
uniform sampler2D _normal_shininessTexture;
uniform sampler2D _diffuse_specularTexture;
uniform sampler2D _emissiveTexture;
uniform mat4 _vpInverse;

in vec2 _screenP;

out vec3 _color;

vec3 getNormal(vec4 normal_shininess)
{
	return normal_shininess.xyz * 2 - vec3(1, 1, 1);
}

void main()
{
	vec2 sp = _screenP * 0.5 + 0.5;
	float depth = texture(_depthTexture, sp).x;

	if (depth <= 0 || depth >= 1)
		discard;

	gl_FragDepth = depth;

	if (_showType == 1)//深度
	{
		_color = vec3(pow(depth, 4096));
	}
	else if (_showType == 2)//位置
	{
		depth = depth * 2 - 1;///映射到 -1,1
		vec4 vertexPosition = _vpInverse * vec4(_screenP, depth, 1);
		vertexPosition.xyz /= vertexPosition.w;

		_color = normalize(vertexPosition.xyz);
		_color = _color.xyz * 0.5 + 0.5;
	}
	else if (_showType == 3)//法线
	{
		vec4 normal_shininess = texture(_normal_shininessTexture, sp);
		_color = normal_shininess.xyz;
	}
	else if (_showType == 4)//漫反射
	{
		vec4 diffuse_specular = texture(_diffuse_specularTexture, sp);
		_color = diffuse_specular.xyz;
	}
	else if (_showType == 5)//自发光
	{
		_color = texture(_emissiveTexture, sp).xyz;
	}
	else if (_showType == 6)//屏幕坐标
	{
		_color.xy = sp;
		_color.z = 1;
	}
	else
		discard;
}