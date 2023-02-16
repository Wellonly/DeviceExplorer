import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import SMO 1.0

Rectangle {
    id: root
    color: Qt.application.active ? "lightblue" : "lightgray"
    border.color: "#1bc418"
    border.width: 2
    radius: 10
    property alias folderModel: listView.model

    ScrollView {
        id: scroll
        anchors.fill: parent
        style: ScrollViewStyle {}

        ListView {
            id: listView
            anchors.fill: parent
            topMargin: 3
            delegate: smoDelegate
            spacing: 3
            header: SmoListHeader { folderItem: folderModel.folderItem() }
            footer: Text { id: statusInFooter; color: "#a05858"; text: " --- " ; font.pointSize: 22}
        } //ListView
    } //ScrollView

    Component {
        id: smoDelegate

        SmoRow {
        }
    }

} //Rectangle

/* usage example...
import "../LIB"
import SMO 1.0

SmoFolder {
    id: fold
    folderModel: smo.folderModel
}
*/