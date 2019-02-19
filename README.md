# Mini-Aevol : A mini-application based on the Aevol simulator

A reduced version (from a model and implementation point of view) of Aevol.

DO NOT USE IT TO SIMULATE BIOLOGICAL RESULTS ! See [http://www.aevol.fr](http://www.aevol.fr) for that !

It must be used only to test HPC optimization of the code (parallel, vector, porting to new architecture...).

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

What things you need to install the software and how to install them.
First, you will need to install zlib (and its headers).

### Compilation

The compilation is straightforward

```
mkdir build
cd build
cmake ..
make
```

It will produced the executable pdc\_mini\_aevol.

## Running a simulation

A help (-e or --help) is given to explain the different parameters.

Basically, you must create a directory to store the simulation files (backup/checkpointing and stats files) and then run the simulation

```
mkdir simulation_example_1
cd simulation_example_1
../build/pdc_mini_aevol
```

## Work for the project

This repository has been created for a university project about HPC and performance evaluation.
Many branch and tag have been created to have multiple implementation of the same code to evaluate and analyse their performance.

All the date and the analyse script can be found in the folder [analysis](https://github.com/Webcretaire/micro-aevol/tree/master/analysis)

On the branch **evaluation** the folder [evaluation_script](https://github.com/Webcretaire/micro-aevol/tree/evaluation/evaluation_script) contains all the python3 scripts that are used to get all the data of the analysis. To generate all the data you will have to past this folder outside the git repository and run [evalutation.py](https://github.com/Webcretaire/micro-aevol/blob/evaluation/evaluation_script/evaluation.py)

## Authors

* **Julien Emanuel** - Student
* **Laurent Turpin** - Student

* **Jonathan Rouzaud-Cornabas** - *Initial work*

For the authors of Aevol software, see [http://www.aevol.fr](http://www.aevol.fr)

## License

This project is licensed under the GPLv2 License
