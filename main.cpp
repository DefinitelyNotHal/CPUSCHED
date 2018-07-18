#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iterator>
#include <deque>
#include <algorithm>
#include "datagen.cpp"
using namespace std;

///Process Structure
struct process
{
    int processID;
    int arrivalT;
    int CPUburstT;
    int ppriority;

};

///User menu
char menu()
{
    char choice;
    cout <<"===============menu====================";
    cout <<"\n1. FIFO";
    cout <<"\n2. SJF with preemption";
    cout <<"\n3. RR(with specified time quantum)";
    cout <<"\n4. Priority with preemption";
    cout <<"\n5. Exit\n";
    cout <<"=======================================\n";

    cin >> choice;
    return choice;

};

///find waiting time
float avgWaitingT(int ft[], process pro[],const int np)
{
    int wt[np];
    float avgwait=0.0;
    ///waitingT=finishT-arrivalT-burstT, and sum of all waitingT
    int sumwait=0;
    for (int i=0;i<np;i++)
    {
        wt[i]=ft[i]-pro[i].arrivalT-pro[i].CPUburstT;
        sumwait=sumwait+wt[i];
    }
    ///avg. WaitingT=sum of all waitingT/total # of processes
    avgwait=sumwait/float(np);
    return avgwait;
}

///find turnaround time
float avgTurnaroundT(int ft[],process pro[],const int np)
{
    int tat[np];
    float avgtat=0.0;
    ///turnaroundT=finishT-arrivalT, and sum of all turnaroundT
    int sumtat=0;
    for (int i=0;i<np;i++)
    {
        tat[i]=ft[i]-pro[i].arrivalT;
        sumtat=sumtat+tat[i];
    }
    ///avg. TurnaroundT=sum of all turnaroundT/ total # of processes
    avgtat=sumtat/float(np);
    return avgtat;
}

///find response time
float avgResponseT(int rt1st[], process pro[], const int np)
{
    int rt[np];
    float avgrest=0.0;
    ///responseT=the actual arrival time in CPU exec. - arrivalT, and sum of all responseT
    int sumrest=0;
    for (int i=0;i<np;i++)
    {
        rt[i]=rt1st[i]-pro[i].arrivalT;
        sumrest=sumrest+rt[i];
    }
    ///avg. ResponseT=sum of all responseT/total # of processes
    avgrest=sumrest/float(np);
    return avgrest;
}

///print statistics of the run to the screen
void printStatistics(int elapT, float thput, float CPUutil, float avgWaitT, float avgTurnaT, float avgRespT, const int np)
{
    cout<<"Statistics for the Run\n"<<endl;
    cout<<"Number of processes: "<<np<<endl;
    cout<<"Total elapsed time(for the scheduler): "<<elapT<<endl;
    cout<<"Throughput: "<<thput<<endl;
    cout<<"CPU utilization: "<<CPUutil<<"%"<<endl;
    cout<<"Average waiting time: "<<avgWaitT<<endl;
    cout<<"Average turnaround time: "<<avgTurnaT<<endl;
    cout<<"Average response time: "<<avgRespT<<endl;
    cout<<endl;
}

///used for sorting the process by its CPU burst time
bool checkbtvector(const process &a, const process &b)
{
    return a.CPUburstT<b.CPUburstT;
}
bool findMin(int i, int j){return (i<j);}

int main()
{
     ///generates the input file and prints the processes to the screen
     genInput();
     ///access to the file and retrieve the data
     ifstream inFile;
     inFile.open("processesInput.txt");
     if (!inFile)
        {  cout << "\n\n\t  INPUT FILE ERROR  \n\n";    }
     string header;
     getline(inFile,header);
     const int n=10000;//number of processes
     process pro[n];
     process tempPro[n];//a copy of pro
     int i=0;
     while(inFile.good()){inFile>>pro[i].processID>>pro[i].arrivalT>>pro[i].CPUburstT>>pro[i].ppriority;i++;}
     inFile.close();

     ///variables
     int elapsedT=0;
     float throuPut=0.0;
     float CPUutilize=0.0;
     float avgWaitingTime=0.0;
     float avgTurnaroundTime=0.0;
     float avgResponseTime=0.0;
     vector <int> idleT;//this is only used for FIFO
     deque <process> tempProReadyQ;
     deque <int> tempMin;
     //list <process> tempProReadyQ;
     //list <process>::iterator it;

     ///User's interface
     char menuChoice;
     menuChoice = menu();
     while( menuChoice !='1'||
            menuChoice !='2'||
            menuChoice !='3'||
            menuChoice !='4'||
            menuChoice !='5')
    {
        if(menuChoice=='1'){
            ///First In First Out, mainly depends on the arrival time
            ///Calculate the finish time
            int finishT[n];
            int actualArrivalT[n];//use for finding the response time
            idleT.clear();
            idleT.push_back(pro[0].arrivalT);
            finishT[0]=pro[0].arrivalT+pro[0].CPUburstT;
            actualArrivalT[0]=pro[0].arrivalT;
            for (int i=1; i<n;i++)
            {
                ///if CPU is idle, then arrival time = actual arrival time, so the finish time= current arrival time+ CPU burst time
                ///else, the previous finish time is the actual arrival time for the current process
                if(pro[i].arrivalT>finishT[i-1])
                {
                    idleT.push_back(pro[i].arrivalT-finishT[i-1]);
                    finishT[i]=pro[i].arrivalT+pro[i].CPUburstT;
                    actualArrivalT[i]=pro[i].arrivalT;
                }
                else
                {
                    ///finishT=the actual arrival time in CPU exec. + CPU burst time
                    finishT[i]=finishT[i-1]+pro[i].CPUburstT;
                    actualArrivalT[i]=finishT[i-1];
                }
            }
            ///calculate sum of CPU idle time
            int sumIdleT=0;
            for(unsigned int i=0;i<idleT.size();i++)
            {
                sumIdleT=sumIdleT+idleT[i];
            }
            ///total elapsed time(for the scheduler)=final completion time- total CPU idle time
            elapsedT=finishT[n-1]-sumIdleT;
            ///calculate throughput
            ///Throughput= # of process/(final completion time-time at which first process is brought to CPU)
            throuPut=float(n)/(finishT[n-1]-pro[0].arrivalT);
            ///calculate CPU utilization
            ///CPU utilization=(final completion time-the sum of CPU idle time)/final completion time
            CPUutilize=float(finishT[n-1]-sumIdleT)/finishT[n-1]*100;
            ///calculate avg. waiting time
            avgWaitingTime=avgWaitingT(finishT,pro,n);
            ///calculate avg. turnaround time
            avgTurnaroundTime=avgTurnaroundT(finishT,pro,n);
            ///calculate avg. response time
            avgResponseTime=avgResponseT(actualArrivalT, pro,n);
            ///print to screen
            printStatistics(elapsedT,throuPut,CPUutilize,avgWaitingTime,avgTurnaroundTime,avgResponseTime,n);
        }
        else if(menuChoice=='2'){
            ///Shortest Job First with preemption, mainly depends on CPU burst time
            ///variables
            int finishT[n];
            int actualArrivalT[n];
            int CPUrunT=0;
            int CPUidleT=0;
            int numProDone=0;
            int countV=0;
            int countN[n]={0};//used for counting the actual arrival time, to calculate the response time of each process

            ///make a copy of process pro, array
            for(unsigned int i=0;i<n;i++)
            {
                tempPro[i].processID=pro[i].processID;
                tempPro[i].arrivalT=pro[i].arrivalT;
                tempPro[i].CPUburstT=pro[i].CPUburstT;
                tempPro[i].ppriority=pro[i].ppriority;
            }
            while(numProDone<n)//checking if all the processes have done, if not, increment by 1.
            {
                int minCPUburstT=INT_MAX;
                int minProcessID=INT_MAX;
                ///Ready Queue Processes
                for(unsigned int i=0;i<n;i++)//get all the possible processes in ready queue, not include any process that has the CPUburstT of 0 or the remainingT of 0
                {
                    if ((tempPro[i].arrivalT<=CPUrunT)&&(tempPro[i].CPUburstT>0))
                    {
                        countV++; //used to check if there is any process in the ready queue, if countN=0, then it means CPU idle.
                        if(tempPro[i].CPUburstT<minCPUburstT)
                        {
                            minCPUburstT=tempPro[i].CPUburstT;//find the smallest CPU burst time
                            minProcessID=tempPro[i].processID;//find the smallest processID with the shortest CPU burst time
                        }
                        else if(tempPro[i].CPUburstT==minCPUburstT)//check if there is more than two process that has the same shortest CPUburstT
                        {
                            if (tempPro[i].processID<minProcessID)//find the smallest processID with the shortest CPU burst time
                            {
                                minProcessID=tempPro[i].processID;//check if there is more than two processes
                            }
                        }
                    }
                }
                if (countV==0)
                {
                    CPUidleT++;
                    CPUrunT++;
                }
                else
                {
                    tempPro[minProcessID-1].CPUburstT--;
                    countN[minProcessID-1]++;
                    if(countN[minProcessID-1]==1)
                    {
                        actualArrivalT[minProcessID-1]=CPUrunT;//only needs the 1st actual arrival time for each process
                    }
                    if(tempPro[minProcessID-1].CPUburstT==0)
                    {
                        finishT[minProcessID-1]=CPUrunT+1;
                        numProDone++;
                    }
                CPUrunT++;
                }
            }

            ///total elapsed time(for the scheduler)=final completion time- total CPU idle time
            elapsedT=finishT[n-1]-CPUidleT;
            ///calculate throughput
            ///Throughput= # of process/(final completion time-time at which first process is brought to CPU)
            throuPut=float(n)/elapsedT;
            ///calculate CPU utilization
            ///CPU utilization=(final completion time-the sum of CPU idle time)/final completion time
            CPUutilize=float(elapsedT)/finishT[n-1]*100;
            ///calculate avg. waiting time
            avgWaitingTime=avgWaitingT(finishT,pro,n);
            ///calculate avg. turnaround time
            avgTurnaroundTime=avgTurnaroundT(finishT,pro,n);
            ///calculate avg. response time
            avgResponseTime=avgResponseT(actualArrivalT,pro,n);
            ///print to screen
            printStatistics(elapsedT,throuPut,CPUutilize,avgWaitingTime,avgTurnaroundTime,avgResponseTime,n);
        }
        else if(menuChoice=='3'){
            ///Round Robin(with specific time quantum), mainly depends on the time quantum
            int numQuantum=0;
            cout<<"Please enter time quantum: "<<endl;
            cin>>numQuantum;
            int finishT[10000];
            int actualArrivalT[10000];
            idleT.clear();
            idleT.push_back(pro[0].arrivalT);

        }
        else if(menuChoice=='4'){
            ///Priority with preemption, mainly depends on priority, smaller number means higher priority
            int finishT[10000];
            int actualArrivalT[10000];
            idleT.clear();
            idleT.push_back(pro[0].arrivalT);

        }
        else if(menuChoice=='5'){
            cout<<"Program is terminated."<<endl;
            return 0;
        }
        menuChoice = menu();
    }///end of while loop
    return 0;
}




