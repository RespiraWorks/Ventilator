# Copyright 2023, RespiraWorks
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

FROM ubuntu:22.04

RUN apt-get update && \
    apt-get -y install sudo

# add jenkins user
RUN adduser --disabled-password --gecos "" jenkins

# copy Ventilator repo to container
COPY --chown=jenkins . /home/jenkins/Ventilator/

# install common dependencies
ENV FORCED_ROOT=1
# the local time is set to skip prompting time select for the tzdata package
RUN ln -fs /usr/share/zoneinfo/America/Los_Angeles /etc/localtime && \
    DEBIAN_FRONTEND=noninteractive && \
    /home/jenkins/Ventilator/software/controller/controller.sh install

USER jenkins
WORKDIR /home/jenkins
