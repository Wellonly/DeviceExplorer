import QtQuick 2.7
import Qt.labs.platform 1.0
import QtQuick.Layouts 1.3
import QtQuick.LocalStorage 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "../../../../../../LIB"
import "_local_db"
import "_local_db/local_db.js" as JS

Rectangle {
    id: db_example
    color: "#e8e28b"
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent

        SmoFolder {
            folderModel: smo.parent.folderModel
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        RowLayout {
            MyButton {
                id: saveButton
                text: "Save current state"
                onClicked: listView.save_data()
            }
            MyButton {
                id: deleteButton
                text: "Delete all"
                onClicked: {
                    JS.dbDeleteAll() // JS.dbDeleteRow(listView.model.get(listView.currentIndex).id)
                    listView.model.clear()
                    listView.currentIndex = -1 // ListView doesn't automatically set its currentIndex to -1 when the count becomes 0.
                }
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
        ScrollView {
            id: scroll
            Layout.fillWidth: true
            style: ScrollViewStyle {}
            ListView {
                id: listView
                anchors.fill: parent
                model: MyModel {}
                delegate: MyDelegate {}
                // enabled: !creatingNewEntry && !editingEntry // Don't allow changing the currentIndex while the user is creating/editing values.

                highlight: highlightBar
                highlightFollowsCurrentItem: true
                focus: true

                header: Component {
                    Text {
                        text: "Database: saved activities:"
                    }
                }

                function append_rec() {
                    var ptime = smo.parent.folderItems[0].data
                    var pvalue = smo.parent.folderItems[1].data
                    var pdate = Date().toLocaleString() /*smo.parent.folderItems[2].data*/
                    var rowid = parseInt(JS.dbInsert(pdate, ptime, pvalue), 10)
                    if (rowid) {
                        listView.model.append({
                                                  id: rowid,
                                                  checked: " ",
                                                  event_date: pdate,
                                                  event_time: ptime,
                                                  event_value: pvalue
                                              })
                        listView.forceLayout()
                    }
                    return rowid;
                }

                function save_data() {
                    if (listView.append_rec())
                    { // Successfully inserted a row...
                        listView.forceLayout()
                        statustext.text = ""
                    } else {
                        // Failed to insert a row; display an error message.
                        statustext.text = "Failed to insert row"
                    }
                }

            } //ListView
        } //ScrollView
        Text {
            id: statustext
            color: "red"
            Layout.fillWidth: true
            font.bold: true
            font.pointSize: 20
            text: "statustext"
        }
    }

    Component.onCompleted: {
        JS.dbInit()
    }

    Connections {
        target: smo
        onDataChanged: {
            console.log("db_example smo changed event:", smo);
            listView.save_data();
        }
    }
}
