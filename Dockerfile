FROM debian:stable AS build

RUN mkdir /app

# RUN echo 'Acquire::http::Proxy "http://192.168.1.200:8080";' >> /etc/apt/apt.conf
# RUN echo 'Acquire::https::Proxy "http://192.168.1.200:8080";' >> /etc/apt/apt.conf

# RUN echo "proxy=http://192.168.1.200:8080" > ~/.curlrc

# RUN export ALL_PROXY=http://192.168.1.200:8080
# RUN export HTTP_PROXY=http://192.168.1.200:8080
# RUN export HTTPS_PROXY=http://192.168.1.200:8080

RUN apt update && apt upgrade --yes
RUN apt install curl gcc zip unzip tar cmake g++ pkg-config automake autoconf git libssl-dev libtool yasm texinfo libboost-all-dev libgmp-dev --yes

# RUN git config --global http.proxy http://192.168.1.200:8080
# RUN git config --global http.sslVerify false

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

CMD ["/timelord"]