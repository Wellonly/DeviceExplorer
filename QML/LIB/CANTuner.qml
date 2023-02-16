import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

Rectangle {
    id: root
    color: Qt.application.active ? "lightblue" : "lightgray"
    border.color: "#1bc418"
    border.width: 2
    radius: 10
    objectName: "CANTunerRoot"
    property alias canModel: listView.model

    ScrollView {
        id: scroll
        anchors.fill: parent
        style: ScrollViewStyle {}

        ListView {
            id: listView
            anchors.fill: parent
            topMargin: 3
            delegate: canDelegate
            spacing: 3
            header: CANTuneHeader { folderItem: root }
            footer: Text { id: statusInFooter; color: "#a05858"; text: " --- " ; font.pointSize: 22}
        } //ListView
    } //ScrollView

    Component {
        id: canDelegate

        CANTuneRow {
        }
    }

} //Rectangle
