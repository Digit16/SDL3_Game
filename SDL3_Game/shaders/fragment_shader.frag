#version 450

// Uniforms for texture and sampler
layout(set = 2, binding = 0) uniform sampler2D u_texture;  // Texture
layout(set = 2, binding = 1) uniform sampler u_sampler;    // Sampler

// Input structure
layout(location = 0) in vec4 v_color;    // Input color (from vertex shader)
layout(location = 1) in vec2 v_uv;       // Input texture coordinates (from vertex shader)

// Output structure
layout(location = 0) out vec4 o_color;   // Output color (to framebuffer)

void main() {
    // Sample the texture
    vec4 color = texture(u_texture, v_uv) * v_color;

    // Calculate grayscale value using the luminance formula
    float gray = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;

    // Set the output color to grayscale (with the same alpha value)
    o_color = vec4(gray, gray, gray, color.a);
}
