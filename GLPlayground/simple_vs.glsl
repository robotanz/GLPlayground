#version 150

/* Default precision */
precision highp float; // highp = "au moins 32bits". on aurait pu mettre lowp ou mediump par exemple.

in vec3 in_vertex;
in vec3 in_normal;

out vec4 position;
out vec4 normal;

uniform float u_time;
uniform vec2 u_mouse;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_proj_matrix;

void main(){

   position = u_model_matrix * vec4(in_vertex, 1.0);
   normal = u_model_matrix * vec4(in_normal, 0.0);

   gl_Position = u_proj_matrix 
			   * u_view_matrix 
			   * u_model_matrix 
			   * vec4(in_vertex, 1.0);
}