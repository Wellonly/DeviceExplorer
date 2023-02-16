import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import CANMarathon 1.0

RowLayout {
    id: smoRowDelegate
    property int preferredInputWidth: 150
    property int rowTextSize: 15
    property alias rowHeight: inviter.height
    spacing: 2

    function toNumber(v) {
        return parseInt(v);
    }

    Image {
        source: "../../Icons.png/data.png"
        Layout.preferredHeight: smoRowDelegate.rowHeight
    }

    Text {
        id: inviter
        text: name /*+ " (id:"+ pid +")"*/ /* model role */
        font.pointSize: smoRowDelegate.rowTextSize
        rightPadding: 11
        Layout.preferredWidth: 333
    }

    TextField {
        id: inBox
        font.pointSize: rowTextSize
        Layout.fillWidth: true
        Layout.minimumWidth: 22
        horizontalAlignment: TextInput.AlignLeft
        Layout.preferredWidth: smoRowDelegate.preferredInputWidth
        Layout.preferredHeight: smoRowDelegate.rowHeight
        text: "" // can.data === undefined ? "undefined" : can.data
    }

    ZButton {
        text: "read"
        Layout.preferredHeight: smoRowDelegate.rowHeight
        onClicked: {
            var result = Can.read(pid);
            inBox.text = result;
            console.log(text, name, "clicked! Result:", result, "pid:", pid);
        }
    }
    ZButton {
        text: "write"
        Layout.preferredHeight: smoRowDelegate.rowHeight
        onClicked: {
            var result = Can.write(pid, toNumber(inBox.text));
            console.log(text, name, "with:", inBox.text, "clicked! Result:", result, "pid:", pid);
        }
    }

    Connections {
        target: Can
        onIoError: {
            log.addError("CAN IO error:" + Can.lastCode());
        }
    }
    Connections {
        target: Can
        onAppWarning: {
            log.addWarning("CAN application warning:" + Can.lastCode());
        }
    }

} //Row

