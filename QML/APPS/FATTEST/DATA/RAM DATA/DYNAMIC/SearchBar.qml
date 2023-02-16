import QtQuick 2.2
import QtQuick.Controls 1.1 /*as QQControls*/
import QtQuick.Controls.Styles 1.1

//import TimeExample 1.0 // import types from the plugin


Rectangle {
  id: searchBar
  
  width: 930
  height: 30
  color: "gray"
  border.color: "green"
  
  property alias placeholderText: lineEdit.placeholderText
  property alias text: lineEdit.text
  
  TextField {
    id: lineEdit
    anchors.topMargin: 1
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.rightMargin: 12
    anchors.leftMargin: 12
    anchors.fill: parent
    verticalAlignment: Text.AlignVCenter
    font.pixelSize: 14
    placeholderText:  qsTr("Search...")
    style: TextFieldStyle {
      placeholderTextColor: "red"
      textColor: "yellow"
      background: Item {
      }
    }
  }
  Accessible.name: text
  Accessible.description: placeholderText
  Accessible.role: Accessible.EditableText
}
