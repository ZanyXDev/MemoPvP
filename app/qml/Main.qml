import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import io.github.zanyxdev.memopvp
import io.github.zanyxdev.memopvp.myplugin
import io.github.zanyxdev.memopvp.core
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
    property string buildQtVersion
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
            console.log(`[DEV.UI.Main] Core.boardModel: ${Core.boardModel}`)
        }
    }

    Column {
        anchors.fill: parent
        // width: 400; height: 300
        anchors.centerIn: parent
        spacing: 4

        Text {
            text: "CurrentPackNAme: " + ImageDataManager.currentPackName
            font.pixelSize: 12
        }
        Image{
            id:testImage
            width: 82
            height: 82
            source: "image://imagepack/__animals/1"
        }
        Image{
            id:testImage2
            width: 128
            height: 82
            source: "image://imagepack/memory_logo.png"
        }
        Button{
            text: "hide"
            onClicked: {
                Core.setHideCard=false
            }
        }
        GridLayout {
            id: cellGridLayout
            anchors.centerIn: parent // <-- центрируем сам GridLayout
            columns:  4
            rowSpacing: 4
            columnSpacing:   4
            Repeater{
                id:gridRepeater
                model: Core.boardModel
                delegate: Rectangle{
                    visible: model.cardVisible
                    Layout.preferredWidth: 82
                    Layout.preferredHeight: 82
                    Layout.alignment: Qt.AlignHCenter
                    Text {
                        text: "id:"+model.pictureId
                        font.pixelSize: 8
                    }
                    CheckBox{
                        text: "State"
                        checked: model.cardVisible
                    }
                    //frontImageSource: "qrc:/assets/images/packs/animals/animals_0.png"
                    //frontImageSource:"image://imagepack/animals/1"
                    //backImageSource: "qrc:/assets/images/cardbackside.jpg"

                    //useShader: root.useShader

                }
            }
        }
    }

    //--------------------- non Visual items -------------------------------------

    MyCounter {
        id: counter
        value: 42
        onValueChanged: console.log("[QML] counter.value =", value)
    }
}
