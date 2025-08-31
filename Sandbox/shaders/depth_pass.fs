#version 330

out vec4 outcolor;

void main()
{
    outcolor = vec4(vec3(gl_FragCoord.z), 1.0);
}
