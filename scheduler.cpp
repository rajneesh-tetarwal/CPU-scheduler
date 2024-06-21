#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <list>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime; // For RR and other algorithms
    int priority; // For priority scheduling
    int completionTime;
    int waitingTime;
    int turnaroundTime;

    Process()
        : id(0), arrivalTime(0), burstTime(0), remainingTime(0),
          priority(0), completionTime(0), waitingTime(0), turnaroundTime(0) {}

    Process(int id, int arrival, int burst, int priority = 0)
        : id(id), arrivalTime(arrival), burstTime(burst), remainingTime(burst), priority(priority),
          completionTime(0), waitingTime(0), turnaroundTime(0) {}
};

void resetProcesses(std::vector<Process>& processes) {
    for (auto& process : processes) {
        process.remainingTime = process.burstTime;
        process.completionTime = 0;
        process.waitingTime = 0;
        process.turnaroundTime = 0;
    }
}

std::pair<double, double> evaluateAlgorithm(const std::vector<Process>& processes) {
    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;

    for (const auto& process : processes) {
        totalWaitingTime += process.waitingTime;
        totalTurnaroundTime += process.turnaroundTime;
    }

    double avgWaitingTime = totalWaitingTime / processes.size();
    double avgTurnaroundTime = totalTurnaroundTime / processes.size();

    return {avgWaitingTime, avgTurnaroundTime};
}

std::vector<int> FCFS(std::vector<Process>& processes) {
    resetProcesses(processes);
    std::vector<int> ganttChart;
    int currentTime = 0;

    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    for (auto& process : processes) {
        if (currentTime < process.arrivalTime) {
            currentTime = process.arrivalTime;
        }
        currentTime += process.burstTime;
        process.completionTime = currentTime;
        process.turnaroundTime = process.completionTime - process.arrivalTime;
        process.waitingTime = process.turnaroundTime - process.burstTime;
        ganttChart.push_back(process.id);
    }

    return ganttChart;
}

std::vector<int> SJF(std::vector<Process>& processes) {
    resetProcesses(processes);
    std::vector<int> ganttChart;
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();
    int minBurstTime;
    int shortest = 0;
    bool found;

    while (completed != n) {
        found = false;
        minBurstTime = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && processes[i].remainingTime < minBurstTime) {
                minBurstTime = processes[i].remainingTime;
                shortest = i;
                found = true;
            }
        }

        if (!found) {
            currentTime++;
            continue;
        }

        currentTime += processes[shortest].remainingTime;
        processes[shortest].remainingTime = 0;
        processes[shortest].completionTime = currentTime;
        processes[shortest].turnaroundTime = processes[shortest].completionTime - processes[shortest].arrivalTime;
        processes[shortest].waitingTime = processes[shortest].turnaroundTime - processes[shortest].burstTime;
        ganttChart.push_back(processes[shortest].id);
        completed++;
    }

    return ganttChart;
}

std::vector<int> PriorityScheduling(std::vector<Process>& processes) {
    resetProcesses(processes);
    std::vector<int> ganttChart;
    int currentTime = 0;
    int completed = 0;
    int n = processes.size();
    int highestPriority;
    int highestPriorityIndex;
    bool found;

    while (completed != n) {
        found = false;
        highestPriority = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                highestPriorityIndex = i;
                found = true;
            }
        }

        if (!found) {
            currentTime++;
            continue;
        }

        currentTime += processes[highestPriorityIndex].remainingTime;
        processes[highestPriorityIndex].remainingTime = 0;
        processes[highestPriorityIndex].completionTime = currentTime;
        processes[highestPriorityIndex].turnaroundTime = processes[highestPriorityIndex].completionTime - processes[highestPriorityIndex].arrivalTime;
        processes[highestPriorityIndex].waitingTime = processes[highestPriorityIndex].turnaroundTime - processes[highestPriorityIndex].burstTime;
        ganttChart.push_back(processes[highestPriorityIndex].id);
        completed++;
    }

    return ganttChart;
}

std::vector<int> RoundRobin(std::vector<Process>& processes, int timeQuantum) {
    resetProcesses(processes);
    std::list<Process*> processQueue;
    std::vector<int> ganttChart;
    int currentTime = 0;

    for (auto& process : processes) {
        if (process.arrivalTime <= currentTime) {
            processQueue.push_back(&process);
        }
    }

    while (!processQueue.empty()) {
        Process* process = processQueue.front();
        processQueue.pop_front();

        if (currentTime < process->arrivalTime) {
            currentTime = process->arrivalTime;
        }

        if (process->remainingTime > timeQuantum) {
            currentTime += timeQuantum;
            process->remainingTime -= timeQuantum;
            ganttChart.push_back(process->id);

            for (auto& p : processes) {
                if (p.arrivalTime <= currentTime && p.remainingTime > 0 && std::find(processQueue.begin(), processQueue.end(), &p) == processQueue.end()) {
                    processQueue.push_back(&p);
                }
            }

            processQueue.push_back(process);
        } else {
            currentTime += process->remainingTime;
            process->remainingTime = 0;
            process->completionTime = currentTime;
            process->turnaroundTime = process->completionTime - process->arrivalTime;
            process->waitingTime = process->turnaroundTime - process->burstTime;
            ganttChart.push_back(process->id);

            for (auto& p : processes) {
                if (p.arrivalTime <= currentTime && p.remainingTime > 0 && std::find(processQueue.begin(), processQueue.end(), &p) == processQueue.end()) {
                    processQueue.push_back(&p);
                }
            }
        }
    }

    return ganttChart;
}

std::string selectBestAlgorithm(std::vector<Process>& processes, int timeQuantum) {
    std::vector<Process> processesCopy = processes;

    std::vector<int> fcfsGantt = FCFS(processesCopy);
    std::pair<double, double> fcfsEval = evaluateAlgorithm(processesCopy);

    processesCopy = processes;
    std::vector<int> sjfGantt = SJF(processesCopy);
    std::pair<double, double> sjfEval = evaluateAlgorithm(processesCopy);

    processesCopy = processes;
    std::vector<int> priorityGantt = PriorityScheduling(processesCopy);
    std::pair<double, double> priorityEval = evaluateAlgorithm(processesCopy);

    processesCopy = processes;
    std::vector<int> rrGantt = RoundRobin(processesCopy, timeQuantum);
    std::pair<double, double> rrEval = evaluateAlgorithm(processesCopy);

    std::pair<std::string, std::pair<double, double>> bestAlgorithm = {"FCFS", fcfsEval};

    if (sjfEval.first < bestAlgorithm.second.first) {
        bestAlgorithm = {"SJF", sjfEval};
    }
    if (priorityEval.first < bestAlgorithm.second.first) {
        bestAlgorithm = {"Priority Scheduling", priorityEval};
    }
    if (rrEval.first < bestAlgorithm.second.first) {
        bestAlgorithm = {"Round Robin", rrEval};
    }

    return bestAlgorithm.first;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <timeQuantum> <processesJson>\n";
        return 1;
    }

    int timeQuantum = std::stoi(argv[1]);
    json processesJson = json::parse(argv[2]);

    std::vector<Process> processes;
    for (const auto& item : processesJson) {
        processes.emplace_back(item["id"], item["arrivalTime"], item["burstTime"], item["priority"]);
    }

    json outputJson;

    std::vector<int> fcfsGantt = FCFS(processes);
    std::pair<double, double> fcfsEval = evaluateAlgorithm(processes);
    outputJson["FCFS"] = {{"ganttChart", fcfsGantt}, {"avgWaitingTime", fcfsEval.first}, {"avgTurnaroundTime", fcfsEval.second}};

    processes = processesJson.get<std::vector<Process>>();
    std::vector<int> sjfGantt = SJF(processes);
    std::pair<double, double> sjfEval = evaluateAlgorithm(processes);
    outputJson["SJF"] = {{"ganttChart", sjfGantt}, {"avgWaitingTime", sjfEval.first}, {"avgTurnaroundTime", sjfEval.second}};

    processes = processesJson.get<std::vector<Process>>();
    std::vector<int> priorityGantt = PriorityScheduling(processes);
    std::pair<double, double> priorityEval = evaluateAlgorithm(processes);
    outputJson["PriorityScheduling"] = {{"ganttChart", priorityGantt}, {"avgWaitingTime", priorityEval.first}, {"avgTurnaroundTime", priorityEval.second}};

    processes = processesJson.get<std::vector<Process>>();
    std::vector<int> rrGantt = RoundRobin(processes, timeQuantum);
    std::pair<double, double> rrEval = evaluateAlgorithm(processes);
    outputJson["RoundRobin"] = {{"ganttChart", rrGantt}, {"avgWaitingTime", rrEval.first}, {"avgTurnaroundTime", rrEval.second}};

    std::string bestAlgorithm = selectBestAlgorithm(processes, timeQuantum);
    outputJson["bestAlgorithm"] = bestAlgorithm;

    std::cout << outputJson.dump(4) << std::endl;

    return 0;
}
