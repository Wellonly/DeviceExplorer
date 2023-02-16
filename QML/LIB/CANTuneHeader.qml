import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
//import SMO 1.0

ColumnLayout {
    spacing: 2
    property var folderItem: QtObject
    RowLayout {
        id: headerRow
        spacing: 2
        property int fontSize: 22
        property alias rowHeight: textDevName.height

        Image {
            id: iconFolder
            source: "../../Icons.png/folder.png"
            Layout.preferredHeight: headerRow.rowHeight
        }
        Text { color: "#a09a49"; text: "Проверка-калибровка платы  "; font.pointSize: parent.fontSize}

        Image {
            id: iconDevice
            source: "../../Icons.png/device.png"
            Layout.preferredHeight: headerRow.rowHeight
        }
        Text { id: textDevName; color: "#5b91a0"; text: "УЗА" + "  "; font.pointSize: headerRow.fontSize}

        ZButton {
            text: "Обновить данные"
            Layout.preferredHeight: headerRow.rowHeight
            onClicked: {
                console.time("ZButton_cliked");
                updatePage(folderItem);
                console.timeEnd("ZButton_cliked");
            }
        }
    } //RowLayout

    Text { color: "#a09a49"; text: " Параметры изделия:"; font.pointSize: headerRow.fontSize}


    Rectangle { //header/data separate line!!!
        border.width: 1
        border.color: "black"
        height: 2
        width: headerRow.width
    }
    Rectangle { //bottomMargin
        height: 2
        opacity: 0
    }

    function updatePage(s) {
        console.log("update itemsCount:", 11);
//        var cou = s.folderItemsCount();
//        console.log(s.objectName, "update folderItemsCount:", cou);
//        for (var i = 0; i < cou; i++) {
//            var smo = s.folderItems[i];
//            console.log("updatePage:", i, smo.objectName, smo.readData(), smo.data);
//        }
    }
} //ColumnLayout
