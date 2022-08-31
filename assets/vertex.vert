#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;

out vec3 vertexColor;
out vec2 textureCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main(){
    
    vertexColor = aColor;
    textureCoord = aTexture;

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

}