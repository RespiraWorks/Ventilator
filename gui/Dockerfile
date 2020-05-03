FROM arm64v8/debian:buster

COPY . .
RUN bash ./gui.sh --install
RUN bash ./gui.sh --build
CMD ["bash"]
