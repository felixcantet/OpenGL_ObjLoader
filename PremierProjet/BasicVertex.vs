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
	v_color = vec4(a_color, 1.0);
	
	v_texcoords = a_texcoords;
	mat4 worldMatrix = u_translateMatrix * u_rotMatrix;
	mat3 normalMat = transpose(inverse(worldMatrix));
										//                WORLD MATRIX			//
	gl_Position = u_projectionMatrix * u_translateMatrix * u_rotMatrix* u_matrix * vec4(a_position, 1.0);
	//v_normals = vec4(a_normals, 1.0);
	v_normals = normalMat * a_normals;
	v_pos = gl_Position;
}