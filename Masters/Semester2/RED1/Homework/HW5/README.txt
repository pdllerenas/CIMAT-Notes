1. 
  gcc sock_server.c -o server 
  gcc sock_client.c -o client

En instancias de terminal distintas,
  ./server
  ./client

Luego, realizar la comunicación


2. 
  gcc sock_time_server.c -o server 
  gcc sock_time_client.c -o client

En instancias de terminal distintas,
  ./server
  ./client

Luego, el servidor regresa un tiempo aleatorio. El cliente toma el tiempo enviado y ajusta su reloj haciendo un offset de su reloj.

3.
  Toma 129 segundos. Al cambiar la ip del cliente en el programa `sock_client.c`, se muestra que la conexión excedió el tiempo de espera, que cuentan 129 segundos.