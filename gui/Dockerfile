# Important: this Dockerfile requires that the repo root be build context (in order to include the common code)

FROM debian:buster

COPY . .
RUN bash gui/gui.sh --install
RUN bash gui/gui.sh --build
CMD ["bash"]
