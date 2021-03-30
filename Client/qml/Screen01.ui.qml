import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQuick 2.7
import QtQuick.Extras 1.4
import QtQuick.Window 2.2

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
    property alias stateServer: stateServer
    property alias channels: channels
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
    property alias moveWindowArea: moveWindowArea

    //property alias leftArea:leftArea
    //property alias bottomArea:bottomArea
    //property alias rightArea:rightArea
    MouseArea {
        id: moveWindowArea
        height: 30
        width: parent.width
        x: 0
        y: 0

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    //The following part is for resizing the window - it doesn't work correctly for now


    /*
    MouseArea {
        id: bottomArea
        height: 10
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        cursorShape: Qt.SizeVerCursor
    }

    MouseArea {
            id: leftArea
            width: 5
            x:0
            y:0
            height:parent.height

            anchors {

                left: parent.left
            }
            cursorShape: Qt.SizeHorCursor

        }

        MouseArea {
            id: rightArea
            width: 5
            x:parent.width-5
            y:0
            height:parent.height

            anchors {
                right: parent.right
            }
            cursorShape:  Qt.SizeHorCursor
        }
*/
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
        anchors.rightMargin: 272
        anchors.leftMargin: 25
        anchors.bottomMargin: 8
        anchors.topMargin: 91

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

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        Login {
            objectName: "login"
            id: login
            x: 73
            y: 66
            width: 200
            height: 300
            visible: true

            anchors.bottom: parent.bottom
            anchors.topMargin: 59
            anchors.top: parent.top
            anchors.bottomMargin: 0
            anchors.leftMargin: 73
            anchors.rightMargin: 70
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }

    Rectangle {

        id: rect_con_server
        x: 383
        y: 85
        width: 232
        height: 278
        visible: true
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
            visible: true
        }

        StateServer {
            objectName: "stateServer"
            id: stateServer
            x: 18
            y: 8
            width: 200
            height: 71
            visible: true
        }
    }

    ListView {
        objectName: "listClients"
        id: listClient
        x: 940
        y: 70
        width: 154
        height: 472
        visible: false
        clip: true

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        ScrollBar.vertical: ScrollBar {
            id: scrollBarClient
            parent: listClient.parent
            anchors.top: listClient.top
            anchors.left: listClient.right
            anchors.bottom: listClient.bottom
        }

        currentIndex: 1

        //preferredHighlightBegin:
        highlightFollowsCurrentItem: true
        highlight: Rectangle {
            color: "#313539"
            height: parent.height
            width: parent.width
        }

        focus: true

        model: ClientModel {
            m_clientsList: clientsList
        }

        delegate: Delegate_Client {
            objectName: "clients"
        }
    }

    //property alias channel:channel
    ListView {

        signal currentIndexChanged(int index)

        objectName: "listChannels"
        id: listChannel

        anchors.top: parent.top
        anchors.bottom: parent.bottom

        ScrollBar.vertical: ScrollBar {
            id: scrollBarChannel
            parent: listChannel.parent
            anchors.top: listChannel.top
            anchors.left: listChannel.right
            anchors.bottom: listChannel.bottom
        }

        currentIndex: 1

        //preferredHighlightBegin:
        highlightFollowsCurrentItem: true

        highlight: Rectangle {
            color: "#33a5e5"
            height: parent.height
            width: parent.width
        }

        focus: true

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

    //property alias MessageWindow: MessageWindow
    property alias listMessage: listMessage

    MessageWindow {
        id: messageWindow
        x: 196
        y: 25
        visible: false
        focus: true

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        ListView {

            id: listMessage
            x: 7
            y: 79
            width: 235
            height: 199
            visible: false
            clip: true

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: messageWindow.left
            anchors.right: messageWindow.right

            ScrollBar.vertical: ScrollBar {
                id: scrollBarMessage
            }

            model: MessageModel {
                m_messagesList: messagesList
            }

            verticalLayoutDirection: ListView.TopToBottom
            delegate: MessageWidget {
                width: parent.width
            }
        }
    }

    AudioWindow {

        anchors.bottom: parent.bottom
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

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        visible: false
    }

    CustomQpushButton2 {
        id: returnHomeBtn
        x: 592
        y: 5
        visible: false
        anchors.right: parent.right

        Connections {
            target: returnHomeBtn
            onClicked: window.state = "Home"
        }
    }

    UserInfo {
        objectName: "userInfo"
        id: userInfo

        anchors.top: menuBar.bottom

        x: 2
        y: 0
        width: 166
        height: 90
        visible: false

        CustomQpushButton {
            id: parameterButton
            x: 85
            y: 33

            Connections {
                target: parameterButton
                onClicked: window.state = "parameterState"
            }
        }
    }

    Column {
        id: home
        x: 221
        y: 0
        width: 200
        height: 400
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.topMargin: 245

        Row {
            id: leftSideHome
            width: 200
            height: 400
            anchors.top: parent.bottom
            anchors.bottom: parent.bottom
        }
    }

    Channels {
        id: channels
        objectName: "channels"
        x: 6
        y: 85
        width: 188
        height: 251
    }

    property alias menuBar: menuBar
    property alias quit: quit
    property alias disconnect: disconnect

    //property alias reduce_window: reduce_window
    //property alias maximize_window: maximize_window
    //property alias normal_window: normal_window
    MenuBar {
        objectName: "menu_bar"
        id: menuBar

        signal quit
        signal disconnect
        signal ping_server
        signal change_server
        background: Rectangle {
            color: "#1b1f21"
        }

        x: 0
        y: 0
        width: parent.width
        height: 30
        visible: false

        Menu {

            objectName: "logo"
            id: logo
        }

        Menu {

            objectName: "menu_quit_button"
            id: menu_quit_button
            Image {}
            spacing: 5

            Action {
                id: quit
                text: qsTr("&Quit")
                onTriggered: quit_popup.visible = true
            }
            Action {
                text: qsTr("&Disconnect")
                id: disconnect
            }
        }

        MenuSeparator {
            id: menuSeparator
            visible: false
        }

        MenuSeparator {}

        delegate: MenuBarItem {
            id: menuBarItem

            contentItem: Text {
                text: menuBarItem.text
                font: menuBarItem.font
                opacity: enabled ? 1.0 : 0.3
                color: menuBarItem.highlighted ? "red" : "#21be2b"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: parent.width
                implicitHeight: 30
                opacity: enabled ? 1 : 0.3
                color: menuBarItem.highlighted ? "#21be2b" : "transparent"
            }
        }
    }

    property alias quit_popup: quit_popup
    property alias quit_confirm: quit_confirm
    property alias quit_back: quit_back
    Popup {
        objectName: "quit_popup"
        id: quit_popup

        signal confirmQuit(int confirm)

        x: 100
        y: 100
        width: 200
        height: 300
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        ColumnLayout {
            Button {
                id: quit_confirm
                text: qsTr("Quit")
            }

            Button {
                id: quit_back
                text: qsTr("Come back to the application")
            }
        }
    }

    Image {
        id: lock_vox_logo11
        x: 172
        y: 6
        width: 295
        height: 79
        source: "lock_vox_logo1.png"
        anchors.rightMargin: 211
        anchors.leftMargin: 211
        fillMode: Image.PreserveAspectFit

        anchors.left: parent.left
        anchors.right: parent.right
    }

    ProgressBar {
        id: progressBar
        x: 221
        y: 221
        visible: false
        value: 0.5
    }

    Text {
        id: membersText
        x: 508
        y: 48
        visible: false

        text: "MEMBERS"
        anchors.right: parent.right

        font.pixelSize: 12
    }

    AddChannelButton {
        id: add_channel_btn
        x: 0
        y: 99
        visible: false
        anchors.left: parent.left
        anchors.top: parent.top
    }

    states: [
        State {
            name: "registerState"

            PropertyChanges {
                target: register
                visible: true
                anchors.bottomMargin: 0
                anchors.leftMargin: 73
                anchors.topMargin: 59
                anchors.rightMargin: 68
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

            PropertyChanges {
                target: rect_con_server
                visible: false
            }

            PropertyChanges {
                target: rect_log_reg
                anchors.leftMargin: 148
                anchors.bottomMargin: 0
                anchors.rightMargin: 149
                anchors.topMargin: 99
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
                y: 29
                width: 179
                height: 80
                visible: true
                color: "#1b1f21"
                border.width: 0
                anchors.topMargin: -3
            }

            PropertyChanges {
                target: listClient
                x: 487
                y: 70
                width: 145

                height: 410
                visible: true
                anchors.rightMargin: 8
                anchors.bottomMargin: 14
                anchors.topMargin: 71
                anchors.leftMargin: 489
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
                target: messageWindow
                x: 176
                y: 0
                width: 305
                height: 480
                visible: true
                anchors.rightMargin: 177
                anchors.bottomMargin: 0
                anchors.leftMargin: 176
                anchors.topMargin: 0
            }

            PropertyChanges {
                target: window
                height: 503
                color: "#1f2325"
                border.width: 0
            }

            PropertyChanges {
                target: audioWindow
                x: 0
                y: 356
                width: 177
                height: 124
                visible: false
                anchors.rightMargin: 461
                anchors.bottomMargin: 0
                anchors.leftMargin: 1
                anchors.topMargin: 326
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
                y: 71
                width: 179
                height: 287
                visible: true
                anchors.bottomMargin: 8
                anchors.topMargin: 157
            }

            PropertyChanges {
                target: listMessage
                x: 17
                y: 40
                width: 271
                height: 366

                visible: true
                boundsBehavior: Flickable.StopAtBounds
                keyNavigationWraps: true
                snapMode: ListView.NoSnap
                anchors.topMargin: 70
                anchors.bottomMargin: 61
                anchors.rightMargin: 21
                anchors.leftMargin: 14
            }

            PropertyChanges {
                target: leftSideHome
                x: 1
                y: 0
                width: 180
                height: 456
                rightPadding: 2
                leftPadding: 2
                spacing: 0
                topPadding: 0
                layoutDirection: Qt.LeftToRight
            }

            PropertyChanges {
                target: home
                x: 0
                y: 0
                width: 640
                height: 480
            }

            PropertyChanges {
                target: parameterButton
                x: 97
                y: 63
                width: 66
                height: 17
                text: "parameter"
            }

            PropertyChanges {
                target: channels
                x: 0
                y: 71
                width: 179
                height: 287
            }

            PropertyChanges {
                target: menuBar
                visible: true
            }

            PropertyChanges {
                target: lock_vox_logo11
                visible: false
            }

            PropertyChanges {
                target: maximize_window
                x: 0
                y: 0
                width: 103
                height: 30
                text: qsTr("maximize")
            }

            PropertyChanges {
                target: normal_window
                height: 30
            }

            PropertyChanges {
                target: reduce_window
                height: 30
            }

            PropertyChanges {
                target: lock_vox_logo_miniature1
                x: 23
                y: 4
                width: 24
                height: 23
            }

            PropertyChanges {
                target: reduce_window
                x: -103
                y: 0
                width: 103
                height: 36
            }

            PropertyChanges {
                target: maximize_window
                x: -282
                y: 2
                width: 86
                height: 32
                text: qsTr("Maximize Window")
            }

            PropertyChanges {
                target: normal_window
                x: -195
                y: 0
                width: 86
                height: 14
            }

            PropertyChanges {
                target: image
                x: 0
                y: 0
                width: 49
                height: 16
            }

            PropertyChanges {
                target: menuSeparator
                visible: true
            }

            PropertyChanges {
                target: scrollBar
                visible: true
                active: true
            }

            PropertyChanges {
                target: membersText
                x: 469
                y: 43
                width: 65
                height: 14
                visible: true
                color: "#676d76"
                font.pixelSize: 9
                minimumPixelSize: 9
                font.bold: true
                anchors.leftMargin: 487
                anchors.topMargin: 48
                anchors.rightMargin: 106
                anchors.bottomMargin: 443
            }

            PropertyChanges {
                target: add_channel_btn
                x: 0
                y: 99
                width: 179
                height: 50
                visible: true
                text: "+ Add a channel"
                anchors.leftMargin: 0
                anchors.topMargin: 108
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
                x: 209
                y: 85
                width: 399
                height: 278
                visible: true
                anchors.topMargin: 92
                anchors.bottomMargin: 117
                anchors.leftMargin: 185
                anchors.rightMargin: 56
            }

            PropertyChanges {
                target: returnHomeBtn
                x: 612
                y: 8
                visible: true
                anchors.rightMargin: 8
            }

            PropertyChanges {
                target: lock_vox_logo11
                x: 172
                y: 8
                width: 295
                height: 94
                visible: false
            }
        },
        State {
            name: "splashScreen"

            PropertyChanges {
                target: loginButton
                visible: false
            }

            PropertyChanges {
                target: connectServer
                visible: false
            }

            PropertyChanges {
                target: stateServer
                visible: false
            }

            PropertyChanges {
                target: rect_con_server
                visible: false
            }

            PropertyChanges {
                target: rect_log_reg
                visible: false
            }

            PropertyChanges {
                target: progressBar
                x: 125
                y: 281
                width: 391
                height: 42
                visible: true
                font.pointSize: 8
                value: 0
            }

            PropertyChanges {
                target: lock_vox_logo11
                x: 173
                y: 177
            }
        },
        State {
            name: "connectToServerState"
            PropertyChanges {
                target: register
                visible: false
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

            PropertyChanges {
                target: audioWindow
                visible: false
            }

            PropertyChanges {
                target: userInfo
                visible: false
            }

            PropertyChanges {
                target: registerButton
                visible: false
            }

            PropertyChanges {
                target: loginButton
                visible: false
            }

            PropertyChanges {
                target: rect_log_reg
                visible: false
            }

            PropertyChanges {
                target: rect_con_server
                x: 203
                visible: true
                anchors.rightMargin: 203
                anchors.topMargin: 117
            }

            PropertyChanges {
                target: lock_vox_logo11
                x: 172
                y: 8
                width: 295
                height: 94
            }

            PropertyChanges {
                target: stateServer
                x: 18
                y: 0
                visible: false
            }

            PropertyChanges {
                target: progressBar
                visible: false
            }

            PropertyChanges {
                target: connectServer
                visible: true
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/

