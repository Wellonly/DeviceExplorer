import QtQuick 2.7
import Qt.labs.platform 1.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

Rectangle {
    id: headr
    color: Qt.application.active ? "lightblue" : "lightgray"
    radius: 10
    Layout.alignment: Qt.AlignHCenter
    Layout.preferredHeight: 30

    property string appName: "0"

    signal nextClicked

    RowLayout {
        spacing: 15
        Text {
            text: " Демонстрация приложений для: <b>" + smo.objectName + "</b>; App №: " + headr.appName
        }
        ZButton {
            text: "Next"
            onClicked: {
                headr.nextClicked();
            }
        }
    } //RowLayout
}
