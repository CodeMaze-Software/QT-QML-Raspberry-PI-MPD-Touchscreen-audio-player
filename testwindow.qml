import QtQuick.Controls 2.0
import QtQuick 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.1 // needed for the Window component

Window {

width: 300
height: 400

flags: Qt.FramelessWindowHint | Qt.Window

Rectangle {

    anchors.fill: parent

    color: "lightGrey"
    border.color: "#f7f7f7"

    Text {
        color: "#f5f4f4"

        anchors.centerIn: parent

        text: "My New Window"
    }
}
}
