#version 420

in vec2 position;
in vec2 uv;
out vec4 color;

uniform sampler2D texSampler;
uniform vec4 textColor;

void main(){
    color = vec4(textColor.rgb, texture(texSampler, uv / 3000).r * textColor.a);
}
