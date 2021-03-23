import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Item {
    height: 50

    Text {
        objectName: "username"
        id: username
        x: 8
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
        x: 66
        y: 12
        color: "#1e7e1b"
        active: isOnline
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:6}
}
##^##*/
