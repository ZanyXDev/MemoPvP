import QtQuick
import QtQuick.Controls

Rectangle {
    id: simpleCard
    property alias angle: rot.angle
    property url frontImageSource: root.frontImageSource
    property color backColor: root.backColor
    // ✅ syncAngle теперь всегда синхронизирован с rot.angle (в градусах)
    readonly property alias syncAngle: rot.angle

    function toggleFlip() {
        flipSimpleAnimation.start()
    }

    signal clicked

    transform: Rotation {
        id: rot
        axis { x: 0; y: 1; z: 0 }
        angle: 0
    }

    // Два параллельных элемента: лицевая сторона и задняя, с изменением прозрачности при повороте
    Image {
        id: frontSide
        source: frontImageSource
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        visible: rot.angle <= 90
        opacity: 1 - Math.abs(rot.angle - 0) / 90  // плавное исчезновение
    }

    Rectangle {
        id: backSide
        anchors.fill: parent
        color: backColor
        visible: rot.angle >= 90
        opacity: (rot.angle - 90) / 90
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
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
