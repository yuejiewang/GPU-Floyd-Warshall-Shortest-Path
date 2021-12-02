# GPU Floyd-Warshall Shortest Path

## Data

This directory contains the data used in the experiments.  

To generate a random weighed graph of `V` nodes and `E` edges, run:  

``` shell
python3 g.py V E  
```

## CUDA

All cuda source code is under `GPU-Floyd-Warshall-Shortest-Path/cuda/` directory.  

To compile the GPU version of Floyd-Warshall algorithm, run:  

```
make  
```

Execute on input file `input.txt` and output to file `output.txt`, run:  

```
./gpu_floyd input.txt output.txt  
```

## Sequential

The sequential version used as benchmark is under `GPU-Floyd-Warshall-Shortest-Path/sequential/` directory.  

To compile the sequential version, run:  

```
make
```

Execute on input file `input.txt` and output to file `output.txt`, run:  

```
./seq_floyd input.txt output.txt  
```
