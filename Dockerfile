FROM gcc:10
RUN apt update && \
    apt install -y make \
    cmake \
    python3-pip
RUN pip3 install conan


