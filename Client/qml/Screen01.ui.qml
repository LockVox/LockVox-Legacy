import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick 2.7

import Client 1.0
import Channel 1.0
import Message 1.0

Rectangle {

    objectName: "window"
    id: window
    x: 0
    y: 0
    visible: true
    anchors.fill: parent
    color: "#1b1f21"

    //Main Obj
    property alias login: login
    property alias listClient: listClient
    property alias register: register
    property alias userInfo: userInfo
    property alias connectServer: connectServer

    //Compenent
    property alias loginButton: loginButton
    property alias registerButton: registerButton
    property alias listChannel: listChannel

    //Layout
    //property alias regLog_layout: regLog_layout
    UserInfo {
        objectName: "userInfo"
        id: userInfo
        x: 1
        y: 0
        width: 166
        height: 90
        visible: false
    }

    Rectangle {
        objectName: "rect_login"
        id: rect_log_reg
        color: "#00ffffff"
        border.color: "#1f3d5c"
        border.width: 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 276
        anchors.leftMargin: 20
        anchors.bottomMargin: 14
        anchors.topMargin: 85

        RowLayout {
            id: rowLayout
            height: 53
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 6
            anchors.leftMargin: 8
            anchors.topMargin: 0

            CustomQpushButton {
                id: registerButton
                width: 111
                text: "Create account"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Connections {
                    target: registerButton
                    onClicked: window.state = "registerState"
                }
            }

            CustomQpushButton {
                id: loginButton
                text: "Log in"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true

                Connections {
                    target: loginButton
                    onClicked: window.state = ""
                }
            }
        }

        Register {
            objectName: "register"
            id: register
            x: 73
            y: 66
            visible: false
        }

        Login {
            objectName: "login"
            id: login
            x: 73
            y: 88
            width: 200
            height: 257
            visible: true
        }
    }

    Rectangle {

        id: rect_con_server
        x: 380
        y: 85
        width: 235
        height: 278
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 85
        anchors.rightMargin: 25

        color: "#00ffffff"
        border.color: "#1f3d5c"
        border.width: 2

        ConnectServer {
            objectName: "connect_server"
            id: connectServer
            x: 50
            y: 85
            width: 137
            height: 157
        }

        StateServer {
            objectName: "stateServer"
            id: stateServer
            x: 18
            y: 8
            width: 200
            height: 71
        }
    }

    Image {
        id: lock_vox_logo1
        x: parent.width / 2 - lock_vox_logo1.width / 2
        y: 0
        source: "lock_vox_logo1.png"
        width: 271
        height: 71
    }

    ListView {
        id: listClient
        x: 940
        y: 70
        width: 154
        height: 472
        visible: false
        clip: true
        model: ClientModel {
            m_clientsList: clientsList
        }

        delegate: Delegate_Client {}
    }

    ListView {
        id: listMessage
        x: 380
        y: 267
        width: 235
        height: 199
        visible: false
        clip: true
        model: MessageModel {
            m_messagesList: messagesList
        }

        verticalLayoutDirection: ListView.BottomToTop
        delegate: RowLayout {
            Text {
                text: from
            }
            Text {
                text: content
            }

            //Delegate_Message {}
        }
    }

    //property alias channel:channel
    ListView {

        signal currentIndexChanged(int index)

        objectName: "listChannels"
        id: listChannel

        x: 940
        y: 70
        width: 154
        height: 472
        visible: false
        clip: true
        model: ChannelModel {
            m_channelsList: channelsList
        }

        delegate: Delegate_Channel {}
    }

    MessageWindow {
        id: messageWindow
        x: 196
        y: 25
        visible: false
    }

    AudioWindow {
        id: audioWindow
        x: 150
        y: 260
        visible: false
    }

    Rectangle {
        id: parameters_list
        x: 0
        y: 0
        width: 200
        height: 200
        visible: false
        color: "#ffffff"
    }

    CustomQpushButton {
        id: returnHomeButton
        x: 515
        y: 8
        visible: false
    }

    ParametersWidget {
        id: parametersWidget
        x: 209
        y: 0
        visible: false
    }

    CustomQpushButton2 {
        id: customQpushButton2
        x: 592
        y: 5
    }

    Channels {
        objectName: "channels"
        x: 6
        y: 85
        width: 188
        height: 251
    }

    states: [
        State {
            name: "registerState"

            PropertyChanges {
                target: register
                visible: true
            }

            PropertyChanges {
                target: login
                visible: false
            }

            PropertyChanges {
                target: messageWindow
                visible: false
            }

            PropertyChanges {
                target: parameters_list
                visible: false
            }

            PropertyChanges {
                target: window
                visible: true
            }

            PropertyChanges {
                target: parametersWidget
                visible: false
            }
        },
        State {
            name: "Home"

            PropertyChanges {
                target: columnLayout
                visible: false
            }

            PropertyChanges {
                target: connectServer
                visible: false
            }

            PropertyChanges {
                target: userInfo
                x: 0
                y: 0
                width: 179
                height: 73
                visible: true
                color: "#1b1f21"
            }

            PropertyChanges {
                target: listClient
                x: 495
                y: 70
                width: 145
                height: 410
                visible: true
            }

            PropertyChanges {
                target: rect_con_server
                visible: false
            }

            PropertyChanges {
                target: rect_log_reg
                visible: false
                anchors.rightMargin: 319
            }

            PropertyChanges {
                target: lock_vox_logo1
                visible: false
            }

            PropertyChanges {
                target: messageWindow
                visible: true
            }

            PropertyChanges {
                target: window
                color: "#2e3538"
            }

            PropertyChanges {
                target: audioWindow
                x: 0
                y: 353
                width: 179
                height: 127
                visible: true
            }

            PropertyChanges {
                target: parameters_list
                visible: false
            }

            PropertyChanges {
                target: returnHomeButton
                visible: false
            }

            PropertyChanges {
                target: listChannel
                x: 0
                y: 79
                width: 179
                height: 268
                visible: true
            }

            PropertyChanges {
                target: listMessage
                x: 185
                y: 79
                width: 304
                height: 349
                visible: true
            }
        },
        State {
            name: "parameterState"
            PropertyChanges {
                target: columnLayout
                visible: false
            }

            PropertyChanges {
                target: connectServer
                visible: false
            }

            PropertyChanges {
                target: userInfo
                x: 0
                y: 0
                width: 179
                height: 73
                visible: false
                color: "#1b1f21"
            }

            PropertyChanges {
                target: listClient
                x: 478
                y: 70
                width: 162
                height: 410
                visible: false
            }

            PropertyChanges {
                target: rect_con_server
                visible: false
            }

            PropertyChanges {
                target: rect_log_reg
                visible: false
                anchors.rightMargin: 319
            }

            PropertyChanges {
                target: lock_vox_logo1
                visible: false
            }

            PropertyChanges {
                target: messageWindow
                visible: false
            }

            PropertyChanges {
                target: window
                color: "#2e3538"
            }

            PropertyChanges {
                target: audioWindow
                x: 0
                y: 356
                visible: false
            }

            PropertyChanges {
                target: parameters_list
                x: 0
                y: 0
                width: 160
                height: 480
                visible: true
                color: "#1b1f21"
            }

            PropertyChanges {
                target: returnHomeButton
                x: 532
                y: 8
                text: "return"
            }

            PropertyChanges {
                target: parametersWidget
                x: 196
                y: 85
                visible: true
            }

            PropertyChanges {
                target: customQpushButton2
                x: 592
                y: 5
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.1;height:480;width:640}
}
##^##*/

