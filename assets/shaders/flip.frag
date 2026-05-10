#version 440
layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 qt_FragColor;
precision mediump float;

layout(binding = 1) uniform sampler2D frontTex;

void main() {
    // ✅ Прямой вывод текстуры без модификаций
    qt_FragColor = texture(frontTex, qt_TexCoord0);
}