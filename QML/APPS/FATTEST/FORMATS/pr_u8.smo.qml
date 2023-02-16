import QtQuick 2.0
//import "../../../LIB"
//import SMO 1.0

Item {
    id: rt
//    width: 100; height: 100
    property int data: 333
    property string str: "str"
    Component.onCompleted: {
        console.log("component created!!!");
    }
}
