import QtQuick 2.0

Item {
    width: 400
    height: 275

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 400
        height: 275
        color: "#1b1f21"
        radius: 4

        BorderImage {
            id: account_pic
            x: 8
            y: 31
            width: 64
            height: 69
            source: "qrc:/qtquickplugin/images/template_image.png"
        }

        Text {
            id: page_title
            x: 108
            y: 8
            width: 185
            height: 14
            color: "#566369"
            text: qsTr("ACCOUNT INFORMATIONS")
            font.pixelSize: 12
            font.bold: true
        }

        Text {
            id: page_title1
            x: 14
            y: 114
            width: 185
            height: 14
            color: "#566369"
            text: qsTr("USERNAME")
            font.pixelSize: 10
            font.bold: true
        }

        Text {
            id: page_title2
            x: 14
            y: 218
            width: 185
            height: 14
            color: "#566369"
            text: qsTr("PASSWORD")
            font.pixelSize: 10
            font.bold: true
        }

        CustomQpushButton {
            id: customQpushButton
            x: 258
            y: 123
            width: 73
            height: 30
            text: "Modify"
        }

        CustomQpushButton {
            id: customQpushButton1
            x: 258
            y: 230
            width: 73
            height: 30
            text: "Modify"
        }

        Text {
            id: text1
            x: 89
            y: 55
            width: 258
            height: 21
            color: "#ffffff"
            text: qsTr("Username")
            font.pixelSize: 15
            font.bold: true
        }

        Text {
            id: text2
            x: 14
            y: 238
            width: 185
            height: 14
            color: "#ffffff"
            text: qsTr("password")
            font.pixelSize: 12
        }

        Text {
            id: text3
            x: 14
            y: 131
            width: 185
            height: 14
            color: "#ffffff"
            text: qsTr("username")
            font.pixelSize: 12
        }

        CustomQpushButton {
            id: customQpushButton2
            x: 258
            y: 176
            width: 73
            height: 30
            text: "Modify"
        }

        Text {
            id: page_title3
            x: 14
            y: 164
            width: 185
            height: 14
            color: "#566369"
            text: qsTr("EMAIL")
            font.pixelSize: 10
            font.bold: true
        }

        Text {
            id: text4
            x: 14
            y: 184
            width: 185
            height: 14
            color: "#ffffff"
            text: qsTr("email@email.com")
            font.pixelSize: 12
        }
    }

}
