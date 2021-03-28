import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import "Client/qml"

import Client 1.0
import Channel 1.0
import Message 1.0

ApplicationWindow {

    id:mainWindow
    width: 640
    height: 480
    visible: true
    title: "Lock-Vox"
    //:"Client/qml/lock_vox_logo1.png"
    minimumHeight : 100
    minimumWidth :200

    property int previousX
    property int previousY

    //maximumHeight: 1080
    //maximumWidth: 1920




    Screen01{
        anchors.fill: parent
        //listChannel.onCurrentIndexChanged:  console.log(listChannel.currentIndex)

        disconnect.onTriggered:menuBar.disconnect()
        change_server.onTriggered: menuBar.change_server()

        quit_confirm.onClicked: quit_popup.confirmQuit(1)
        quit_back.onClicked: quit_popup.confirmQuit(0)

        //reduce_window.onTriggered: mainWindow.showMinimized()
        //maximize_window.onTriggered: mainWindow.showMaximized()
        //normal_window.onTriggered: mainWindow.showNormal()
        listMessage.onCountChanged: listMessage.positionViewAtEnd()


        //Move Window
        moveWindowArea.onPressed: {
            previousX = mouse.x
            previousY = mouse.y
        }

        moveWindowArea.onMouseXChanged: {
            var dx = mouse.x - previousX
            mainWindow.setX(mainWindow.x + dx)
        }

        moveWindowArea.onMouseYChanged: {
            var dy = mouse.y - previousY
            mainWindow.setY(mainWindow.y + dy)
        }
/*
        //LeftArea resizing
        leftArea.onPressed: {
            previousX = mouse.x
        }

        leftArea.onMouseXChanged: {
            var dx = mouse.x - previousX
            mainWindow.setWidth(mainWindow.width + dx)
        }

        //BottomArea resizing
        bottomArea.onPressed: {
            previousY = mouse.y
        }

        bottomArea.onMouseYChanged: {
            var dy = mouse.y - previousY
            mainWindow.setHeight(mainWindow.height + dy)
        }

        //RightArea resizing
        rightArea.onPressed: {
            previousX = mouse.x
        }

        rightArea.onMouseXChanged: {
            var dx = mouse.x - previousX
            mainWindow.setWidth(mainWindow.width + dx)
        }
*/

    }

}
