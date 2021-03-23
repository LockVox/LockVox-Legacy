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



        TextField {
            id: emailField
            placeholderText: qsTr("Email")
        }

        TextField {
            id: usernameField
            placeholderText: qsTr("Username")
        }

        TextField {
            id: passwordField
            placeholderText: qsTr("Password")
        }

        TextField {
            id: verifyPasswordField
            placeholderText: qsTr("Verify password")
        }

    }

    CustomQpushButton {
        id: customQpushButton
        x: 50
        y: 220
        text: qsTr("Register")

        Connections {
            target: customQpushButton
            onClicked: register.register_request(emailField.text,usernameField.text,passwordField.text,verifyPasswordField.text)
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}
}
##^##*/
