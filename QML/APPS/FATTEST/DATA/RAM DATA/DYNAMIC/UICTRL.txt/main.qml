import QtQuick 2.7
import Qt.labs.platform 1.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import "../../../../../../LIB"
//import "tumbler_fold"

Rectangle {
    id: root
    border.color: "#1bc418"
    border.width: 2
    color: Qt.application.active ? "lightblue" : "lightgray"
    radius: 10
    property int widthForChild: width -10

    ColumnLayout {
        anchors.fill: parent
        spacing: 3

        Rectangle {
            id: topSpacer
            opacity: 0
            Layout.preferredHeight: 5
        }

        DEHeader {
            id: header
            Layout.preferredWidth: root.widthForChild
        }

        Rectangle {
            id: appsInStack
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: root.widthForChild
            Layout.preferredHeight: root.height - 50
            radius: 10
            StackLayout {
                id: stack_layout
                anchors.fill: parent
                Tumbler_example {
                    id: tumbler_example
                } //0: tumbler_example
//                SmoFolder {
//                    id: fold_example
//                    folderModel: smo.parent.folderModel
//                }
                DBexample {
                    id: db_example
                } //1: DBexample
                SCircularGauge {
                    id: circular_example
                } //2:
            } //StackLayout
        } //Rectangle
    } //ColumnLayout

    Connections {
        id: connNext
        target: header
        onNextClicked: {
            var nexti = stack_layout.currentIndex + 1;
            if (nexti >= stack_layout.count) nexti = 0;
            stack_layout.currentIndex = nexti;
            header.appName = nexti;
        }
    }
} //root

//    ZCircularGauge {
//    }

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
