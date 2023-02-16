import QtQuick 2.0

Rectangle {
    id: root
    color: "#ffffff"
    border.color: "#0c0d18"

    Rectangle {
        property int d: 100
        id: square
        width: d
        height: d
        anchors.centerIn: parent
        color: "blue"
        NumberAnimation on rotation { from: 0; to: 360; duration: 2000; loops: Animation.Infinite; }
    }

    Text {
        anchors.centerIn: parent
        text: "Platform OS:" + Qt.platform.os
        font.bold: true
        font.italic: true
        font.pointSize: 15
    }
}
