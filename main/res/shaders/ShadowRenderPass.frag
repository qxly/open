
uniform sampler2D _depthTexture;
uniform sampler2D _shadowTexture;

uniform mat4 _mvpInverse;

uniform mat4 _lightShdowMapMVP;
uniform vec3 _shadowColor = vec3(0, 0, 0);

in vec2 _screenP;
out vec4 _depthColor;

void main()
{
	vec2 sp = _screenP * 0.5 + 0.5;
	float depth = texture(_depthTexture, sp).x;

	if (depth <= 0 || depth >= 1)
		discard;
	else
	{
		depth = depth * 2 - 1;///ӳ�䵽 -1,1

		vec4 vertexPosition = _mvpInverse * vec4(_screenP, depth, 1);
		vertexPosition.xyzw /= vertexPosition.w;

		vec4 coord = _lightShdowMapMVP * vertexPosition;//-1,1
		coord.xyz /= coord.w;

		if (coord.x < -1 || coord.x > 1 || coord.y < -1 || coord.y > 1 || coord.z < -1 || coord.z > 1)
			discard;

		coord.xyz = coord.xyz * 0.5 + vec3(0.5);///0-1

		float depth_shadow = texture(_shadowTexture, coord.xy).x;//0-1

		if (coord.z > depth_shadow + 0.0005)
		{
			_depthColor.xyz = _shadowColor;
			_depthColor.w = 0.8;
		}
		else
			discard;
	}
}