# This Dockerfile creates an image that has the necessary tools to build and test the controller code.
# It replicates the same environment as the one used in TravisCI (see the .travis.yml file in this repo),
# though without the GUI build tools and dependencies.
FROM ubuntu:bionic
RUN apt-get update && apt-get install build-essential python-pip git -y
RUN pip install -U pip

# NOTE: pre-commit version is pinned to prevent installing a Python 3-only version.
RUN pip install pre-commit==1.21.0 platformio
RUN platformio update
WORKDIR /root/VentilatorSoftware
COPY . ./
CMD /bin/bash test.sh
