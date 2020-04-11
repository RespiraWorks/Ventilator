FROM arm64v8/debian:buster

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    qt5-default \
    qtbase5-dev-tools \
    qtdeclarative5-dev \
    qtmultimedia5-dev \
    libqt5charts5 \
    libqt5charts5-dev \
    qml-module-qtcharts \
    libqt5multimedia5-plugins \
    qtquickcontrols2-5-dev \
    qml-module-qtquick-controls \
    qml-module-qtquick-controls2 \
    qtdeclarative5-dev-tools \
    xvfb && \
    exit 0

COPY . .
RUN bash ./build.sh
CMD ["bash"]

