# Analysis

```{contents}
:depth: 3
```

## Task description

At [bigf.json.bz2](https://quiz.storpool.com/bigf.json.bz2) there is a compressed
JSON file. In it are described disks with their model and serial number. Count
how many models there are and how many times each one is present.

## Implementation

The program uses the [yajl](https://github.com/lloyd/yajl) C library to process
the input JSON file. This library has been choosen due to its ability to process
incrementally JSON data off a stream.

The file is read on chunks of 65536 bytes. Each JSON dictionary:

```
{"id":0,"model":"XXXX","serial":"XXXX"}
```

is processed separately and the `model` data is extracted and stored into a hash
table.

The model string is stored into the hash table using the `disk_ht_insert` function.

```{eval-rst}
.. autocfunction:: disk_ht.h::disk_ht_insert
```

The hash table is with constant size of `DISK_HT_LEN 20` items. Collisions are resolved
by external chaining overlaping hash ids to a linked list on the same index.

```{note}
The program is constrained by the buffer used for storing the model string which
is set to 1000 bytes by the `DISK_MODEL_LEN_MAX` define.
```

## Tests

The test *analysis/test/analysis.py* processes the data stored in `data_sample.json`
and compares it with the output of the binary `analysis`.

**Test output**

```console
C binary /home/iliya/Work/StorPool/StorPool/build/analysis/analysis output:
Info: Processing: /home/iliya/Work/StorPool/StorPool/analysis/test/data_sample.json ...
Info: Disk data
     HGST2048T: 7
     SSDLP2: 5
     broken: 13
     HGST3T: 3
     DRV1: 12
     RDV2: 7
     HGST8T: 13
     SSDF1: 10
     SCSI3HD: 6
     DSD07461: 10
     123456789: 8
     SSDDC1: 6
Info: Total entries: 100

Ok
```

Output for `bigf.json`

```console
$ analysis -i ~/Downloads/bigf.json
Info: Processing: /home/iliya/Downloads/bigf.json ...
Info: Disk data
      HGST2048T: 33332531
      SSDLP2: 33345174
      broken: 33328584
      HGST3T: 33337292
      DRV1: 33338513
      RDV2: 33332954
      MODEL: 1
      HGST8T: 33337967
      SSDF1: 33328579
      SCSI3HD: 33329611
      DSD07461: 33333959
      123456789: 33327094
      SSDDC1: 33327742
Info: Total entries: 400000001
```
