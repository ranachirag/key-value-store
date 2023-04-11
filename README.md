# Key-Value Store

Please take a look at [REPORT.md](./REPORT.md) for our report.

## Installation

### Cloning the repo 

```
git clone --recursive https://github.com/ranachirag/CSC443-project.git
```

### Running main code
```
./build.sh
./run_src.sh
```

### Running tests
```
./build.sh
./run_test.sh
```

### Running experiments
```
./build.sh
./run_experiments/step1_exp1.sh
./run_experiments/step3_exp1.sh
./run_experiments/step3_exp2.sh
```

### Running on Docker
```
docker build -t csc443 .
docker run -it csc443
git clone --recurse-submodule https://github.com/ranachirag/CSC443-project.git
cd CSC443-project
./build.sh
```

## Generate Documentation

- Install [doxygen](https://doxygen.nl/download.html)

```
doxygen Doxyfile
open ./html/index.html
```

## Authors

* Chirag Rana
* Rutav Shah
