#version 150

/* Default precision */
precision highp float; // highp = "au moins 32bits". on aurait pu mettre lowp ou mediump par exemple.

in vec4 pass_normal;
in vec4 pass_position;

/* uniforms */
uniform float u_time;
uniform vec4 u_light[2 * 2];
uniform vec3 u_eye;

out vec4 out_Color;

float random(vec2 p){
    float tmp = sin(1234.26 * p.x - 1425.15 * p.y);
    tmp *= 1000.0;
    return fract(tmp); // on prend la partie fractionnaire. A partir du 4eme chiffre apres la virgule. A partir de la on admet que c'est quasi aleatoire.
}

float noise(vec2 p){
	vec2 p0 = floor(p); // floor prend la valeur entiere inferieure
	float r1 = random(p0);
	float r2 = random(p0 + vec2(1,0));
	float r3 = random(p0 + vec2(0,1));
	float r4 = random(p0 + vec2(1,1));
    
	vec2 u = p - p0;
	u = 3.0 * u * u - 2.0 * u * u; // evite l'interpolation lineaire. Adoucit l'interpolation
	float r13 = mix(r1, r3, u.y);
	float r24 = mix(r2, r4, u.y);
    
	return mix(r13, r24, u.x);
}

void main(){
	// il faut normaliser la normal recu en "in"
	vec3 normal = normalize(pass_normal.xyz);
	vec3 eyedir = normalize(u_eye - pass_position.xyz);
    
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
    
	for (int i = 0 ; i < 2; ++i){
		// on a besoin du vecteur de la lumiere
		vec3 lightdir;
		if (u_light[2 * i].w > 0.0){
			// c'est un point
			lightdir = normalize(pass_position.xyz - u_light[2 * i].xyz);
		}
		else{
			// c'est un directionnel
			lightdir = normalize(u_light[2 * i].xyz);
		}
		diffuse += dot(normal, -lightdir) * u_light[2 * i + 1].rgb;
		specular += pow(max(dot(reflect(lightdir, normal), eyedir), 0.0), u_light[2 * i + 1].a) * u_light[2 * i + 1].rgb;
	}
	
	vec3 illumination = diffuse + specular;
    
    out_Color = vec4(illumination, 1.0);
    //out_Color = vec4(1.0,1.0,0.0, 1.0);

}