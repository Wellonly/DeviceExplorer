import QtQuick 2.7
import Qt.labs.platform 1.0
import QtQuick.Extras 1.4
import "../../../../../../../LIB"


ListModel {
    id: listModel
    
    ListElement {
        value: -1
        strvalue: "minus1"
        descr: "мигает"
    }
    ListElement {
        value: 0
        strvalue: "zero"
        descr: "выключено"
    }
    ListElement {
        value: 1
        strvalue: "one"
        descr: "включено"
    }
}
