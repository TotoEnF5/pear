@vs vertex_shader

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coords;
layout (location = 2) in vec3 a_normal;

out vec2 tex_coords;
out vec3 normal;

layout (binding = 0) uniform vs_uniforms {
    mat4 u_mvp;
};

void main() {
    gl_Position = u_mvp * vec4(a_pos, 1.0);
    tex_coords = a_tex_coords;
    normal = a_normal;
}

@end

@fs fragment_shader

in vec2 tex_coords;
in vec3 normal;

out vec4 frag_color;

layout (binding = 0) uniform texture2D u_texture;
layout (binding = 0) uniform sampler u_sampler; 

void main() {
    frag_color = texture(sampler2D(u_texture, u_sampler), tex_coords);
}

@end

@program shader vertex_shader fragment_shader
