#version 420

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2)
out vec2 position;
out vec2 uv;

uniform vec2 screen;

void main(){
    gl_Position = vec4(vec2(inPosition.x, -inPosition.y) / (screen / 2) + vec2(-1, 1), 0, 1);
    position = gl_Position.xy;
    uv = inUV;
}