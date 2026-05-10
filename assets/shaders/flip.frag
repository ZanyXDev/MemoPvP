#version 440
layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 qt_FragColor;
layout(binding = 1) uniform sampler2D frontTex;

void main() {
    // ✅ Тест: если текстура работает — увидим шахматный паттерн + изображение
    vec2 uv = qt_TexCoord0;
    vec4 tex = texture(frontTex, uv);

    // Шахматная сетка для отладки
    if (tex.a < 0.1) {
        qt_FragColor = vec4(mod(floor(uv.x * 10.0) + floor(uv.y * 10.0), 2.0) * 0.5 + 0.5);
    } else {
        qt_FragColor = tex;
    }
}