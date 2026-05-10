import QtQuick
import QtQuick.Controls

Item {
    id: root
    anchors.fill: parent

    property alias angle: rot.angle
    property alias frontImageSource: frontSide.source
    property alias backImageSource: backSide.source

    // ✅ syncAngle теперь всегда синхронизирован с rot.angle (в градусах)
    readonly property alias syncAngle: rot.angle

    function toggleFlip() {
        flipSimpleAnimation.start()
    }

    transform: Rotation {
        id: rot
        axis { x: 0; y: 1; z: 0 }
        angle: 0
        // Rotation по умолчанию вращает вокруг точки (0, 0) — левый верхний угол.
        // Чтобы вращать вокруг центра, нужно задать свойство origin.
        // КРИТИЧНО: точка вращения — центр элемента
        origin.x: root.width / 2
        origin.y: root.height / 2
    }

    // Два параллельных элемента: лицевая сторона и задняя, с изменением прозрачности при повороте
    Image {
        id: frontSide
        anchors.fill: parent
        anchors.margins: 2
        fillMode: Image.PreserveAspectFit
        smooth: true
        sourceSize: Qt.size(root.width , root.height)
        visible: rot.angle <= 90
        opacity: 1 - Math.abs(rot.angle - 0) / 90  // плавное исчезновение
    }

    Image {
        id: backSide
        anchors.fill: parent
        anchors.margins: 2
        fillMode: Image.PreserveAspectFit
        smooth: true
        sourceSize: Qt.size(root.width , root.height)
        visible: rot.angle >= 90
        opacity: (rot.angle - 90) / 90
    }

    Rectangle{
        id:borderRect
        anchors.fill: parent
        color: "transparent"
        border.color: "darkgrey"   // ✅ Рамка теперь здесь
        border.width: 2
        radius: 6
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
