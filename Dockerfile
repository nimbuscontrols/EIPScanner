FROM gcc:8

RUN apt-get update && apt-get install -y git cmake

RUN mkdir /git
WORKDIR /git

RUN git clone https://github.com/google/googletest.git \
    && cd /git/googletest \
    && git checkout release-1.8.1 \
    && cmake . \
    && cmake --build . --target install \
    && rm /git/googletest -rf

WORKDIR /
ADD / /code
RUN mkdir build
WORKDIR build
RUN cmake -DTEST_ENABLED=ON /code
RUN cmake --build .