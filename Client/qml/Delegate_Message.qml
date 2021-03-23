import QtQuick 2.0

Component {
        id: msgDelegate
        Rectangle
            {
                id: rec
                width: 50
                height: 50
                color: index = view.currentIndex ? "blue": "red"

                Text{
                    text: from
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

/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
