import QtQuick 2.7
import Qt.labs.platform 1.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import "_tumbler_fold"
import "../../../../../../LIB"


Rectangle {
    id: tumbler_example
    anchors.fill: parent
    border.color: "#1bc418"
    border.width: 2
    color: "#4674b4"
    radius: 10
    
    TumblerListModel {
        id: listModel
    }
    
    Row {
        anchors.fill: parent
        Tumbler {
            id:tumbler
            height: parent.height
            
            TumblerColumn {
                id: tcolumn0
                model: listModel
                role: "value"
                visible: true
                width: 99
            }
            TumblerColumn {
                id: tcolumn1
                model: listModel
                role: "descr"
                visible: true
                width: 100
            }
            Component.onCompleted: {
                tumbler.setCurrentIndexAt(0, smo.data + 1)
                console.log("component completed: tumbler!")
            }
        }
        
        Text {
            id: tvalue
            anchors.verticalCenter: parent.verticalCenter
            text: smo.objectName + qsTr(":") + listModel.get(tcolumn0.currentIndex).descr + "; index:" + tcolumn0.currentIndex
            font.pointSize: 15
            Component.onCompleted: console.log("component completed: text tvalue!")
        }
    }
    
    //Connections...
    Connections {
        id: connFromSmo
        target: smo
        onDataChanged: {
            connToSmo.enabled = false;
            var newIndex = smo.data +1;
            tumbler.setCurrentIndexAt(0, newIndex);
            tumbler.setCurrentIndexAt(1, newIndex);
            connToSmo.enabled = true;
            console.log("connFromSmo new index:", newIndex, smo.flagsToString());
        }
        Component.onCompleted: {
            console.log("connFromSmo: Completed!");
        }
    }
    Connections {
        id: connToSmo
        target: tcolumn0
        onCurrentIndexChanged: {
            tumbler.setCurrentIndexAt(1, tcolumn0.currentIndex);
            var newvalue = tcolumn0.currentIndex -1;
            smo.data = newvalue;
            console.log("connToSmo new value:", newvalue, smo.flagsToString());
        }
        Component.onCompleted: {
            console.log("connToSmo: Completed!");
        }
    }
    
    Component.onCompleted: {
        console.log("Tumbler example:","smo.data:", smo.data, "; smo.flags:", smo.flags);
    }
}
