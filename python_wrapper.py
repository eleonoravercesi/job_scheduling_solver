"""
This is not a real wrapper, it is just the function you may want to use
"""
import subprocess
import os

def run_schedule(P, m, schedule_path=None, remove_tmp=True):
    """
    Parameters
    ----------
    p   :   list
         The list of the processing time
    m   :   int
         The number of machines
    schedule_path   :   str
            The path of the schedule executable, MUST END WITH "schedule"
    remove_tmp  :   bool
            Whether to remove the temporary files or not (data file & log file)

    Returns
    -------
    T   :   int
        The optimal makespan (and that's it bc I am lazy, sorry :( )
    """
    # Checks
    assert schedule_path != None, "You should provide the path of the schedule executable"
    assert type(P) == list
    assert type(m) == int

    # Write p in a file
    F = open("tmp.txt", "w+")
    F.write(str(int(m)) + "\n")

    for idx, p in enumerate(P):
        if idx != len(P) - 1:
            F.write(f"{str(int(p))}\n")
        else:
            F.write(f"{str(int(p))}")
    # Run schedule
    this_dir =  os.getcwd()
    with open("tmp.log", "w+") as log:
        subprocess.run(
            [schedule_path, f"{this_dir}/tmp.txt"],
            stdout=log,
            stderr=subprocess.STDOUT,
            check=True
        )

    # Parse the result
    F = open("tmp.log", "r")
    lines = F.readlines()
    last_line = lines[-1]
    T = int(last_line.split(" ")[-1])
    # Remove temporary files
    if remove_tmp:
        subprocess.run("rm tmp.txt tmp.log", shell = True)
    return T

if __name__ == "__main__":
    P = [2, 4, 5, 6, 8, 10, 12, 15, 18, 20]
    m = 3
    schedule_path = "/home/vercee/Documents/job_scheduling_solver/schedule"
    T = run_schedule(P, m, schedule_path=schedule_path)
    print(f"Optimal makespan: {T}")


