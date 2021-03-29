import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    objectName: "register"
    id: register

    signal register_request(string emailField,string usernameField, string passwordField, string verifyPasswordField)
    width: 200
    height: 300

    Column {
        id: column
        x: 0
        y: -2
        spacing: 8

        anchors.left: parent.left
        anchors.right:parent.right
        anchors.verticalCenter:parent.verticalCenter
        anchors.verticalCenterOffset:-50




        TextField {
            id: emailField
            height: 40
            placeholderText: qsTr("Email")
            anchors.left:parent.left
            anchors.right:parent.right
            color:"white"
            background: Rectangle {
                color:"#313639"
                radius:0
                border.width: 0
            }
        }

        TextField {
            id: usernameField
            height: 40
            placeholderText: qsTr("Username")
            anchors.left:parent.left
            anchors.right:parent.right
            color:"white"
            background: Rectangle {
                color:"#313639"
                radius:0
                border.width: 0
            }
        }

        TextField {
            id: passwordField
            height: 40
            placeholderText: qsTr("Password")
            echoMode:TextInput.Password
            anchors.left:parent.left
            anchors.right:parent.right
            color:"white"
            background: Rectangle {
                color:"#313639"
                radius:0
                border.width: 0
            }
        }

        TextField {
            id: verifyPasswordField
            height: 40
            placeholderText: qsTr("Verify password")
            echoMode:TextInput.Password
            anchors.left:parent.left
            anchors.right:parent.right
            color:"white"
            background: Rectangle {
                color:"#313639"
                radius:0
                border.width: 0
            }
        }

    }

    CustomQpushButton {
        id: customQpushButton
        x: 50
        y: 219
        text: qsTr("Register")
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20


        Connections {
            target: customQpushButton
            onClicked: register.register_request(emailField.text,usernameField.text,passwordField.text,verifyPasswordField.text)
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.3300000429153442}
}
##^##*/
