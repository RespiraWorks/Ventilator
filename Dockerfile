FROM ubuntu:bionic
RUN apt-get update && apt-get install build-essential python-pip git -y
RUN pip install -U pip
RUN pip install pre-commit==1.21.0 platformio
WORKDIR /root/VentilatorSoftware
COPY . ./
CMD /bin/bash
