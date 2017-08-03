
in vec2 _coord;

uniform sampler2D _shadowTexture;
uniform vec3 _shadowColor = vec3(0,0,0);

out vec4 _depthColor;

void main()
{
	float z = texture(_shadowTexture,_coord).x;

	if(gl_FragCoord.z > z + 0.0005)
	{
		_depthColor.xyz = _shadowColor;
		_depthColor.w = 0.7;
	}
	else
		discard;
}