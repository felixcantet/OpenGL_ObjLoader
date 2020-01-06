attribute vec3 a_position;
attribute vec3 a_color;
attribute vec2 a_texcoords;
attribute vec3 a_normals;

varying vec4 v_color;
varying vec2 v_texcoords;
varying vec3 v_normals;
varying vec3 v_pos;

uniform float u_time;
uniform mat4 u_matrix;
uniform mat4 u_rotMatrix;
uniform mat4 u_translateMatrix;
uniform mat4 u_projectionMatrix;


void main(void)
{
	mat4 worldMat =  u_translateMatrix * u_rotMatrix* u_matrix;
	mat4 normalMat = transpose(inverse(worldMat));
	v_color = vec4(a_color, 1.0);
	
	v_texcoords = a_texcoords;
	gl_Position = u_projectionMatrix * u_translateMatrix * u_rotMatrix* u_matrix * vec4(a_position, 1.0);
	v_normals = normalMat * vec4(a_normals, 1.0);
	v_pos = gl_Position;
}