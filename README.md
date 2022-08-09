# Reverse shell in C using socket programming

The project concerns the realization of a reverse shell in C using socket programming. In order to do this, we need to use the concept of the client/server relationship with the TCP network protocol.

## Disclamer

This project is carried out in the context of a course and is only made public for educational purpose. Any unauthorized use of this software for malicious purposes may be punishable by law. 

## Getting started

To start the server, you need to specify two arguments : **Port number** | **IP address**

```console
$ ./server port_number IP_address
```

Once connected, the server will wait for the client to connect and if it connects it will display the client's ip address.

![Server command lines](./Images/server.png)

To start the client, you need to do the same thing that for the server.

```console
$ ./client port_number IP_address
```
