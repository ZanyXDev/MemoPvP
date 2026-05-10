import QtQuick
import QtQuick.Controls

Item {
    id: root
    anchors.fill: parent

    property alias angle: rot.angle
    property alias frontImageSource: frontSide.source
    property alias backImageSource: backSide.source

    property bool showEmptyCard
    // ✅ Флаг: заблокирован ли ввод (пока карточка переворачивается обратно)
    property bool isFlippingBack: false

    transform: Rotation {
        id: rot
        axis { x: 0; y: 1; z: 0 }
        angle: 180 // default show backSideCard
        // Rotation по умолчанию вращает вокруг точки (0, 0) — левый верхний угол.
        // Чтобы вращать вокруг центра, нужно задать свойство origin.
        // КРИТИЧНО: точка вращения — центр элемента
        origin.x: root.width / 2
        origin.y: root.height / 2
    }

    onShowEmptyCardChanged: {
        // Игрок выбрал две одинаковые карты, движок пометил их как удаленные
        if (showEmptyCard){
            if (flipSimpleAnimation.running) {
                flipSimpleAnimation.stop()
            }
            if (flipBackAnimation.running){
                flipBackAnimation.stop()
            }
            // Небольшая задержка для плавности перехода
            Qt.callLater(() => {
                             baseRect.visible = false
                             frontSide.visible = false
                             backSide.visible = false
                         })
        }
    }
    // Два параллельных элемента: лицевая сторона и задняя, с изменением прозрачности при повороте
    Image {
        id: frontSide
        anchors.fill: parent
        anchors.margins: 2
        fillMode: Image.PreserveAspectFit
        smooth: true
        sourceSize: Qt.size(root.width , root.height)
        visible: (rot.angle <= 90) && (!root.showEmptyCard)
        opacity: 1 - Math.abs(rot.angle - 0) / 90  // плавное исчезновение
    }

    Image {
        id: backSide
        anchors.fill: parent
        anchors.margins: 2
        fillMode: Image.PreserveAspectFit
        smooth: true
        sourceSize: Qt.size(root.width , root.height)
        visible: (rot.angle >= 90) && (!root.showEmptyCard)
        opacity: (rot.angle - 90) / 90
    }

    Rectangle{
        id:baseRect
        anchors.fill: parent
        color: root.showEmptyCard ? "lightgrey" :"transparent"
        border.color: "darkgrey"   // ✅ Рамка теперь здесь
        border.width: 2
        radius: 6
    }
    // ---------------------- non visual  items -------------------------------
    // ✅ Таймер авто-возврата
    Timer {
        id: autoFlipTimer
        interval: 1500  // 1500 мс
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered: {
            // Если карточка уже в исходном положении — ничего не делаем
            if (rot.angle === 180) return

            // ✅ Помечаем, что идёт процесс возврата
            isFlippingBack = true

            // Запускаем анимацию возврата в исходное положение
            flipBackAnimation.start()
        }
    }
    // --------------------- Animation ---------------------------------------------

    NumberAnimation {
        id: flipSimpleAnimation
        target: rot
        property: "angle"
        to: rot.angle === 0 ? 180 : 0
        duration: 600
        easing.type: Easing.InOutQuad
        // ✅ Если пользователь кликнул во время анимации — сбрасываем таймер
        onStarted: {
            if (autoFlipTimer.running) {
                autoFlipTimer.stop()
            }
            isFlippingBack = false
        }

    }

    // ✅ Анимация возврата (отдельная, для чёткого контроля)
    NumberAnimation {
        id: flipBackAnimation
        target: rot
        property: "angle"
        to: 180  // Всегда возвращаем в 180 (Показываем рубашку карточки)
        duration: 400  // Быстрее, чем переворот
        easing.type: Easing.InOutQuad

        onFinished: {
            // ✅ Сбрасываем флаг после завершения
            isFlippingBack = false
        }
    }

    // --------------------- JS function -------------------------------------------
    function toggleFlip() {
        // Если уже идёт авто-возврат — игнорируем клик
        if (isFlippingBack) return

        // Запускаем анимацию переворота
        flipSimpleAnimation.start()

        // ✅ Запускаем таймер авто-возврата
        autoFlipTimer.start()
    }
}
