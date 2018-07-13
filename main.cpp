#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>
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
float avgWaitingT(int ft[], process pro[])
{
    int wt[10000];
    float avgwait=0.0;
    ///waitingT=finishT-arrivalT-burstT, and sum of all waitingT
    int sumwait=0;
    for (int i=0;i<10000;i++)
    {
        wt[i]=ft[i]-pro[i].arrivalT-pro[i].CPUburstT;
        sumwait=sumwait+wt[i];
    }
    ///avg. WaitingT=sum of all waitingT/total # of processes
    avgwait=sumwait/10000.0;
    return avgwait;
}

///find turnaround time
float avgTurnaroundT(int ft[],process pro[])
{
    int tat[10000];
    float avgtat=0.0;
    ///turnaroundT=finishT-arrivalT, and sum of all turnaroundT
    int sumtat=0;
    for (int i=0;i<10000;i++)
    {
        tat[i]=ft[i]-pro[i].arrivalT;
        sumtat=sumtat+tat[i];
    }
    ///avg. TurnaroundT=sum of all turnaroundT/ total # of processes
    avgtat=sumtat/10000.0;
    return avgtat;
}

///find response time
float avgResponseT(int rt1st[], process pro[])
{
    int rt[10000];
    float avgrest=0.0;
    ///responseT=the actual arrival time in CPU exec. - arrivalT, and sum of all responseT
    int sumrest=0;
    for (int i=0;i<10000;i++)
    {
        rt[i]=rt1st[i]-pro[i].arrivalT;
        sumrest=sumrest+rt[i];
    }
    ///avg. ResponseT=sum of all responseT/total # of processes
    avgrest=sumrest/10000.0;
    return avgrest;
}

///print statistics of the run to the screen
void printStatistics(int elapT, float thput, float CPUutil, float avgWaitT, float avgTurnaT, float avgRespT)
{
    cout<<"Statistics for the Run\n"<<endl;
    cout<<"Number of processes: 10,000"<<endl;
    cout<<"Total elapsed time(for the scheduler): "<<elapT<<endl;
    cout<<"Throughput: "<<thput<<endl;
    cout<<"CPU utilization: "<<CPUutil<<"%"<<endl;
    cout<<"Average waiting time: "<<avgWaitT<<endl;
    cout<<"Average turnaround time: "<<avgTurnaT<<endl;
    cout<<"Average response time: "<<avgRespT<<endl;
    cout<<endl;
}

///used for sorting the process by its CPU burst time
bool checkbt(process a,process b)
{
	return a.CPUburstT<b.CPUburstT;

}

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
     process pro[10000];
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
     vector <int> idleT;
     vector <int> tempProcessID;
     vector <int> tempArrivalT;
     vector <int> tempCPUburstT;
     vector <int> tempPriority;

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
            int finishT[10000];
            int actualArrivalT[10000];//use for finding the response time
            idleT.clear();
            idleT.push_back(pro[0].arrivalT);
            finishT[0]=pro[0].arrivalT+pro[0].CPUburstT;
            actualArrivalT[0]=pro[0].arrivalT;
            for (int i=1; i<10000;i++)
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

            ///calculate throughput
            ///Throughput= # of process/(final completion time-time at which first process is brought to CPU)
            throuPut=10000.0/(finishT[9999]-pro[0].arrivalT);
            ///calculate sum of CPU idle time
            int sumIdleT=0;
            for(unsigned int i=0;i<idleT.size();i++)
            {
                sumIdleT=sumIdleT+idleT[i];
            }
            ///total elapsed time(for the scheduler)=final completion time- total CPU idle time
            elapsedT=finishT[9999]-sumIdleT;
            ///calculate CPU utilization
            ///CPU utilization=(final completion time-the sum of CPU idle time)/final completion time
            CPUutilize=float(finishT[9999]-sumIdleT)/finishT[9999]*100;
            ///calculate avg. waiting time
            avgWaitingTime=avgWaitingT(finishT,pro);
            ///calculate avg. turnaround time
            avgTurnaroundTime=avgTurnaroundT(finishT,pro);
            ///calculate avg. response time
            avgResponseTime=avgResponseT(actualArrivalT, pro);
            ///print to screen
            printStatistics(elapsedT,throuPut,CPUutilize,avgWaitingTime,avgTurnaroundTime,avgResponseTime);
        }
        else if(menuChoice=='2'){
            ///Shortest Job First with preemption, mainly depends on CPU burst time
            int finishT[10000];
            int actualArrivalT[10000];
            int remainingT[10000];
            idleT.clear();
            tempArrivalT.clear();
            tempCPUburstT.clear();
            tempPriority.clear();
            tempProcessID.clear();
            int countN=0;
            for(int i=1;i<10000;i++)//select the first arrival time process for the calculation of CPU utilization and Throughput
            {
                if ((pro[i].arrivalT==pro[0].arrivalT) && (pro[i].CPUburstT<pro[0].CPUburstT))//checking if there is any process has the same arrival time as the first arrival, if so, get the SJF.
                {
                    tempProcessID.push_back(pro[i].processID);//stores the processID to tempProcessID

                }
                else
                {
                    countN++;
                    if (countN==1)
                    {
                        idleT.push_back(pro[0].arrivalT);
                        actualArrivalT[0]=pro[0].arrivalT;
                    }

                }
            }
            ///sort(pro, pro+10000, checkbt);
            int CPUrunT=0;
            int numProDone=0;
            while(numProDone<100)//checking if all the processes have done, if not, increment by 1.
            {
                for(unsigned int i=0;i<10000;i++)
                {
                    if (pro[i].arrivalT<=CPUrunT)
                    {
                        idleT.push_back(pro[i].CPUburstT);
                    }
                }
                numProDone++;
                CPUrunT++;//anything before this count from 0, anything else below this count from 1
            }
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




