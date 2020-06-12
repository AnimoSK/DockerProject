FROM gcc
COPY . /usr/src/server
WORKDIR /usr/src/server
RUN gcc -c server.c
RUN gcc -c encrypt.c
RUN gcc -o server server.o encrypt.o
CMD [ "./server" ]