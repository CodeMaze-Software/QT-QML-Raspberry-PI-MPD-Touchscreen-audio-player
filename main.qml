import QtQuick.Controls 2.0
import QtQuick 2.1
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 400
    title: qsTr("Hello World")
    id:root

    property variant win;

    Button {
        id: button
        x: 22
        y: 23
        text: qsTr("play")

        onClicked:
        {
            player.play()

            var component = Qt.createComponent("testwindow.qml");
            win = component.createObject(root);
            win.show();
        }
    }

    Button {
        id: button2
        x: 22
        y: 82
        text: qsTr("prev")

        onClicked: player.prev()
    }

    Button {
        id: button1
        x: 22
        y: 146
        text: qsTr("next")

        onClicked: player.next()
    }

    Button {
        id: button3
        x: 22
        y: 211
        text: qsTr("stop")

        onClicked:
        {
            player.stop()
            win.hide()
        }
    }

    Button {
        id: button4
        x: 22
        y: 277
        text: qsTr("pause")

        onClicked: player.stop()
    }

    Tumbler {
        id: volume
        x: 1095
        y: 0
        width: 185
        height: 400
        model:100
        wrap:false
        currentIndex: player.currentVolume

        onCurrentIndexChanged:
        {
            player.volume(volume.currentIndex)
        }
    }

    Button {
        id: button5
        x: 150
        y: 82
        text: qsTr("Seek : 60")

        onClicked: player.seek(60)
    }

    Button {
        id: button6
        x: 426
        y: 23
        text: qsTr("Add playlist1")

        onClicked: {
            player.addPlaylist("PlaylistX")
            player.clearPlaylist("PlaylistX")

        }
    }

    Button {
        id: button7
        x: 285
        y: 82
        text: qsTr("RM playlists")

        onClicked: player.removePlaylist("PlaylistX")
    }

    Button {
        id: button8
        x: 284
        y: 277
        text: qsTr("Show playlists")

        onClicked: player.listPlaylists()
    }

    Button {
        id: button9
        x: 426
        y: 82
        text: qsTr("List playlist1")

        onClicked: player.listPlaylist("PlaylistX")
    }

    Button {
        id: button10
        x: 284
        y: 211
        text: qsTr("List Dir")

        onClicked: player.listDirectory("/")
    }

    Button {
        id: button11
        x: 426
        y: 146
        text: qsTr("Add song to PL1")

        onClicked:
        {
            player.addSongToPlaylist("PlaylistX","Alphaville - Forever Young (HQ Audio).mp3")
            player.addSongToPlaylist("PlaylistX","Showtek feat. Lexi Jean - Music On My Mind HD.mp3")
        }
    }

    Button {
        id: button12
        x: 426
        y: 211
        width: 107
        height: 40
        text: qsTr("Play playlist1")

        onClicked: player.playPlaylist("PlaylistX")
    }

    Button {
        id: button13
        x: 566
        y: 211
        width: 107
        height: 40
        text: qsTr("Play Radio")

        onClicked: player.playPlaylist("PlaylistY")
    }

    Button {
        id: button14
        x: 566
        y: 146
        text: qsTr("Add song to Radio's")

        onClicked:
        {
            player.addSongToPlaylist("PlaylistY","http://www.skyrock.fm/stream.php/tunein16_128mp3.mp3")
            player.addSongToPlaylist("PlaylistY","http://us2.internet-radio.com:8075")
            player.addSongToPlaylist("PlaylistY","http://direct.fipradio.fr/live/fip-midfi.mp3")
            player.addSongToPlaylist("PlaylistY","http://stream.srg-ssr.ch/m/couleur3/aacp_96")
            player.addSongToPlaylist("PlaylistY","http://stream.srg-ssr.ch/m/la-1ere/aacp")

            //player.sendCudtomCmd("playlistadd \"PlaylistY\" \"Dr Alban 'No Coke' (12 'mix).mp3\"") http://uk3.internet-radio.com:8405
        }
    }

    Button {
        id: button15
        x: 570
        y: 82
        text: qsTr("List playlist2")

        onClicked: player.listPlaylist("PlaylistY")
    }

    Button {
        id: button16
        x: 566
        y: 23
        text: qsTr("Add playlist2")

        onClicked: {
            player.addPlaylist("PlaylistY")
            player.clearPlaylist("PlaylistY")

        }
    }

    Text {
        id: element
        x: 28
        y: 351
        width: 308
        height: 27
        visible: true
        font.pixelSize: 18
        color: "#c9c7c7"
        text:player.currentPlayedInfo
    }


}
