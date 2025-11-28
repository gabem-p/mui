#version 420

in vec2 position;
in vec2 uv;
in uvec2 size;
out vec4 color;

uniform sampler2D texSampler;
uniform vec4 rectColor;

void main(){
    color = texture(texSampler) * rectColor;
}