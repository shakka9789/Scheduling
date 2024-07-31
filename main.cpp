#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class node {
public:
    int jobID;
    int jobTime;
    node* next;

    node() : jobID(0), jobTime(0), next(NULL) {}

    node(int jobID, int jobTime, node *next) {
        this->jobID = jobID;
        this->jobTime = jobTime;
        this->next = next;
    }

    void printnode(node *node, ofstream &outFile) {
        outFile << "<" << jobID << "," << jobTime << "> ";
        if (node->next != NULL) {
            outFile << "-> ";
        }
    }
};

class schedule {
public:
    int numNodes, numProcs, procUsed, currentTime, totalJobTimes;
    int *jobTimeAry;
    int *jobStatus;
    int **Matrix;
    int **Table;
    node *OPEN;

    schedule(int numNodes, int numProcs) {
        procUsed = 0;
        currentTime = 0;
        totalJobTimes = 0;
        this->numNodes = numNodes;
        this->numProcs = numProcs;

        jobTimeAry = new int[numNodes + 1];
        for (int i = 0; i < numNodes + 1; i++) {
            jobTimeAry[i] = 0;
        }

        jobStatus = new int[numNodes + 1];
        for (int i = 0; i < numNodes + 1; i++) {
            jobStatus[i] = 1;
        }

        Matrix = new int*[numNodes + 1];
        for (int i = 0; i < numNodes + 1; i++) {
            Matrix[i] = new int[numNodes + 1];
            for (int j = 0; j <= numNodes; j++) {
                Matrix[i][j] = 0;
            }
        }

        OPEN = new node();
    }

    void loadTable() {
        Table = new int*[numProcs + 1];
        for (int i = 0; i <= numProcs; i++) {
            Table[i] = new int[totalJobTimes + 1];
            for (int j = 0; j <= totalJobTimes; j++) {
                Table[i][j] = 0;
            }
        }
    }

    void loadMatrix(ifstream &inFile) {
        int Ni = 0, Nj = 0;
        Matrix[0][0] = numNodes;

        while (inFile >> Ni >> Nj) {
            Matrix[Ni][Nj] = 1;
            Matrix[0][Nj]++;
            Matrix[Ni][0]++;
        }
    }

    int loadJobTimeAry(ifstream &inFile) {
        int jobID, time;
        while (inFile >> jobID >> time) {
            jobTimeAry[jobID] = time;
            totalJobTimes += time;
        }
        return totalJobTimes;
    }

    void setJobStatus(int i) {
        jobStatus[i] = 2;
    }

    void printMatrix(ofstream &outFile) {
        outFile << "Matrix:\n";
        for (int i = 0; i <= numNodes + 1; i++) {
            outFile << i << " ";
            if (i == 0) {
                outFile << " ";
            }
        }
        outFile << endl;

        for (int i = 0; i <= numNodes; i++) {
            outFile << "---";
        }
        outFile << endl;

        for (int i = 0; i <= numNodes; i++) {
            outFile << i + 1 << " |";
            for (int j = 0; j <= numNodes; j++) {
                outFile << Matrix[i][j] << " ";
            }
            outFile << endl;
        }
        outFile << "\n";
    }

    int findOrphan() {
        for (int i = 1; i < numNodes + 1; i++) {
            if (Matrix[0][i] == 0 && jobStatus[i] == 1) {
                setJobStatus(i);
                return i;
            }
        }
        return -1;
    }

    void OpenInsert(node *newNode) {
        node *tmp = OPEN;
        while ((tmp->next != NULL) && (Matrix[tmp->next->jobID][0] > Matrix[newNode->jobID][0])) {
            tmp = tmp->next;
        }
        newNode->next = tmp->next;
        tmp->next = newNode;
    }

    void printOPEN(ofstream &outFile) {
        node *tmp;
        if (OPEN->next != NULL) {
            tmp = OPEN->next;
            outFile << "Open list: ";
            while (tmp->next != NULL) {
                tmp->printnode(tmp, outFile);
                tmp = tmp->next;
            }
            if (tmp->next == NULL) {
                tmp->printnode(tmp, outFile);
                outFile << endl;
            }
        }
    }

    int getNextProc(int currentTime) {
        for (int i = 1; i <= numProcs; i++) {
            if (Table[i][currentTime] == 0) {
                return i;
            }
        }
        return -1;
    }

    void fillOPEN(ofstream &outFile) {
        int jobId = findOrphan();
        while (jobId != -1) {
            if (jobId > 0) {
                node *newNode = new node(jobId, jobTimeAry[jobId], NULL);
                OpenInsert(newNode);
                printOPEN(outFile);
            }
            jobId = findOrphan();
        }
    }

    void putJobOnTable(int availProc, int currentTime, int jobId, int jobTime) {
        int Time = currentTime;
        int EndTime = Time + jobTime;
        while (Time < EndTime) {
            Table[availProc][Time] = jobId;
            Time++;
        }
    }

    void fillTable() {
        int availProc = getNextProc(currentTime);
        while (availProc >= 0 && OPEN->next != NULL) {
            if (availProc >= 0) {
                node *newJob = OPEN->next;
                OPEN->next = OPEN->next->next;
                putJobOnTable(availProc, currentTime, newJob->jobID, newJob->jobTime);
                if (availProc > procUsed) {
                    procUsed++;
                }
                availProc = getNextProc(currentTime);
            }
        }
    }

    void printTable(ofstream &outFile, int currentTime) {
        for (int i = 0; i <= totalJobTimes; i++) {
            outFile << "===";
        }
        int procUse = 0;
        for (int i = 1; i <= numProcs; i++) {
            if (Table[i][currentTime] != 0) {
                procUse++;
            }
        }
        outFile << endl;
        outFile << "Scheduling Table: \nprocUsed: " << procUse << " " << "\t|\tcurrentTime: " << currentTime << endl;
        outFile << "Time:\t ";
        for (int k = 0; k <= totalJobTimes; k++) {
            outFile << " | " << k << " | ";
        }
        outFile << endl;
        for (int i = 1; i <= numProcs; i++) {
            outFile << "Proc: " << i;
            for (int j = 0; j <= totalJobTimes; j++) {
                if (j >= 10) {
                    outFile << " | " << Table[i][j] << " | ";
                } else {
                    outFile << " | " << Table[i][j] << " | ";
                }
            }
            outFile << endl;
        }
    }

    // Test
    bool checkCycle() {
        if ((OPEN->next == NULL) && (Matrix[0][0] > 0)) {
            for (int i = 1; i <= numProcs; i++) {
                if (Table[i][currentTime - 1] > 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    bool isGraphEmpty() {
        return Matrix[0][0] == 0;
    }

    void deleteJob(int jobId) {
        jobStatus[jobId] = 0;
        Matrix[0][0]--;
        for (int i = 1; i <= numNodes; i++) {
            if (Matrix[jobId][i] > 0) {
                Matrix[0][i]--;
            }
        }
    }

    void deleteDoneJobs(ofstream &outFile) {
        int proc = 1;
        while (proc <= procUsed) {
            if (Table[proc][currentTime] <= 0 && Table[proc][currentTime - 1] > 0) {
                int jobId = Table[proc][currentTime - 1];
                deleteJob(jobId);
            }
            printMatrix(outFile);
            proc++;
        }
    }
};

int main(int argc, char const *argv[]) {
    ifstream inFile1, inFile2;
    ofstream outFile1, outFile2;
    int numNodes, numProcs;

    inFile1.open(argv[1]);
    inFile2.open(argv[2]);
    numProcs = atoi(argv[3]); // 3
    outFile1.open(argv[4]);
    outFile2.open(argv[5]);

    inFile1 >> numNodes;
    inFile2 >> numNodes;

    outFile2 << "numNodes: " << numProcs << endl; // Test
    outFile2 << "numProcs: " << numProcs << endl; // Test

    if (numProcs <= 0) {
        cout << "need 1 or more processors\n";
        exit(0);
    } else if (numProcs > numNodes) {
        numProcs = numNodes;
    }

    schedule s(numNodes, numProcs);
    s.totalJobTimes = s.loadJobTimeAry(inFile2);
    outFile2 << "TotalJobTimes: " << s.totalJobTimes << endl;
    s.loadMatrix(inFile1);
    s.printMatrix(outFile2);
    s.loadTable();
    s.printTable(outFile1, s.currentTime);

    while (!s.isGraphEmpty()) {
        for (int i = 0; i <= numNodes; i++) {
            outFile2 << "===";
        }
        outFile2 << endl;

        s.fillOPEN(outFile2);
        if (s.checkCycle()) {
            outFile1 << "there is a cycle in the graph!!!\n";
            exit(0);
        }

        s.printOPEN(outFile2);
        s.fillTable();
        s.printTable(outFile1, s.currentTime);
        s.currentTime++;
        s.deleteDoneJobs(outFile2);
    }

    s.printTable(outFile1, s.currentTime);

    inFile1.close();
    inFile2.close();
    outFile1.close();
    outFile2.close();

    return 0;
}
