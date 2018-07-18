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
     //deque <process> tempProReadyQ;
     deque <int> tempMin;
     list <process> tempProReadyQ;
     list <process>::iterator it;

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
                ///Ready Queue Processes
                for(unsigned int i=0;i<n;i++)//get all the possible processes in ready queue, not include any process that has the CPUburstT of 0 or the remainingT of 0
                {
                    if ((tempPro[i].arrivalT<=CPUrunT)&&(tempPro[i].CPUburstT>0))
                    {
                        tempProReadyQ.push_back(tempPro[i]);//creating a copy of ready queue processes, list
                        /*
                        tempProReadyQ.push_back(process());//creating a copy of ready queue processes, vector
                        tempProReadyQ[countV].processID=tempPro[i].processID;
                        tempProReadyQ[countV].arrivalT=tempPro[i].arrivalT;
                        tempProReadyQ[countV].CPUburstT=tempPro[i].CPUburstT;
                        tempProReadyQ[countV].ppriority=tempPro[i].ppriority;
                        countV++;*/
                    }
                }
                countV=0;
                if(tempProReadyQ.empty())//if ready queue is empty and no process is brought in, then it means the CPU is idle
                {
                    CPUidleT++;
                    CPUrunT++;//anything before this count from 0, anything else below this count from 1
                }
                else if (!tempProReadyQ.empty())//else ready queue has processes
                {
                    if (tempProReadyQ.size()==1)//if there is only one process in the ready queue
                    {
                        tempPro[(tempProReadyQ.front().processID)-1].CPUburstT--;//list
                        countN[(tempProReadyQ.front().processID)-1]++;
                        if(countN[(tempProReadyQ.front().processID)-1]==1)
                        {
                          actualArrivalT[(tempProReadyQ.front().processID)-1]=CPUrunT;//only needs the 1st actual arrival time for each process
                        }
                        if(tempPro[(tempProReadyQ.front().processID)-1].CPUburstT==0)
                        {
                            finishT[(tempProReadyQ.front().processID)-1]=CPUrunT+1;
                            numProDone++;
                        }

                        /*tempPro[tempProReadyQ[0].processID-1].CPUburstT--;
                        countN[tempProReadyQ[0].processID-1]++;
                        if(countN[tempProReadyQ[0].processID-1]==1)
                        {
                          actualArrivalT[tempProReadyQ[0].processID-1]=CPUrunT;//only needs the 1st actual arrival time for each process
                        }
                        if(tempPro[tempProReadyQ[0].processID-1].CPUburstT==0)
                        {
                            finishT[tempProReadyQ[0].processID-1]=CPUrunT+1;
                            numProDone++;
                        }*/
                        CPUrunT++;
                    }
                    else//there are more than one process in the ready queue
                    {
                    sort(tempProReadyQ.begin(),tempProReadyQ.end(),checkbtvector);//sort the process based on their CPU burst time, smallest CPUburst time is the first element
                    for(list <process>::iterator it1=tempProReadyQ.begin();it1!=tempProReadyQ.end();++it1)//to collect the processes ID that has the same shortest CPUburstT
                    {
                        if(it->CPUburstT==tempProReadyQ.front().CPUburstT)//check if there are more than one process that has the same shortest CPUburstT
                        {
                            tempMin.push_back(it->processID);//it stores process IDs
                        }
                    }
                        /*for(unsigned int i=0;i<tempProReadyQ.size();i++)//to collect the processes ID that has the same shortest CPUburstT
                        {
                        if(tempProReadyQ[i].CPUburstT==tempProReadyQ[0].CPUburstT)//check if there are more than one process that has the same shortest CPUburstT
                        {
                            tempMin.push_back(tempProReadyQ[i].processID);//it stores process IDs
                        }
                        }*/

                    if(!tempMin.empty())//there are processes with the same CPUburstT
                    {
                        sort(tempMin.begin(),tempMin.end(),findMin);//find the smallest process ID
                        tempPro[tempMin[0]-1].CPUburstT--;
                        countN[tempMin[0]-1]++;
                        if(countN[tempMin[0]-1]==1)
                        {
                           actualArrivalT[tempMin[0]-1]=CPUrunT;//only needs the 1st actual arrival time for each process
                        }
                        if(tempPro[tempMin[0]-1].CPUburstT==0)
                        {
                           finishT[tempMin[0]-1]=CPUrunT+1;
                           numProDone++;
                        }
                        CPUrunT++;
                    }
                    else if(tempMin.empty())//no processes with the same CPUburstT
                    {
                        tempPro[(tempProReadyQ.front().processID)-1].CPUburstT--;//list
                        countN[(tempProReadyQ.front().processID)-1]++;
                        if(countN[(tempProReadyQ.front().processID)-1]==1)
                        {
                          actualArrivalT[(tempProReadyQ.front().processID)-1]=CPUrunT;//only needs the 1st actual arrival time for each process
                        }
                        if(tempPro[(tempProReadyQ.front().processID)-1].CPUburstT==0)
                        {
                            finishT[(tempProReadyQ.front().processID)-1]=CPUrunT+1;
                            numProDone++;
                        }
                        /*
                        tempPro[tempProReadyQ[0].processID-1].CPUburstT--;
                        countN[tempProReadyQ[0].processID-1]++;
                        if(countN[tempProReadyQ[0].processID-1]==1)
                        {
                          actualArrivalT[tempProReadyQ[0].processID-1]=CPUrunT;//only needs the 1st actual arrival time for each process
                        }
                        if(tempPro[tempProReadyQ[0].processID-1].CPUburstT==0)
                        {
                            finishT[tempProReadyQ[0].processID-1]=CPUrunT+1;
                            numProDone++;
                        }*/
                        CPUrunT++;
                    }
                    tempMin.clear();
                    }
                }
                tempProReadyQ.clear();//used it when it vector
                //while(!tempProReadyQ.empty())
                //{
                //    tempProReadyQ.pop_back();
                //}
                //tempProReadyQ.shrink_to_fit();
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




