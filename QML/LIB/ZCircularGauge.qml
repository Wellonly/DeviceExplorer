import QtQuick 2.7
import QtQuick.Extras 1.4

CircularGauge {
    value: accelerating ? maximumValue : 0
    anchors.centerIn: parent
    
    property bool accelerating: false
    
    Keys.onSpacePressed: accelerating = true
    Keys.onReleased: {
        if (event.key === Qt.Key_Space) {
            accelerating = false;
            event.accepted = true;
        }
    }
    
    Component.onCompleted: forceActiveFocus()
    
    Behavior on value {
        NumberAnimation {
            duration: 1000
        }
    }
}
