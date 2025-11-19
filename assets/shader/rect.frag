#version 420

in vec2 uv;
out vec4 color;

uniform sampler2D texSampler;

void main(){
    color = vec4(0, 0, 0, texture(texSampler, uv / 4096).r);
}