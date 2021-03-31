import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Item {

    id: client
    x: 0
    y: 0
    width: parent.width-5
    height: 50
    focus: false



    clip:true
    MouseArea
    {

        hoverEnabled: true
        //containsMouse: true
        id: mouse
        width: rectangle.width
        height: rectangle.height
        x:rectangle.x
        y:rectangle.y

        visible: true
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        onEntered: username.color="#f46f68"

        onExited: username.color="#33a5e5"

        onClicked: {
            listClient.currentIndexChanged(index)
            listClient.currentIndex = index
        }
    }

    Text {
        objectName: "username"
        id: username
        x: 55
        y: 18
        width: 123
        height: 15
        color: "#33a5e5"
        text: pseudo
        font.pixelSize: 13
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        lineHeightMode: Text.ProportionalHeight
        wrapMode: Text.Wrap
        font.bold: true
    }

    Image{
        objectName: "img"
        id: img
        x: 8
        y: 8
        fillMode: Image.PreserveAspectFit
        width: 35
        height: 35
        source:"image://ImageProvider/"+pseudo
    }

    StatusIndicator {
        id: statusIndicator
        x: 34
        y: 34
        width: 9
        height: 9
        color: "#52f34c"
        active: isOnline
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
