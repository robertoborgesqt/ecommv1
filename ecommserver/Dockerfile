FROM rabits/qt:5.15-desktop as ecommsrv
MAINTAINER RobertoSB "rborges67@me.com"

RUN sudo apt-get update -qq \
    && sudo apt-get install -y -qq python3 python3-pip mysql-client libmariadbclient-dev net-tools \
    && pip3 install gcovr \
    && sudo apt-get clean
RUN cd /tmp && curl -L -O "https://download.qt.io/official_releases/qt/5.15/5.15.2/submodules/qtbase-everywhere-src-5.15.2.tar.xz" \
    && tar xvJf qtbase-everywhere-src-5.15.2.tar.xz 
RUN cd /tmp/qtbase-everywhere-src-5.15.2/src/plugins/sqldrivers \
    && qmake -- MYSQL_INCDIR="/usr/include/mysql" MYSQL_LIBDIR="/usr/lib/x86_64-linux-gnu" \
    && sudo make sub-mysql \
    && sudo make install \
    && sudo rm -rf /tmp/qtbase-everywhere-src-* 

RUN mkdir /home/user/app
COPY app/. /home/user/app
RUN cd app && qmake && make 
RUN cd app && cd debug && ./datasrv 9000 &
# network create  --driver=bridge   --subnet=172.28.0.0/16   --ip-range=172.28.5.0/24  --gateway=172.28.5.254   br0

EXPOSE 9000


 




