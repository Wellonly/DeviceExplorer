import QtQuick 2.0

Rectangle {
    id: button

    property bool checked: false
    property alias text: buttonText.text

    Accessible.name: text
    Accessible.description: "This button does " + text
    Accessible.role: Accessible.Button
    Accessible.onPressAction: button.clicked()

    signal clicked

    implicitWidth: buttonText.implicitWidth + 20
    implicitHeight: 30
    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: "grey"
        }
        GradientStop {
            position: 1.0
            color: button.focus ? "red" : "grey"
        }
    }

    radius: 5
    antialiasing: true

    Text {
        id: buttonText
        text: parent.description
        anchors.centerIn: parent
        font.pixelSize: parent.height * .5
        style: enabled ? Text.Sunken : Text.Normal
        color: enabled ? "white" : "#555"
        styleColor: "black"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked()
    }

    Keys.onSpacePressed: clicked()
}
