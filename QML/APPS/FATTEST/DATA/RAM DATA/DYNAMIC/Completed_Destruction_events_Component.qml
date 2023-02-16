import QtQuick 2.0

Rectangle {
  Component.onCompleted: console.log("Completed Running!")
  Component.onDestruction: console.log("Destruction Beginning!")
  Rectangle {
    Component.onCompleted: console.log("Nested Completed Running!")
    Component.onDestruction: console.log("Nested Destruction Beginning!")
  }
}
