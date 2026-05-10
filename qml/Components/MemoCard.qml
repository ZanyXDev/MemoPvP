import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import io.github.zanyxdev.memopvp

Rectangle{
    id:root
    property url frontImageSource
    property color backColor: "darkblue"
    property bool useShader: false     // по умолчанию не используем шейдер
    signal clicked
    // угол поворота (0..180)
    // ✅ Read-only привязка: автоматически обновляется при изменении syncAngle
    readonly property real angle: loader.item ? loader.item.syncAngle : 0
    Loader {
        id: loader
        anchors.fill: parent
        source: (root.useShader ? "ShaderFlip.qml" : "SimpleFlip.qml")
    }


    function toggleFlip() {
        if (loader.item){
            loader.item.toggleFlip()
        }
    }
}