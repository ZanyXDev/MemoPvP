// ShaderFlip.qml
import QtQuick

Item {
    id: root
    property alias angle: shaderEffect.angle
    property url frontImageSource
    property color backColor
    property real perspective: 0.8
    readonly property real syncAngle: shaderEffect.angle * 180 / Math.PI

    function toggleFlip() { flipAnimation.start() }
    signal clicked

    // ShaderFlip.qml — исправленный захват текстуры
    ShaderEffectSource {
        id: frontTexture
        // ✅ Явный размер
        width: Math.max(1, root.width > 0 ? root.width : 82)
        height: Math.max(1, root.height > 0 ? root.height : 82)

        // ✅ sourceItem с обработкой загрузки
        sourceItem: Image {
            id: frontImageItem
            source: frontImageSource
            width: frontTexture.width
            height: frontTexture.height
            fillMode: Image.PreserveAspectCrop
            smooth: true
            visible: false  // можно оставить, hideSource:true скроет оригинал
            asynchronous: false  // ✅ синхронная загрузка для надёжности

            onStatusChanged: {
                // Когда изображение загрузилось — принудительно обновляем текстуру
                if (status === Image.Ready) {
                    console.log("✅ Image loaded, scheduling texture update")
                    // Небольшая задержка для гарантии, что пайплайн готов
                    Qt.callLater(function() {
                        frontTexture.scheduleUpdate()
                    })
                } else if (status === Image.Error) {
                    console.error("❌ Image load failed:", frontImageSource)
                }
            }
        }

        hideSource: true
        live: true  // ✅ важно: отслеживать изменения
        format: ShaderEffectSource.RGBA

        // ✅ Дополнительная защита: обновить при изменении размера
        onWidthChanged: if (width > 0) scheduleUpdate()
        onHeightChanged: if (height > 0) scheduleUpdate()
    }
    // 🔹 Захват задней текстуры
    ShaderEffectSource {
        id: backTexture
        width: Math.max(1, root.width > 0 ? root.width : 82)
        height: Math.max(1, root.height > 0 ? root.height : 82)

        sourceItem: Rectangle {
            width: backTexture.width
            height: backTexture.height
            color: backColor
            visible: false
        }
        hideSource: true
        live: true
        format: ShaderEffectSource.RGBA
    }

    // 🔹 Сам шейдер
    ShaderEffect {
        id: shaderEffect
        anchors.fill: parent

        // ✅ Имена свойств ДОЛЖНЫ точно совпадать с uniform в шейдере
        property real angle: 0          // ↔ uniform float angle;
        property real persp: perspective // ↔ uniform float persp;
        property var frontTex: frontTexture  // ↔ layout(binding=1) uniform sampler2D frontTex;
        property var backTex: backTexture    // ↔ layout(binding=2) uniform sampler2D backTex;

        vertexShader: Qt.resolvedUrl("qrc:/qt/qml/shadereffects/assets/shaders/flip.vert.qsb")
        fragmentShader: Qt.resolvedUrl("qrc:/qt/qml/shadereffects/assets/shaders/flip.frag.qsb")

        // Принудительное обновление при изменении свойств
        onAngleChanged: update()
        onPerspChanged: update()
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }

    NumberAnimation {
        id: flipAnimation
        target: shaderEffect
        property: "angle"
        from: shaderEffect.angle
        to: shaderEffect.angle < Math.PI/2 ? Math.PI : 0
        duration: 600
        easing.type: Easing.InOutQuad
    }
    Component.onCompleted: {
        console.log("=== ShaderFlip Debug ===")
        console.log("frontTexture.status:", frontTexture.status,
                    "(0=Null, 1=Ready, 2=Error, 3=Undefined)")
        console.log("frontTexture.sourceItem.status:",
                    frontTexture.sourceItem?.status ?? "no sourceItem")


        // Проверка шейдера после инициализации
        Qt.callLater(() => {
            if (shaderEffect.effectVersion === 0) {
                console.error("❌ ShaderEffect failed to compile!")
            }
        })
    }

}