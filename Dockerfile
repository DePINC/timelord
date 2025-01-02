FROM debian:stable AS build

ARG PROXY

RUN mkdir /app

# Add proxy configuration if PROXY is set
RUN if [ -n "$PROXY" ]; then \
      echo "Acquire::http::Proxy \"$PROXY\";" >> /etc/apt/apt.conf && \
      echo "Acquire::https::Proxy \"$PROXY\";" >> /etc/apt/apt.conf && \
      echo "proxy=$PROXY" > ~/.curlrc && \
      export ALL_PROXY=$PROXY && \
      export HTTP_PROXY=$PROXY && \
      export HTTPS_PROXY=$PROXY; \
    fi

RUN apt update && apt upgrade --yes
RUN apt install curl gcc zip unzip tar cmake g++ pkg-config automake autoconf git libssl-dev libtool yasm texinfo libboost-all-dev libgmp-dev --yes

RUN if [ -n "$PROXY" ]; then \
      git config --global http.proxy $PROXY && \
      git config --global http.sslVerify false; \
    fi

RUN git clone https://github.com/Kitware/CMake /cmake && cd /cmake && ./configure && make -j3 && make install

COPY . /timelord
RUN cd /timelord && git clean -xfd && git submodule update --init
RUN cd /timelord/vcpkg && git clean -xfd && ./bootstrap-vcpkg.sh
RUN cd /timelord && cmake . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j7 && cp build/timelord /app

RUN git clone https://github.com/chia-network/chiavdf /chiavdf && cd /chiavdf/src && make -f Makefile.vdf-client && cp /chiavdf/src/vdf_client /app && cp /chiavdf/src/vdf_bench /app

FROM debian:stable AS main

COPY --from=build /app/* /

RUN apt update && apt install libgmp-dev --yes

EXPOSE 19191

EXPOSE 39393

ENTRYPOINT [ "/timelord", "--vdf_client-path=/vdf_client", "--db=/mnt/timelord.sqlite3", "--bind=0.0.0.0", "--port=19191", "--logfile=/mnt/timelord.log" ]
