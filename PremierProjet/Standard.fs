varying vec4 v_color;
varying vec2 v_texcoords;
varying vec3 v_normals;
varying vec3 v_pos;

uniform sampler2D u_sampler;

struct SpecularLight{
	vec3 direction;
	vec3 color;
	vec3 strength;
};

struct AmbiantLight
{
	vec3 color;
	vec3 strength;
};

struct DiffuseLight{
	vec3 direction;
	vec3 color;
	vec3 strength;
}

uniform SpecularLight u_specularLight;

void main(void) {
	// Test Light integration

	// Diffuse lighting
	// define Light Color
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	// define light Position
	vec3 lightPos = vec3(10.0, 0.0, 10.0);
	//Compute normalized light direction
	vec3 lightDir = normalize(lightPos - v_pos);
	// Compute Lambert
	float diff = max(dot(v_normals, lightDir), 0.0);
	// Add color to diffuse value
	vec3 diffCol = diff * vec3(1.0, 1.0, 1.0);
	
	// Ambiant Lighting

	// Define ambiant lighting intensity
	float ambiantStrength = 0.2;
	// Add Ambiant color
	vec3 ambiant = ambiantStrength * lightColor;

	vec4 textureColor = texture2D(u_sampler, v_texcoords);

	// Compute final color (ambiant + diffuse) * base color
	vec3 result = (diffCol + ambiant) * textureColor;
	// output final color
	gl_FragColor = vec4(result, 1.0);
}