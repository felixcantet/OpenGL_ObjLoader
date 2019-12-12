attribute vec2 a_position;
attribute vec3 a_color;
attribute vec2 a_texcoords;

varying vec4 v_color;
varying vec2 v_texcoords;

uniform float u_time;
uniform mat4 u_matrix;
uniform mat4 u_rotMatrix;
uniform mat4 u_translateMatrix;
uniform mat4 u_projectionMatrix;

void main(void)
{
	v_color = vec4(a_color, 1.0);
	v_color *= u_time;
	v_texcoords = a_texcoords;
	gl_Position = u_projectionMatrix * u_translateMatrix * u_rotMatrix* u_matrix * vec4(a_position, 0.0, 1.0);
}