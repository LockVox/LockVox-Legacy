import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import "Client/qml"
import Client 1.0
import Channel 1.0

ApplicationWindow {

    width: 640
    height: 480
    visible: true
    title: "Lock-Vox"
/*
    ListView {
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

        delegate: RowLayout {
            Text {
                text :"test"
            }

            Text {
                text: model.name
            }

            Text {
                text: model.maxUsers
            }
        }
    }
  */
    Screen01{


    }

}


