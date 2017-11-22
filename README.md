# Qt TCP server template

## Description

Template application to write simple TCP servers with Qt.

## Requirements

- Tested with Qt 5.7.1
- You may use Qt Creator to build and run this application.

## Build

```
qmake && make
```

## Usage

```
./bin/tcp_server
```

The server listens at port 9999, press CTRL + C to exit.

You may use telnet as a client:

```
telnet localhost 9999
```

