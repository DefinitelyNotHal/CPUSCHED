#include <iostream>
#include <fstream>
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
    cout <<"menu";
    cout <<"\n1. FIFO";
    cout <<"\n2. SJF with preemption";
    cout <<"\n3. RR(with specified time quantum)";
    cout <<"\n4. Priority with preemption";
    cout <<"\n5. Exit\n";

    cin >> choice;
    return choice;

};

///find total elapsed time(for the scheduler)
void ElapsedT()
{

}

///find throughput
void throughput()
{
    ///Throughput=no of process/(max of completion time-time at which first process is brought to CPU)
}

///find CPU utilization
void CPUutilization()
{
    ///CPU utilization=
}

///find waiting time
void waitingT()
{
    ///waitingT=finishT-arrivalT-burstT
}

///find turnaround time
void turnaroundT()
{
    ///turnaroundT=finishT-arrivalT
}

///find response time
void responseT()
{
    ///responseT=1stResponseT-arrivalT
}

///print statistics of the run to the screen
void printStatistics()
{
    cout<<"Statistics for the Run\n\n"<<endl;
    cout<<"Number of processes: 10,000"<<endl;
    /*cout<<"Total elapsed time(for the scheduler): "<<totalElpsedT<<endl;
    cout<<"Throughput: "<<Throughput<<endl;
    cout<<"CPU utilization: "<<CPUuti<<endl;
    cout<<"Average waiting time: "<<avgWaitT<<endl;
    cout<<"Average turnaround time: "<<avgTurnaT<<endl;
    cout<<"Average response time: "<<avgResT<<endl;*/
}

int main()
{
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
     cout<<pro.arrivalT;
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
        }
        else if(menuChoice=='2'){
            ///Shortest Job First with preemption, mainly depends on CPU burst time

        }
        else if(menuChoice=='3'){
            ///Round Robin(with specific time quantum), mainly depends on the time quantum
            int numQuantum=0;
            cout<<"Please enter time quantum: "<<endl;
            cin>>numQuantum;
        }
        else if(menuChoice=='4'){
            ///Priority with preemption, mainly depends on priority, smaller number means higher priority
        }
        else if(menuChoice=='5'){
            cout<<"Program is terminated."<<endl;
            return 0;
        }
        menuChoice = menu();
    }///end of while loop
    return 0;
}




