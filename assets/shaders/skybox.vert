#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    // Remove the translation part of the view matrix so the skybox doesn't move with the camera
    mat4 view_no_translation = mat4(mat3(view)); 
    vec4 pos = projection * view_no_translation * vec4(aPos, 1.0);
    // Force the depth to be the maximum value so it's always in the background
    gl_Position = pos.xyww;
}