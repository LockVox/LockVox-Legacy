import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Item {

    id: channel
    x: 0
    y: 0
    width: 640
    height: 50

    clip:true
    MouseArea
    {

        //hoverEnabled: true
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

        onClicked: {
            listClient.currentIndexChanged(index)
            listClient.currentIndex = index
        }
    }

    Text {
        objectName: "username"
        id: username
        x: 56
        y: 18
        width: 59
        height: 15
        color: "#000000"
        text: pseudo
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
    }

    StatusIndicator {
        id: statusIndicator
        x: 114
        y: 12
        color: "#1e7e1b"
        active: isOnline
    }

    Rectangle {
        id: rectangle
        x: 8
        y: 12
        width: 33
        height: 30
        color: "#ffffff"
        radius: 7
    }


    Image{
        source:imgPath
    }



}

/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
