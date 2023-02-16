import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import SMO 1.0

RowLayout {
    id: smoRowDelegate
    property int preferredInputWidth: 150
    property int rowTextSize: 15
    property alias rowHeight: smoInviter.height
    spacing: 2

    Image {
        source: "/media/sf_VMshare/Examples/Icons.png/data.png"
        Layout.preferredHeight: smoRowDelegate.rowHeight
    }

    Text {
        id: smoInviter
        text: smo.objectName
        rightPadding: 88
        font.pointSize: smoRowDelegate.rowTextSize
    }

    TextField {
        id: inBox
        font.pointSize: rowTextSize
        Layout.fillWidth: true
        Layout.minimumWidth: 22
        horizontalAlignment: TextInput.AlignLeft
        Layout.preferredWidth: smoRowDelegate.preferredInputWidth
        Layout.preferredHeight: smoRowDelegate.rowHeight
        text: smo.data === undefined ? "undefined" : smo.data
    }

    ZButton {
        text: "write"
        Layout.preferredHeight: smoRowDelegate.rowHeight
        onClicked: {
            var result = smo.writeText(inBox.text);
            console.log(text, smo.objectName, "with:", inBox.text, "clicked! Result:", result);
        }
    }
} //Row

