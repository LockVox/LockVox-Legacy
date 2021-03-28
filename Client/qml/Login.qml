import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0



Item {
    objectName: "login"
    id: login

    signal login_request(string email, string password)
    width: 200
    height: 300


    Column {
        id: column
        x: 0
        y: 0
        spacing: 8

        TextField {
            id: emailField
            placeholderText: qsTr("Email")
        }

        TextField {
            id: passwordField
            placeholderText: qsTr("Password")
            echoMode:TextInput.Password

            //TextField.passwordCharacter: true


        }

    }

    CustomQpushButton {
        id: customQpushButton
        x: 50
        y: 184
        text: qsTr("Sign In")

        Connections {
            target: customQpushButton
            onClicked: login.login_request(emailField.text,passwordField.text)
        }
    }
}




/*##^##
Designer {
    D{i:0;formeditorZoom:0.8999999761581421}
}
##^##*/
