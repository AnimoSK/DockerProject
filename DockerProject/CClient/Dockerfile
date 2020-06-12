FROM gcc
COPY . /usr/src/client
WORKDIR /usr/src/client
RUN gcc -o client client.c
CMD [ "./client" ]