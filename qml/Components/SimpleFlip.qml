import QtQuick
import QtQuick.Controls

Item {
    id: root
    anchors.fill: parent
    property alias angle: rot.angle
    property alias frontImageSource: frontSide.source
    property color backColor
    // ✅ syncAngle теперь всегда синхронизирован с rot.angle (в градусах)
    readonly property alias syncAngle: rot.angle

    function toggleFlip() {
        flipSimpleAnimation.start()
    }

    transform: Rotation {
        id: rot
        axis { x: 0; y: 1; z: 0 }
        angle: 0
    }

    // Два параллельных элемента: лицевая сторона и задняя, с изменением прозрачности при повороте
    Image {
        id: frontSide
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        visible: rot.angle <= 90
        opacity: 1 - Math.abs(rot.angle - 0) / 90  // плавное исчезновение

    }

    Rectangle {
        id: backSide
        anchors.fill: parent
        color: backColor
        border.color: "darkgrey"
        border.width: 2
        radius: 6

        visible: rot.angle >= 90
        opacity: (rot.angle - 90) / 90
    }
    NumberAnimation {
        id: flipSimpleAnimation
        target: rot
        property: "angle"
        to: rot.angle === 0 ? 180 : 0
        duration: 600
        easing.type: Easing.InOutQuad
    }
}
