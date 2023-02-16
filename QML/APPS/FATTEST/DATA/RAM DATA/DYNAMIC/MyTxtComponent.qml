import QtQuick 2.0

Text {
    text: "currentDate:" + currentDate
    font.italic: true
    font.bold: true
    font.pointSize: 15

    MouseArea {
        anchors.fill: parent
        onClicked: console.log("zvv:" + parent.text)
    }
}
