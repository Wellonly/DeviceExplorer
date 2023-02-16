import QtQuick 2.7
import Qt.labs.platform 1.0
import QtQuick.Extras 1.4
import "../../../../../LIB"


Rectangle {
    id: root
    border.color: "#1bc418"
    border.width: 2
    color: "#4674b4"
    radius: 10

    ZGauge {
    }

//    ZCircularGauge {
//    }

//    ZTumbler {

//    }
}

//Rectangle {
//    id: root
//    border.color: "#1bc418"
//    border.width: 2
//    color: "steelblue"
//    radius: 10

//    MessageDialog {
//        id: mess
//        buttons: MessageDialog.Ok
//        text: "The document has been modified."
//    }

//    Text {
//        text: "JulianDay:" + qdate
//        font.italic: true
//        font.bold: true
//        font.pointSize: 15

//        MouseArea {
//            anchors.fill: parent
//            onClicked: mess.open()
//        }
//    }
//}


/// NOT WORKED yet ..........................................................

//SystemTrayIcon {
//      visible: true
//      iconSource: "/media/sf_VMshare/Examples/Icons.png/group0.png"

//      menu: Menu {
//          MenuItem {
//              text: qsTr("Quit")
//              onTriggered: Qt.quit()
//          }
//      }
//  }
