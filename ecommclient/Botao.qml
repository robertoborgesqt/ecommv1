import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4


Button {
    id: control
    width: 120
    flat: false
    background: Rectangle {
        border.width: control.activeFocus ? 2 : 1
        border.color: "#888"
        radius: 4
        gradient: Gradient {
            GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
            GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
        }
    }
}
