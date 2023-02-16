
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

RowLayout {
    id: rowDelegate
    property alias inviter: inviterText.text
    property alias dataField: textField.text

    property int preferredInputWidth: 150
    property int rowTextSize: 15
    property alias rowHeight: inviterText.height
    spacing: 2

//    Image {
//        source: "/media/sf_VMshare/Examples/Icons.png/data.png"
//        Layout.preferredHeight: rowDelegate.rowHeight
//    }

    Text {
        id: inviterText
        rightPadding: 88
        font.pointSize: rowDelegate.rowTextSize
    }

    TextField {
        id: textField
        font.pointSize: rowTextSize
        Layout.fillWidth: true
        Layout.minimumWidth: 22
        horizontalAlignment: TextInput.AlignLeft
        Layout.preferredWidth: rowDelegate.preferredInputWidth
        Layout.preferredHeight: rowDelegate.rowHeight
//        text: smo.data === undefined ? "undefined" : smo.data
    }

//    ZButton {
//        text: "write"
//        Layout.preferredHeight: smoRowDelegate.rowHeight
//        onClicked: {
//            var result = smo.writeText(inBox.text);
//            console.log(text, smo.objectName, "with:", inBox.text, "clicked! Result:", result);
//        }
//    }
} //Row
