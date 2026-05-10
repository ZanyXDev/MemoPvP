import QtQuick
import QtQuick.Controls

import src.core
import io.github.zanyxdev.memopvp
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
    property bool isDebugMode: AppConfig.isDebugMode
    property FontLoader buiraFont: FontLoader {
        id: buiraFont
        source: "qrc:qt/qml/io/github/zanyxdev/memopvp/assets/fonts/Buira/Buira.otf"
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
        console.log(`Info: ${AppConfig.buildQtVersion}`)
    }

    StackView {
        id: stack
        anchors.fill: parent
        InitPage{
            id:initPage
            Component.onCompleted: {
                console.log(initPage)
            }
        }

        TestPage{
            id:testPage
            Component.onCompleted: {
                console.log(initPage)
            }
        }
        initialItem: testPage

    }
}