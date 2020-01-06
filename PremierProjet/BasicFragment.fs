struct Material{
	sampler2D diffuse;
	float specular;
	float shininess;
};

struct DirectionalLight{
	vec3 direction;
	vec3 ambiant;
	vec3 diffuse;
	vec3 specular;

	float intensity;
};

varying vec4 v_color;
varying vec2 v_texcoords;
varying vec3 v_normals;
varying vec3 v_pos;

uniform sampler2D u_sampler;
uniform vec3 viewPos;
uniform DirectionalLight u_light;
uniform Material u_material;



void main(void) {
	u_light.intensity = 1;
	u_light.direction = vec3(1, -1, 0.5);
	u_light.ambiant = vec3(0.3, 0.255, 0.336);
	u_light.diffuse = vec3(1.0, 1.0, 1.0);
	u_light.specular = vec3(1.0, 0.0, 0.0);
	//u_material.specular = 1.0;
	//u_material.shininess = 32;
	vec3 lightDir = normalize(-u_light.direction);

	// Ambiant
	vec3 ambiant = u_light.intensity * u_light.ambiant;
	
	// Diffuse
	vec3 norm = normalize(v_normals);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_light.diffuse * u_light.intensity * diff * texture(u_material.diffuse, v_texcoords).rgb;

	// specular
	// viewPos is (0, 0, 0) here
	vec3 viewDir = normalize(vec3(0,0,0) - v_pos);
	vec3 h = normalize(lightDir + viewDir);
	float spec = pow(max(dot(viewDir, h), 0.0), u_material.shininess);
	vec3 specular = u_light.specular * spec * u_material.specular;

	//vec4 textureColor = texture2D(u_sampler, v_texcoords);

	// Compute final color (ambiant + diffuse) * base color
	vec3 result = ambiant + diffuse + specular;
	// output final color
	gl_FragColor = vec4(result, 1.0);
}