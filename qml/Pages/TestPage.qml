import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import src.core
import ImageProviderCore
import io.github.zanyxdev.memopvp

Page{
    id:root
    anchors.fill: parent
    enabled: StackView.status  === StackView.Active
    property bool useShader : false   ///TODO need gets from GameEngine
    ColumnLayout{
        id:mainLayout
        spacing: 4
        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 60
        }
        // Image {
        //     Layout.fillWidth: true

        //     Layout.preferredHeight: 82
        //     source: "image://imagepack/yellow"
        // }
        Rectangle{
            Layout.preferredWidth: 360
            Layout.preferredHeight: 360
            radius: 4
            border.color: "darkgrey"
            border.width: 2
            GridLayout {
                id: cellGridLayout
                anchors.centerIn: parent // <-- центрируем сам GridLayout
                columns: GameEngine.gameModeEasy ? 3 : 4
                rowSpacing: GameEngine.gameModeEasy ? 6 : 4
                columnSpacing:  GameEngine.gameModeEasy ? 6 : 4
                Repeater{
                    id:gridRepeater
                    model:GameEngine.cellsInBoard;
                    delegate: MemoCard{
                        Layout.preferredWidth: 82
                        Layout.preferredHeight: 82
                        Layout.alignment: Qt.AlignHCenter

                        //frontImageSource: "qrc:/assets/images/packs/animals/animals_0.png"
                        frontImageSource:"image://imagepack/animals/1"
                        backImageSource: "qrc:/assets/images/cardbackside.jpg"

                        useShader: root.useShader

                    }
                }
            }

        }

        Button {
            id:easyHardMode
            Layout.preferredWidth: 60
            Layout.preferredHeight: 60
            Layout.alignment: Qt.AlignHCenter
            text: GameEngine.gameModeEasy ? qsTr("EASY"): qsTr("HARD")

            onClicked: {
                GameEngine.gameModeEasy = !GameEngine.gameModeEasy
            }
        }
        Button {
            id:useShader
            Layout.preferredWidth: 60
            Layout.preferredHeight: 60
            Layout.alignment: Qt.AlignHCenter
            text: root.useShader ? qsTr("Shader"): qsTr("Software")

            onClicked: {
                root.useShader= !root.useShader
            }
        }
        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 260
        }
    }
    Component.onCompleted: {
        console.log(`TestPage onCompleted`)
    }
}
