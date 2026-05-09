import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.folderlistmodel
import src.core


Page {
    id:root
    anchors.fill: parent
    enabled: StackView.status  === StackView.Active
    ColumnLayout{
        id:mainLayout
        ComboBox{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 40
            model:GameEngine.imagePacks
        }

        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 160
        }

        Image{
            id:logoImg
            Layout.preferredHeight: 110
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            sourceSize.width: 360
            sourceSize.height: 110
            smooth:true
            source:"qrc:qt/qml/io/github/zanyxdev/memopvp/assets/images/memory_logo.png"
            fillMode: Image.PreserveAspectFit
        }
        Text {
            id: appVerText
            text: "v." + AppConfig.appVersion
            color: "red"
            z: 1
            opacity: 1.0
            //  visible: false

        }
        Text {
            text: "Количество: " + folderModel.count
        }
        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 260
        }
    }
    Component.onCompleted: {
        console.log(`Page onCompleted`)
    }

    FolderListModel {
        id: folderModel
        // Укажите путь к ресурсу с "qrc:"

        folder:"qrc:qt/qml/io/github/zanyxdev/memopvp/assets/images/"

        nameFilters: ["*.png", "*.jpg"]
        showDirs: false
    }


}
