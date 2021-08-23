import QtQuick 2.0
import QtQuick.Controls 2.15

TextField {
    id: textEdit
    y: 8
    height: 47
    anchors.left: parent.left
    anchors.right: parent.right
    font.pixelSize: 12
    anchors.rightMargin: 10
    anchors.leftMargin: 39
    Text {
        text: textEdit.placeholderText
        color: "#aaa"
        visible: !textEdit.text && !textEdit.activeFocus
    }
}
