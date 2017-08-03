
in vec3 _vertexPosition;
in vec4 _vertexColor;

in float _rotationAngle;

uniform mat4 _mvp;

out mat2 _rotationMatrix;
out vec4 _color;

void main()
{
	float cos_r = cos(_rotationAngle);
	float sin_r = sin(_rotationAngle);
	_rotationMatrix = mat2(cos_r,sin_r,-sin_r,cos_r);

	gl_Position = vec4(_vertexPosition,1.0) * _mvp;
}