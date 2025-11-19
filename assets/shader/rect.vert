#version 420

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 inUV;
out vec2 uv;

uniform vec2 screen;

void main(){
    gl_Position = vec4(vec2(position.x, -position.y) / (screen / 2) + vec2(-1, 1), 0, 1);
    uv = inUV;
}
