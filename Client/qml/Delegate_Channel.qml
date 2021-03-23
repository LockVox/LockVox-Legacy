import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Item {
    height: 50
    Rectangle
        {
            id: rec
            x: 0
            y: 0
            width: 640
            height: 50
            color: index = view.currentIndex ? "blue": "red"

            Text {
                objectName: "title"
                id: title
                x: 8
                y: 18
                width: 59
                height: 15
                color: "#000000"
                text: name
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea
            {
                anchors.fill: parent
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0

                onClicked: view.currentIndex = index

            }




        }


}

