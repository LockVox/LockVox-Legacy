import QtQuick 2.10
import QtQuick.Templates 2.1 as T
import QtQuick.Layouts 1.0

T.Button {
    id: control

    //implicitWidth: Math.max(
    //buttonBackground ? buttonBackground.implicitWidth : 0,
    //textItem.implicitWidth + leftPadding + rightPadding)
    height: 45 + text1.height
    width: listMessages.width

    leftPadding: 4
    rightPadding: 4

    text: "My Button"

    background: background
    Rectangle {
        id: background
        color: "#00646464"
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        radius: 2
        anchors.fill: parent
    }

    Text {
        id: usernamemsg
        x: 44
        y: 2
        width: 31
        height: 11
        color: "#1f8cad"
        text: from
        font.pixelSize: 11
        font.bold: true
        Layout.preferredHeight: 14
        Layout.preferredWidth: 106
    }

    Text {
        id: timeMsg
        x: 591
        y: 0
        text: time
        anchors.right: parent.right
        font.pixelSize: 9
        font.italic: true
        anchors.rightMargin: 8
        Layout.preferredHeight: 14
        Layout.preferredWidth: 105
        color: "#6b7376"
    }

    Text {
        id: text1
        x: 8
        y: 20

        width: parent.width - 100

        color: "#b7cad0"
        text: content
        wrapMode: Text.Wrap
        anchors.rightMargin: -44
        anchors.leftMargin: 44
        font.pixelSize: 12
        verticalAlignment: Text.AlignVCenter
        lineHeightMode: Text.ProportionalHeight
        Layout.preferredHeight: 114
        Layout.preferredWidth: 234
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Image {
        id: rectangle
        x: 13
        y: 5
        width: 25
        height: 24
        //color: "#676d76"
        //radius: 7
        //border.color: "#313539"
        //border.width: 1
        source: "image://ImageProvider/" + usernamemsg
    }

    Text {
        id: dateMsg
        x: 528
        y: 0
        color: "#6b7376"
        text: date
        anchors.right: parent.right
        font.italic: true
        font.pointSize: 8
        minimumPixelSize: 9
        anchors.rightMargin: 71
        Layout.preferredWidth: 105
        Layout.preferredHeight: 14
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2}
}
##^##*/

