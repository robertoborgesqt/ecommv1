import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Controls.Material 2.4
import Cliente 1.0
import Endereco 1.0
import Produto 1.0


ApplicationWindow {
    id: window
    width: 480
    height: 840
    visible: true
    title: qsTr("ECommClient")

    property bool b_endereco_visible: false
    property bool b_main_visible: true
    property string str_nome
    property string str_logradouro
    property string str_cidade
    property string str_uf
    property int id_usuario
    property int contador
    property double valor_total : 0
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Component.onCompleted: {
        enderecoEntrega.onEnderecoFechado.connect(window.enderecoFechado);
        cliente.executeUpload = true;
    }

    Cliente {
        id: cliente
        onExecuteUploadChanged:{
            str_nome = nome;
            id_usuario = id;
            endereco.idusuario = id;
        }
    }

    Endereco {
        id: endereco
        onExecuteUploadChanged:{
            str_logradouro = logradouro;
            str_cidade = cidade;
            str_uf = uf;
            produto.executeUpload = true;

        }
    }

    Produto {
        id: produto
        onAtualizaLista:{
            var list = JSON.parse(listastr);
            listModel.clear();
            for (var i in list["body"])
            {
                var id = list["body"][i].id;
                var descr = list["body"][i].descr;
                var preco = list["body"][i].preco;

                listModel.append(
                            {
                                "id": id  ,
                                "descr": descr,
                                "preco":preco
                            }
                            );

            }
        }
    }

    ListModel{
        id: listModel
    }

    function enderecoFechado(){
        b_endereco_visible = false;
        b_main_visible = true;
    }

    Rectangle {
        id: rectangle
        y: 0
        visible: b_main_visible
        height: 68
        color: "#b07bdd"
        anchors.verticalCenter: listView.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.leftMargin: 0

        Image {
            id: image
            x: 8
            y: 13
            width: 48
            height: 47
            source: "imagens/user.png"
            mipmap: false
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: text1
            x: 74
            y: 27
            width: 106
            height: 14
            text: qsTr("Receber agora em:")
            font.pixelSize: 12
        }


        Image {
            id: image1
            x: 499
            y: -13
            width: 25
            height: 25
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            source: "imagens/back.png"
            mirror: true
            anchors.rightMargin: 10
            fillMode: Image.PreserveAspectFit
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    b_main_visible = false;
                    b_endereco_visible = true;
                }
            }
        }

        Text {
            id: text2
            x: 74
            y: 41
            width: 221
            height: 14
            text: str_logradouro + ", " + str_cidade + " - " + str_uf;
            font.pixelSize: 12
        }

        Image {
            id: image2
            x: 364
            y: 10
            width: 32
            height: 32
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: image1.left
            source: "imagens/Ecommerce-Shopping-Cart-Empty-icon.png"
            anchors.rightMargin: 30
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: txtNome
            x: 74
            y: 13
            width: 106
            height: 14
            text: qsTr("Bom dia ") + str_nome
            font.pixelSize: 12
        }

        Text {
            id: txtContador
            x: 395
            y: 9
            width: 15
            height: 14
            color: "#d41111"
            text: contador
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            style: Text.Raised
        }
    }

    ScrollView {
        visible: b_main_visible
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: rectangle.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 25

        ListView {
            id: listView
            width: parent.width
            model: listModel
            delegate: Button {
                width: parent.width / 1.07
                anchors.horizontalCenter: parent.horizontalCenter
                text: id + ' '+ descr + ' ' + preco + ' '
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        contador += 1;
                        valor_total += roundNumber(preco,2);
                        console.log( 'total = '+valor_total );
                        txtContador = contador;
                    }
                }
            }
        }
    }

    function roundNumber(number, decimals) {
        var newnumber = new Number(number+'').toFixed(parseInt(decimals));
        return parseFloat(newnumber);
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    EnderecoEntrega{
        id: enderecoEntrega
        visible: b_endereco_visible
        anchors.fill: parent
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2}
}
##^##*/
