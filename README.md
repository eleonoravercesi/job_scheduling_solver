## Installation instructions
1. Install SCIPOPT SUITE https://scipopt.org/index.php#download
2. Replace line 4 of the Makefile with the path to your SCIPOPT installation.
3. Run `make` in the terminal. Some errors will arise for sure. The one that appeared to me can be solved by the following:
```bash
sudo apt install libmetis-dev
```
4. Something called `schedule` should appear, run

```bash
schedule input_file
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