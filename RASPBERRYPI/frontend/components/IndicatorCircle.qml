import QtQuick

Rectangle {
    id: inferencedClassCircleColor
    property var circleColor: [
        "red", "blue", "green", "yellow", "purple", "orange",
        "pink", "cyan", "magenta", "brown", "gray", "lime",
        "teal", "indigo", "gold", "silver", "maroon", "navy", "turquoise"
    ]

    implicitWidth: 8
    implicitHeight: 8
    radius: implicitWidth / 2  // Hace que sea un círculo

}
