

in vec3 _screenPosition;
out vec2 _screenP;

void main()
{
	gl_Position.w = 1;
	gl_Position.xyz = _screenPosition.xyz;

	_screenP = _screenPosition.xy;
}