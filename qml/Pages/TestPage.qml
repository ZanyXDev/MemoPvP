import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import src.core
import ImageProviderCore

Page{
    id:root
    anchors.fill: parent
    enabled: StackView.status  === StackView.Active
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
                    delegate: Rectangle{
                        height: 82
                        width:  82
                        radius: 4
                        border.color: "darkgrey"
                        border.width: 2
                        color:"transparent"
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
            // ✅ Правильно:
            onClicked: {
                GameEngine.gameModeEasy = !GameEngine.gameModeEasy
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
