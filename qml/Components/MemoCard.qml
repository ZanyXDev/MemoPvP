import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import io.github.zanyxdev.memopvp

Item{
    id:root

    property url frontImageSource
    property url backImageSource
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
        // ✅ Передаем свойства сразу при создании элемента
        onItemChanged: {
            if (item) {
                item.frontImageSource = root.frontImageSource
                item.backImageSource = root.backImageSource
                if (appWnd.isDebugMode){
                    // ✅ Проверяем размеры ПОСЛЕ завершения компоновки
                    Qt.callLater(function() {
                        console.log(`[UI.DEBUG] POST-LAYOUT: ` +
                                    `loader: ${loader.width}×${loader.height}, ` +
                                    `item: ${item.width}×${item.height}`)
                    })
                }
            }
        }
    }

    MouseArea {
        ///TODO Добавить ховер и другие варианты
        id:mArea
        anchors.fill: parent
        onClicked: {
            root.clicked()
            if (loader.item){
                loader.item.toggleFlip()
            }
        }
    }
}