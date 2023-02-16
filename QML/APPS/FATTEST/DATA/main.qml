import QtQuick 2.0
import "../../../LIB"
import SMO 1.0

Rectangle {
    id: _root
    color: Qt.application.active ? "lightblue" : "lightgray"
    border.color: "#0c0d18"

    Text {
        id: tname
        text: "date:" + Date(Date.now())
        font.pointSize: 20
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: /*tname.text = smo.objectName + "clicked!!!"*/
                {
                    console.log("smo.data:", smo.data, SMO)
                    console.log("smo inviter:", smo.inviteString())
                    console.log("Status Ok:", SMO.FlagIOk | SMO.FlagIntegrityError)
                    console.log("Status is Ok:",
                                !!(smo.flags & (SMO.FlagIOk | SMO.FlagIntegrityError)))
                    console.log("smo.flags:", smo.flags, smo.flagsToString())
                    console.log("smo.parent:", smo.parent.objectName)
                    console.log("smo.child:", smo.folderItems)
                    console.log("smo.child.at(0):", smo.folderItems[0].objectName)
                    console.log("smo.child cou:", smo.folderItems.length)
                    console.log("smo.device:", smo.device.objectName)
                    console.log("smo.device.deviceItem:", smo.device.deviceItem.objectName)
                    console.log("dataType:", smo.dataType,
                                smo.dataType === SMO.DtUndefined, SMO.DtRGB)
                    console.log("smoType:", smo.smoType,
                                smo.smoType === SMO.TypeFolder, SMO.TypeFolder)
                    console.log("folderType:", smo.folderType,
                                smo.folderType === SMO.FolderGeneric, SMO.FolderNet)
                }
    }
    //    ZGauge {
    //    }
}
