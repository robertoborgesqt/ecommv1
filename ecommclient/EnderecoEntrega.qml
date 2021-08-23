import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: enderecoEntrega
    Material.theme: Material.Normal
    Material.accent: Material.Normal
    Material.foreground: Material.color(Material.Black)
    color: "#b07bdd"


    signal onEnderecoFechado()

    Rectangle {
        color: "#b07bdd"
        id: rectangle1
        x: 0
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        Text {
            id: text2
            x: 84
            y: 18
            width: 80
            height: 14
            text: qsTr("Endereço")
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Image {
            id: image
            x: 8
            y: 8
            width: 24
            height: 24
            source: "imagens/back.png"
            fillMode: Image.PreserveAspectFit

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: onEnderecoFechado()
            }
        }
    }

    Rectangle {
        id: rectangle
        height: 223
        color: "#c5adad"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: rectangle1.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        Texto {
            id: textEndereco
            anchors.top: parent.top
            anchors.topMargin: 8
            placeholderText: qsTr("Informe o endereço")
        }

        Texto {
            id: textCidade
            anchors.top: textEndereco.bottom
            anchors.topMargin: 10
            placeholderText: qsTr("Informe a cidade e estado")
        }
        Texto {
            id: textComplemento
            anchors.top: textCidade.bottom
            anchors.topMargin: 10
            placeholderText: qsTr("Informe o complemento")
        }
        Texto {
            id: textReferencia
            anchors.top: textComplemento.bottom
            anchors.topMargin: 10
            placeholderText: qsTr("Informe o ponto de referência")
        }

    }

    Rectangle {
        id: rectangle4
        height: 108
        color: "#c5adad"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: rectangle1.bottom
        anchors.rightMargin: 0
        anchors.topMargin: 224
        anchors.leftMargin: 0

        Botao {
            width: 164
            height: 41
            text: qsTr("Casa")
            anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                checked: true
                autoExclusive: true
                checkable: true
                anchors.leftMargin: 10
        }

        Botao {
            width: 164
            text: qsTr("Trabalho")
            anchors.verticalCenter: parent.verticalCenter
            checkable: true
            autoExclusive: true
            anchors.horizontalCenter: parent.horizontalCenter
            }

        Botao {
            width: 164
            text: qsTr("Outro")
            anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                autoExclusive: true
                checkable: true
                anchors.rightMargin: 10
            }
    }

    Rectangle {
        id: rectangle7
        color: "#c5adad"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: rectangle4.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 1
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        anchors.leftMargin: 0

        Botao {
            id: rectangle6
            width: 345
            height: 48
            text: qsTr("Continuar")
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:6}
}
##^##*/
