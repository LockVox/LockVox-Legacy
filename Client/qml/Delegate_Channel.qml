import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Item {

            id: channel
            x: 0
            y: 0
            width: 640
            height: 50

            Text {
                objectName: "title"
                id: title
                x: 8
                y: 10
                width: 188
                height: 32
                color: "#000000"
                text: model.name
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
            }

            MouseArea
            {
                id: mouse
                anchors.fill: parent
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0

                onClicked: listChannel.currentIndexChanged(index)

            }

        }


