import QtQuick
import QtQuick.Controls

import io.github.zanyxdev.memopvp
import io.github.zanyxdev.memopvp.myplugin
import ImageProviderCore

ApplicationWindow {
    id: appWnd

    // ----- Property Declarations
    // Required properties should be at the top.
    readonly property int screenOrientation: Qt.PortraitOrientation

    property var screenWidth: Screen.width
    property var screenHeight: Screen.height
    property var screenAvailableWidth: Screen.desktopAvailableWidth
    property var screenAvailableHeight: Screen.desktopAvailableHeight

    // Свойство для версии приложения
    property string appVersion
    // Свойство-флаг для мобильной платформы
    property bool isMobile: false // Можно задать значение по умолчанию
    // Свойство-флаг для режима отладки
    property bool isDebugMode: false // Можно задать значение по умолчанию

    property FontLoader buiraFont: FontLoader {
        id: buiraFont
        source: "qrc:/qt/qml/assets/fonts/Buira/Buira.otf"
    }

    // ----- Signal declarations

    // ----- Size information
    width: (screenOrientation === Qt.PortraitOrientation) ? 360 : 640
    height: (screenOrientation === Qt.PortraitOrientation) ? 640 : 360
    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width
    // ----- Then comes the other properties. There's no predefined order to these.
    visible: true
    visibility: (isMobile) ? Window.FullScreen : Window.Windowed
    flags: Qt.Dialog

    // ----- Qt provided visual children
    background: Rectangle {
        id: background
        anchors.fill: parent
        color: "lightgrey"
    }
    Component.onCompleted: {
        if  (appWnd.isDebugMode){
            console.log(`[DEV.UI.Main] Info: ${buildQtVersion}`)
        }
    }

    Column {
        anchors.fill: parent
        width: 400; height: 300
        anchors.centerIn: parent
        spacing: 12

        Text {
            text: "CurrentPackNAme: " + ImageDataManager.currentPackName
            font.pixelSize: 24
        }

        Text { text: "Counter: " + counter.value; font.pixelSize: 24 }
        Button {
            text: "Increment"
            onClicked: counter.value++
        }
        Button {
            text: "Decrement"
            onClicked: counter.value--
        }
        Image{
            id:testImage
            width: 82
            height: 82
            source: "image://imagepack/animals/1"
        }
    }

    //--------------------- non Visual items -------------------------------------

    MyCounter {
        id: counter
        value: 42
        onValueChanged: console.log("[QML] counter.value =", value)
    }
}
