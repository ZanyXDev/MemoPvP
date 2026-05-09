pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root
    width: 400; height: 400

    property string text: "MemoPvP"
    property real radius: 150
    property real fontSize: 24
    property color textColor: "white"
    property real startAngle: -90
    property real arcAngle: 180
    property alias font:innerText.font
    Text{
        id:innerText

    }

    Repeater {
        model: root.text.split('')
        delegate: Text {
            required property var modelData
            required property int index
            text: modelData
            font:root.font
            color: root.textColor

            property real angle: {
                var t = index / (root.text.length - 1 || 1)
                return root.startAngle + t * root.arcAngle
            }
            property real rad: angle * Math.PI / 180

            x: root.width / 2 + root.radius * Math.cos(rad) - contentWidth / 2
            y: root.height / 2 + root.radius * Math.sin(rad) - contentHeight / 2
            rotation: angle + 90
        }
    }
}