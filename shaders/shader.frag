#version 330 core

in vec3 color;
in vec2 texc;
out vec4 fragColor;

uniform sampler2D tex;
uniform int useTexture = 0;
uniform vec2 repeatUV;

void main(){
    vec3 texColor = texture(tex, texc*repeatUV).rgb;
    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
<<<<<<< HEAD
    fragColor = vec4(color * texColor, 0.3);
=======
    fragColor = vec4(color * texColor, 1);
>>>>>>> 7c397879dba1d4ef9f116c355614980cf7622de3
}
