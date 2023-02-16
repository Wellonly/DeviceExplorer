/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:FDL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Free Documentation License Usage
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file. Please review the following information to ensure
** the GNU Free Documentation License version 1.3 requirements
** will be met: http://www.gnu.org/copyleft/fdl.html.
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Layouts 1.1
//import QtQuick.LocalStorage 2.0
//import "local_db.js" as JS

Item {
    id: delegate
    width: parent.width
    height: rDate.implicitHeight
    property int pixSize: 22

    Rectangle {
        id: baseRec
        anchors.fill: parent
        opacity: 0.8
        color: index % 2 ? "lightgrey" : "grey"

        MouseArea {
            anchors.fill: parent
            onClicked: listView.currentIndex = index
        }
        GridLayout {
            anchors.fill:parent
            columns: 3

            Text {
                id: rDate
                text: event_date
                font.pixelSize: delegate.pixSize
                color: "green"
            }
            Text {
                id: rDesc
                text: event_time
                Layout.alignment: Qt.AlignLeft
                font.pixelSize: delegate.pixSize
                color: "steelblue"
            }
            Text {
                id: rDistance
                text: event_value
                font.pixelSize: delegate.pixSize
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                color: "black"
            }
        }
    }
}
