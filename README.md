# ecommv1
 QT QML Http server/app
 
 Este pacote é constituido das seguintes aplicações:
 ECommClient - É a solução escrita em QT/QML/C++ que demonstra os acessos ao ECommServer utilizando HTTP/Json
 ECommServer - Baseado no framework HttpC++ stephen frings, traduz as requisições em SQL e retorna resultos em Json. 
 Mysql - Banco de dados via imagem docker configurado com um script SQL
 
 
 CLIENT --requisição http ---> SERVER --> Traduz requisição ---> MYSQL ---> executa SQL traduzido ---> Retorna JSon
 |                               |                                 |
 |                               |                                 |
 > Client Maquina Local          V                                 |
                                 Imagem docker porta 9000          |
                                                                   V
                                                                   DB instalado porta 3308
                  
# Banco de dados
Baixar imagem docker mysql:

sudo docker pull mysql/mysql-server:latest

Executar a imagem
sudo docker run --name=[container_name] -d mysql/mysql-server:latest
utilizar o arquivo script.sql localizado na pasta app para criar o banco  
                                                                   
# Configurando server-http
Configurar o arquivo app/etc/datasrv.ini com as opções de Arquivos de log.
Configurar o arquivo app/etc/dbconfig.json com as opções de acesso ao banco de dados.
Necessário informar o ip/porta que deseja compartilhar o serviço (Veja opções de container)
No docker, abrir o terminal e: 
cd app/debug
./datasrv 9000



                                                                   
 
