Texture2D u_texture : register(t0, space2);
SamplerState u_sampler : register(s0, space2);

struct PSInput {
    float4 v_color : COLOR0;
    float2 v_uv : TEXCOORD0;
};

struct PSOutput {
    float4 o_color : SV_Target;
};

PSOutput main(PSInput input) {
    PSOutput output;
    
    // Sample the texture
    float4 color = u_texture.Sample(u_sampler, input.v_uv) * input.v_color;
    
    // Add a fun effect: dynamic color change based on sine of UV coordinates
    float r = sin(input.v_uv.x * 10.0) * 0.5 + 0.5; // Sine wave modulation on X
    float g = sin(input.v_uv.y * 10.0) * 0.5 + 0.5; // Sine wave modulation on Y
    float b = sin((input.v_uv.x + input.v_uv.y) * 10.0) * 0.5 + 0.5; // Combined X and Y for dynamic effect
    
    // Apply fun color modulation
    output.o_color = float4(color.r * r, color.g * g, color.b * b, color.a);
    
    return output;
}
