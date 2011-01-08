MsgIO
====
A windows/wince fork of [mpio](http://github.com/frsyuki/mpio)

## Introduce
In process of migrating [msgpack-rpc](http://github.com/msgpack/msgpack-rpc) to wince platform, I firstly tried to make mpio work on it.
But since it is hard to provide a similar implement such as epoll/kqueue which is core of mpio, I have to make my own implement.Since I have made the model hardly involved with win32 socket & its Message Queue, I changed the name to MsgIO.

## About implemention
Since no IOCP support in win32, I build the kernel with WSAAsyncSelect & Message Queue.

Contributing
------------
Submit a issue if you found a problem, thanks!
