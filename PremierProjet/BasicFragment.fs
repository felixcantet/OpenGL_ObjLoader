varying vec4 v_color;
varying vec2 v_texcoords;
varying vec3 v_normals;
varying vec3 v_pos;

uniform sampler2D u_roughness;
uniform sampler2D u_sampler;



void main(void) {
	// Test Light integration

	// Diffuse lighting
	// define Light Color
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	// define light Position
	vec3 lightPos = vec3(5, 5, -5);
	float diffuseStrength = 1.0;
	vec3 norm = normalize(v_normals);
	//Compute normalized light direction
	vec3 lightDir = normalize(lightPos - v_pos);
	// Compute Lambert
	float diff = max(dot(norm, lightDir), 0.0) * diffuseStrength;
	// Add color to diffuse value
	vec3 diffCol = diff * vec3(1.0, 1.0, 1.0);
	
	// Ambiant Lighting

	// Define ambiant lighting intensity
	float ambiantStrength = 0.2;
	// Add Ambiant color
	vec3 ambiant = ambiantStrength * lightColor;

	// Specular color
	float specularStrength = 0.5;
	vec3 specularColor = vec3(1.0, 0.0, 0.0);
	vec3 viewDir = normalize(vec3(0,0,0) - v_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = max(dot(viewDir, reflectDir), 0.0);
	spec = pow(spec, 1);
	vec3 specular = specularStrength * spec * specularColor;

	vec4 textureColor = texture2D(u_sampler, v_texcoords);
	// Compute final color (ambiant + diffuse) * base color
	vec3 result = (diffCol + ambiant + specular) * textureColor;
	
	gl_FragColor = vec4(result, 1.0);

	// output final color
}