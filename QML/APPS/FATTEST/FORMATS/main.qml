import QtQuick 2.0
import "../../../LIB"
import SMO 1.0

SmoFolder {
    id: fold
    folderModel: smo.folderModel
}


//Rectangle {
//    gradient: Gradient {
//        GradientStop { position: 0; color: "steelblue" }
//        GradientStop { position: 1; color: "black" }
//    }

//    Rectangle {
//        property int d: 100
//        id: square
//        width: d
//        height: d
//        anchors.centerIn: parent
//        color: "green"
//        NumberAnimation on rotation { from: 360; to: 0; duration: 4000; loops: Animation.Infinite; }
//    }

//    Text {
//        anchors.centerIn: parent
//        text: "Qt Quick running in a dock widget"
//        color: "purple"
//        font.bold: true
//        font.pointSize: 14
//    }
//}
