import QtQuick
import QtQuick.Controls

import io.github.zanyxdev.memopvp
import io.github.zanyxdev.memopvp.myplugin

ApplicationWindow {
    id: appWnd

    // ----- Property Declarations
    // Required properties should be at the top.
    readonly property int screenOrientation: Qt.PortraitOrientation

    property var screenWidth: Screen.width
    property var screenHeight: Screen.height
    property var screenAvailableWidth: Screen.desktopAvailableWidth
    property var screenAvailableHeight: Screen.desktopAvailableHeight
    property bool isDebugMode: AppConfig.isDebugMode

    property FontLoader buiraFont: FontLoader {
        id: buiraFont
        source: "://qt/qml/assets/fonts/Buira/Buira.otf"
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
    visibility: (AppConfig.isMobile) ? Window.FullScreen : Window.Windowed
    flags: Qt.Dialog

    // ----- Qt provided visual children
    background: Rectangle {
        id: background
        anchors.fill: parent
        color: "lightgrey"
    }
    Component.onCompleted: {
        if  (appWnd.isDebugMode){
            console.log(`[DEV.UI.Main] Info: ${AppConfig.buildQtVersion}`)
        }
    }

    Column {
        width: 400; height: 300
        anchors.centerIn: parent
        spacing: 12

        Text { text: "Counter: " + counter.value; font.pixelSize: 24 }
        Button {
            text: "Increment"
            onClicked: counter.value++
        }
        Button {
            text: "Decrement"
            onClicked: counter.value--
        }
    }

    //--------------------- non Visual items -------------------------------------

    MyCounter {
        id: counter
        value: 42
        onValueChanged: console.log("[QML] counter.value =", value)
    }
}
