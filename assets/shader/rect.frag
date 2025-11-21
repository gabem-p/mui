#version 420

in vec2 position;
in vec2 uv;
out vec4 color;

uniform sampler2D texSampler;
uniform bool text;

void main(){
    if(text)
        color = vec4(0, 0, 0, texture(texSampler, uv / 4096).r);
    else
        color = texture(texSampler, uv);
}