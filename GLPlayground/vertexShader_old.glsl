#version 150

/* Default precision */
precision highp float; // highp = "au moins 32bits". on aurait pu mettre lowp ou mediump par exemple.

in vec3 in_vertex;
in vec3 in_norm;

out vec4 pass_normal;
out vec4 pass_position;

uniform float u_time;
uniform vec2 u_mouse;
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_proj_matrix;

void main(){
    // le "in_norm" que l'on recoit est en model space. Or on en a besoin en world space.
    pass_normal = u_model_matrix * vec4(in_norm, 0);
    // idem pour position
    pass_position = u_model_matrix * vec4(in_vertex, 1);
    
    gl_Position = u_proj_matrix * (u_view_matrix * (u_model_matrix * vec4(in_vertex, 1.0)));
}