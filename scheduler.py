import tkinter as tk
from tkinter import messagebox
import subprocess
import json

def run_scheduler():
    algorithm = algorithm_var.get()
    time_quantum = time_quantum_entry.get()
    processes = []
    
    try:
        time_quantum = int(time_quantum)
    except ValueError:
        messagebox.showerror("Invalid Input", "Time quantum must be an integer")
        return

    try:
        for process_entry in process_entries:
            id = int(process_entry[0].get())
            arrival = int(process_entry[1].get())
            burst = int(process_entry[2].get())
            priority = int(process_entry[3].get())
            processes.append({"id": id, "arrivalTime": arrival, "burstTime": burst, "priority": priority})
    except ValueError:
        messagebox.showerror("Invalid Input", "Process fields must be integers")
        return
    
    processes_json = json.dumps(processes)
    command = f"./scheduler {time_quantum} '{processes_json}'"
    
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True, check=True)
        output_json = json.loads(result.stdout)
        display_results(output_json)
    except subprocess.CalledProcessError as e:
        messagebox.showerror("Error", f"Scheduler failed: {e.stderr}")

def display_results(output_json):
    results = tk.Toplevel(app)
    results.title("Scheduler Results")

    for algo in ["FCFS", "SJF", "PriorityScheduling", "RoundRobin"]:
        algo_data = output_json.get(algo, {})
        if not algo_data:
            continue
        frame = tk.Frame(results)
        frame.pack(padx=10, pady=10)
        tk.Label(frame, text=f"{algo} Results").grid(row=0, column=0, columnspan=2)

        gantt_chart = " ".join("P" + str(id) for id in algo_data["ganttChart"])
        tk.Label(frame, text="Gantt Chart:").grid(row=1, column=0, sticky="e")
        tk.Label(frame, text=gantt_chart).grid(row=1, column=1, sticky="w")

        tk.Label(frame, text="Avg Waiting Time:").grid(row=2, column=0, sticky="e")
        tk.Label(frame, text=str(algo_data["avgWaitingTime"])).grid(row=2, column=1, sticky="w")

        tk.Label(frame, text="Avg Turnaround Time:").grid(row=3, column=0, sticky="e")
        tk.Label(frame, text=str(algo_data["avgTurnaroundTime"])).grid(row=3, column=1, sticky="w")

    best_algo = output_json.get("bestAlgorithm", "N/A")
    tk.Label(results, text=f"Best Algorithm: {best_algo}").pack(pady=10)

app = tk.Tk()
app.title("Process Scheduler")

tk.Label(app, text="Time Quantum").grid(row=0, column=0)
time_quantum_entry = tk.Entry(app)
time_quantum_entry.grid(row=0, column=1)

process_entries = []

def add_process():
    row = len(process_entries) + 1
    id_entry = tk.Entry(app)
    id_entry.grid(row=row, column=0)
    arrival_entry = tk.Entry(app)
    arrival_entry.grid(row=row, column=1)
    burst_entry = tk.Entry(app)
    burst_entry.grid(row=row, column=2)
    priority_entry = tk.Entry(app)
    priority_entry.grid(row=row, column=3)
    process_entries.append((id_entry, arrival_entry, burst_entry, priority_entry))

tk.Button(app, text="Add Process", command=add_process).grid(row=1, column=0, columnspan=4)
tk.Button(app, text="Run Scheduler", command=run_scheduler).grid(row=2, column=0, columnspan=4)

add_process()  # Add initial process entry row

app.mainloop()
