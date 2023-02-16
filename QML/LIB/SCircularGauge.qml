import QtQuick 2.7
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4

CircularGauge {
    id: smoGauge
    anchors.centerIn: parent

    minimumValue: -5
    maximumValue: 5
    stepSize: 0.1
    tickmarksVisible: true

    style: CircularGaugeStyle {
        id: sCircularGaugeStyle
        labelStepSize: 1
        background: Item {
            Rectangle {
                width: parent.width
                height: parent.height
                radius: width / 2
                color: "#474040"
                anchors.centerIn: parent
            }
        }
    }

    Text {
        id: indexText
        text: "<b>Value: </b>" + Math.ceil(smoGauge.value)
        font.pointSize: 12
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        anchors.bottomMargin: -40

        color: "red"
    }

    Keys.onSpacePressed: {
        var smov = smo.data + 1;
        if (smov > 1) smov = -1;
        smo.data = smov;
        value = smov;
        console.log("CircularGauge: new smo value:", smov);
    }

    Behavior on value {
        NumberAnimation {
            duration: 111
        }
    }

    Connections {
        target: smo
        onDataChanged: {
            smoGauge.value = smo.data;
        }
    }

    Component.onCompleted: {
        value = smo.data //accelerating ? maximumValue : 0
        forceActiveFocus()
    }
}
