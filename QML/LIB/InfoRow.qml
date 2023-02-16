import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

RowLayout {
    id: infoRow
    property int preferredInviterWidth: 199
    property int preferredInputWidth: 199
    property int rowTextSize: 15
    property alias imageSource: image.source
    property alias isReadonly: textField.readOnly
    property alias rowHeight: inviter.height
    property alias inviteText: inviter.text
    property alias valueText: textField.text
    signal writing

    spacing: 2

    Image {
        id: image
        source: "../../Icons.png/info.png"
        Layout.preferredHeight: infoRow.rowHeight
    }

    Text {
        id: inviter
        text: "info row inviter:"
        font.pointSize: infoRow.rowTextSize
        rightPadding: 11
        Layout.preferredWidth: infoRow.preferredInviterWidth
    }

    TextField {
        id: textField
        readOnly: false
        font.pointSize: rowTextSize
        Layout.fillWidth: true
        Layout.minimumWidth: 22
        horizontalAlignment: TextInput.AlignLeft
        Layout.preferredWidth: infoRow.preferredInputWidth
        Layout.preferredHeight: infoRow.rowHeight
        opacity: readOnly ? 0.5 : 1
        text: ""
    }

    ZButton {
        text: "write"
        visible: !textField.readOnly
        Layout.preferredHeight: infoRow.rowHeight
        onClicked: {
            console.log(text, inviter.text, "with:", textField.text, "clicked!");
            infoRow.writing();
        }
    }
} //infoRow

