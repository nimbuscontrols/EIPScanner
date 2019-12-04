FROM gcc:8

RUN apt-get update && apt-get install -y git cmake libcap-dev

RUN git clone https://github.com/EIPStackGroup/OpENer.git --depth=1
WORKDIR /OpENer/bin/posix
RUN sh setup_posix.sh
RUN make -j4