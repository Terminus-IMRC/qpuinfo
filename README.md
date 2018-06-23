# `qpuinfo`

Show information of VideoCore IV V3D of Raspberry Pi.


## Requirements

You need [mailbox](https://github.com/Terminus-IMRC/mailbox) and
[libvc4regmap](https://github.com/Terminus-IMRC/libvc4regmap).


## Building

```
$ git clone https://github.com/Terminus-IMRC/qpuinfo
$ cd qpuinfo/
$ cmake .
$ make
$ sudo make install
```

You can create `.deb` package and install it:

```
$ make package
$ sudo dpkg -i qpuinfo-x.y.z-system.deb
```


## Running

To show info:
```
$ sudo qpuinfo -e -d
```

To enable QPU:
```
$ sudo qpuinfo -e
```

To disable QPU after above:
```
$ sudo qpuinfo -d
```
