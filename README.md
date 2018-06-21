# `qpuinfo`

Show information of VideoCore IV V3D of Raspberry Pi.


## Requirements

You need [mailbox](https://github.com/Terminus-IMRC/mailbox) and
[libvc4regmap](https://github.com/Terminus-IMRC/libvc4regmap).


## Building

```
$ make
```


## Running

To show info:
```
$ sudo ./qpuinfo -e -d
```

To enable QPU:
```
$ sudo ./qpuinfo -e
```

To disable QPU after above:
```
$ sudo ./qpuinfo -d
```
