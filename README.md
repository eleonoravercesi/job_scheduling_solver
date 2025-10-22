# Simple SCIP implementation of an Identical Machine Job Scheduling Problem

## Installation instructions
1. Install [SCIPOptSuite](https://scipopt.org/index.php#download)
2. Replace line 4 of the Makefile with the path to your SCIPOptSuite installation.
3. Run `make` in the terminal. Some errors will arise for sure. The one that appeared to me can be solved by the following:
```bash
sudo apt install libmetis-dev
```
4. Something called `schedule` should appear, so you have your nice solver :)

## Usage from command line
From command line, you can run the solver as follows:
```bash
./schedule data.txt
```
Where input file is something like
```
m
p_1
p_2
...
p_n
```
WARNING: no final new line

## Usage in python
In principle, you would love to use the code directly in python, and for precisely this reason I have implemented a python function
that works as follows

```python
T = run_schedule(P, m, schedule_path=None, remove_tmp=True)
```
Where:
- `P` is a list of processing times
- `m` is the number of machines
- `schedule_path` is the path to the schedule executable. If None, it throws an error.
- `remove_tmp` is a boolean that indicates whether to remove the temporary input/output files created (the tmp.txt file and the tmp.log file with all the execution details)
- `T` is the optimal makespan found

Does it work? No, and I don't know why :)

## Some performances considerations:
May be helpful to run them in parallel and pick the first one that finishes.
```
seed 3, n = 100, m = 50
Optimal solution found!
OPT =  97.99999999999999 ; Python time =  18.122  seconds
C++ SCIP Runtime: 8.40254 seconds
```

```
seed 3, n = 100, m = 70
Optimal solution found!
OPT =  98.0 ; Python time =  12.124  seconds
C++ SCIP Runtime: 20.2296 seconds
```

```
seed 3, n = 100, m = 90
Optimal solution found!
OPT =  98.0 ; Python time =  17.422  seconds
C++ SCIP Runtime: 32.1434 seconds
```

```
seed 41, n = 50, m = 20
Processing times generated.
Optimal solution found!
OPT =  129.0 ; Python time =  17.552  seconds
C++ SCIP Runtime: 90.747 seconds
```

