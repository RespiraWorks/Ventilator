# Important: this Dockerfile requires that the repo root be build context (in order to include the common code)

FROM debian:buster

COPY . .
RUN bash software/gui/gui.sh --install
RUN bash software/gui/gui.sh --build
CMD ["bash"]
