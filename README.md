# Gomoku

## Dependencies

sfml
boost

## Building

```bash
mkdir build
cd build
cmake ..
make
cd ..
./build/gomoku
```

## Todo

* only show greenscreen when necessary
* getNextMove returns pos vector, and play chooses
* display real suggestions instead of heatmap
* negamax, negamax_thread (and maybe start_negamax) should all be static
* more documentation
* remove boost dependency
* 