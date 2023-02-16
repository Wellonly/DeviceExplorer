import QtQuick 2.7
import Qt.labs.platform 1.0
import QtQuick.Layouts 1.3
import QtQuick.LocalStorage 2.0
import "local_db"
import "local_db/db_uictrl.js" as JS

Rectangle {
    id: db_example
    color: "#e8e28b"
    anchors.fill: parent

    property bool creatingNewEntry: false
    property bool editingEntry: false

    ColumnLayout {
        anchors.fill: parent

//        Header {
//            id: input
//            Layout.fillWidth: true
//        }
        RowLayout {
            MyButton {
                text: "New"
                onClicked: {
                    input.initrec_new()
                    creatingNewEntry = true
                    listView.model.setProperty(listView.currentIndex, "id", 0)
                }
            }
            MyButton {
                id: saveButton
                enabled: (creatingNewEntry || editingEntry) && listView.currentIndex != -1
                text: "Save"
                onClicked: {
                    var insertedRow = false;
                    if (listView.model.get(listView.currentIndex).id < 1) {
                        //insert mode
                        if (input.insertrec()) {
                            // Successfully inserted a row.
                            input.setlistview()
                            insertedRow = true
                        } else {
                            // Failed to insert a row; display an error message.
                            statustext.text = "Failed to insert row"
                        }
                    } else {
                        // edit mode
                        input.setlistview()
                        JS.dbUpdate(listView.model.get(listView.currentIndex).date,
                                    listView.model.get(listView.currentIndex).trip_desc,
                                    listView.model.get(listView.currentIndex).distance,
                                    listView.model.get(listView.currentIndex).id)
                    }

                    if (insertedRow) {
                        input.initrec()
                        creatingNewEntry = false
                        editingEntry = false
                        listView.forceLayout()
                    }
                }
            }
            MyButton {
                id: editButton
                text: "Edit"
                enabled: !creatingNewEntry && !editingEntry && listView.currentIndex != -1
                onClicked: {
                    input.editrec(listView.model.get(listView.currentIndex).date,
                                  listView.model.get(listView.currentIndex).trip_desc,
                                  listView.model.get(listView.currentIndex).distance,
                                  listView.model.get(listView.currentIndex).id)

                    editingEntry = true
                }
            }
            MyButton {
                id: deleteButton
                text: "Delete"
                enabled: !creatingNewEntry && listView.currentIndex != -1
                onClicked: {
                    JS.dbDeleteRow(listView.model.get(listView.currentIndex).id)
                    listView.model.remove(listView.currentIndex, 1)
                    if (listView.count == 0) {
                        // ListView doesn't automatically set its currentIndex to -1
                        // when the count becomes 0.
                        listView.currentIndex = -1
                    }
                }
            }
            MyButton {
                id: cancelButton
                text: "Cancel"
                enabled: (creatingNewEntry || editingEntry) && listView.currentIndex != -1
                onClicked: {
                    if (listView.model.get(listView.currentIndex).id === 0) {
                        // This entry had an id of 0, which means it was being created and hadn't
                        // been saved to the database yet, so we can safely remove it from the model.
                        listView.model.remove(listView.currentIndex, 1)
                    }
                    listView.forceLayout()
                    creatingNewEntry = false
                    editingEntry = false
                    input.initrec()
                }
            }
            MyButton {
                text: "Exit"
                onClicked: Qt.quit()
            }
        }
        Component {
            id: highlightBar
            Rectangle {
                width: listView.currentItem.width
                height: listView.currentItem.height
                color: "lightgreen"
            }
        }
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: MyModel {}
            delegate: MyDelegate {}
            // Don't allow changing the currentIndex while the user is creating/editing values.
            enabled: !creatingNewEntry && !editingEntry

            highlight: highlightBar
            highlightFollowsCurrentItem: true
            focus: true

            header: Component {
                Text {
                    text: "Saved activities"
                }
            }
        }
        Text {
            id: statustext
            color: "red"
            Layout.fillWidth: true
            font.bold: true
            font.pointSize: 20

        }
    }


    Component.onCompleted: {
        JS.dbInit()
    }

//    Text {
//        text: smo.objectName + " value: " + smo.data
//        font.underline: true
//        font.pointSize: 20
//    }
}
