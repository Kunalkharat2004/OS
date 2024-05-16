#include<stdio.h>
#include<stdlib.h>
#include<iostream>  //stream input-output
#include<fstream>  //file input-output
#include<string.h>

using namespace std;
int IC,PTR,VA,RA,kio=-1,flag[30],PI,TI,SI,TTC=0,LLC=0;  // Instruction counter , page table Register , Virtual Address , Real address , flag for memory allocation , PI - programme interrupt , TI - timer interrupt                                                      
char M[300][4],IR[4],R[4],C; //Memory, Instruction Register , General purpose register and Toggle
char page[10][4];  // process page
fstream fin,fout;
char temp[100];
string line;
void MOS();
int actualDataCardCount = 0; // Define and initialize the variable to store the actual count of data cards
//Allocate process control Block (PCB) with Job id, time count and line count
struct PCB
{
    char job[4],TTL[4],TLL[4];
}pcb;

int ttl,tll;

//End programme with pri ting interrup status and time and line counnt
void endprogram()
{
    fout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
    fout<<"TTC = "<<TTC<<"  LLC="<<LLC<<endl;
    cout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
    cout<<"TTC = "<<TTC<<"  LLC="<<LLC<<endl;
    exit(1);
}

// Allocate the Memory for program Instructions
void allocate()
{
    int pos, i, j, k = 0, check = 0, len;
    char str[2];
    int level = 0;

    // Continues until not completed (check)
    while (check != 1)
    {
        cout << "TEMP  " << kio << endl;
        kio++;

        cout << "Cnt1  " << level << endl;
        pos = (rand() % 29) * 10;  // Generate a random position within the memory range
        cout << "Pos1   " << pos << endl;
        while (flag[pos / 10] != 0)
        {
            cout << "In while pos" << endl;
            pos = (rand() % 29) * 10;  // If the position is occupied, find a new empty block
            cout << "Pos2  " << pos << endl;
            cout << "Cnt2  " << level << endl;
        }

        // Once an empty block is found, mark it as occupied
        flag[pos / 10] = 1;
        itoa(pos, str, 10);

        // Load the memory instruction and address into the block
        if (pos / 100 == 0)
        {
            M[PTR + kio][2] = '0';
            M[PTR + kio][3] = str[0];
        }
        else
        {
            M[PTR + kio][2] = str[0];
            M[PTR + kio][3] = str[1];
            cout << "Cnt3  " << level << endl;
        }

        // Read and execute from input file until Halt "H" is encountered, terminating allocation
        getline(fin, line);
        cout << line << "Line2" << endl;
        level++;
        k = 0;
        for (i = 0; i < line.size() / 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                M[pos + i][j] = line[k];
                k++;
                if (line[k] == 'H')
                {
                    check = 1;  // Set check flag to terminate allocation
                    M[pos + (i + 1)][0] = 'H';
                    M[pos + (i + 1)][1] = '0';
                    M[pos + (i + 1)][2] = '0';
                    M[pos + (i + 1)][3] = '0';
                }
            }
        }
    }

    // Display the allocated memory
    cout << endl
         << "MEMORY" << endl;
    for (i = 0; i < 300; i++)
    {
        cout << "[" << i << "] = ";
        for (j = 0; j < 4; j++)
        {
            cout << M[i][j];
        }
        cout << endl;
    }
}



// convert or map virtual memory address into physical memory addresss
void AddMap()
{
    int add, pos;
    char str[2];
    // real address = base address (page table register) + virtual address/10
    RA = PTR + (VA / 10);
    
    // If virtual address is not within the valid range (0 to 99), set PI to 2 (operand error)
    if (VA < 0 || VA > 99)
    {
        PI = 2;
        MOS();
        return; // Exit function immediately
    }
    
    // Whenever third element is #, it indicates page fault, new location (unoccupied) is allocated to memory block
    if (M[RA][3] == '#')
    {
        cout << "**** Page fault occurred ****\n";
        pos = (rand() % 29) * 10;
        cout << "POS1   " << pos << endl;
        while (flag[pos / 10] != 0)
        {
            cout << "In while pos" << endl;
            pos = (rand() % 29) * 10;
            cout << "POS2  " << pos << endl;
        }
        flag[pos / 10] = 1;
        itoa(pos, str, 10);

        if (pos / 100 == 0)
        {
            M[RA][2] = '0';
            M[RA][3] = str[0];
        }
        else
        {
            M[RA][2] = str[0];
        }

        PI = 3;
    }
    // Check for operand error if RA is not within the valid range (0 to 99)
    if (RA > PTR + 10 || RA < PTR)
    {
        PI = 2;
        MOS();
    }
}


//reading the output for RA and incrementing RA=RA+9
void read()
{
    int no,i,j,k,z,x;

    getline(fin,line);
    cout<<"\nLine  :"<<line;
    no=((M[RA][2]-48)*10)+(M[RA][3]-48);
    no=no*10;
    k=0;
    for(i=0; k<=line.size()  ;i++)
    {
        for(j=0;j<4 && k<=line.size();j++)
        {
            cout<<"\n Count  :";
            cout<<no+i<<endl<<j<<"\n";
            M[no+i][j] = line[k];
            k++;
        }
    }

    for(i=0;i<300;i++)
    {
        cout<<"["<<i<<"] = ";
        for(j=0;j<4;j++)
        {
            cout<<M[i][j];

        }
        cout<<endl;
    }
}

void write()
{

    char buff[40];
    cout<<"\n In Write : \n";
    int no=0,i,j,k,z,x;
    no=((M[RA][2]-48)*10)+(M[RA][3]-48);
    no=no*10;
    k=0;
    //whenever '_' encounters move to the next line of  M
    //end the buffer with '/0' character
    while(1)
    {
        for(i=0; i<4 ;i++)
        {
            if(M[no][i] == '_')
                break;
            buff[k] = M[no][i];
            k++;
        }
        if(M[no][i] == '_')
                break;
        no++;
    }
    buff[k]='\0';
    cout<<"\n Line is : "<<buff<<endl;
    fout.write(buff,strlen(buff));
    // after writting print the message that porgram terminated normally.......
    cout<<"program terminated normally";
    fout<<endl<<"-----------------------------------------------------------------------------------"<<endl;
    fout<<endl<<"program terminated normally"<<endl;
}

//Master mode operation
// PI (1) = opcode error , PI (2) = operand error , PI (3) = Page Fault
// TI = 0 (no error) , TI=2 (time limit exceeded) , TI =1 (for SI=2) (line limit exceeded)
void MOS()
{
    if(PI==1)
    {
        cout<<"\n**** Opcode Error : ****\n**** Program terminated abnormally. ****\n\n";
        fout<<"\n**** Opcode error : ****\n**** Program terminated abnormally. ****";
        endprogram();
    }
    else if(PI==2)
    {
        cout<<"\n**** Operand Error : ****\n**** Program terminated abnormally. ****\n\n";
        fout<<"\n**** Operand error : ****\n**** Program terminated abnormally. ****";
        endprogram();
    }

    if(SI==3)
        endprogram();
    else if(SI==1)
    {
        if(TI==0)
            read();
        else if(TI==2)
        {
            cout<<"\n**** Time Limit Exceeded : ****\n**** Program terminated abnormally. ****";
            fout<<"\n**** Time Limit Exceeded : ****\n**** Program terminated abnormally. ****";
            endprogram();
        }
    }
    else if(SI==2)
    {
        cout<<"\nIN SI=2";
        if(TI==0)
        {
            cout<<"\nIN TI=0";
            write();
        }
        else if(TI==2)
        {
            cout<<"\nIN TI=2";
            write();
            cout<<"\n**** Time Limit Exceeded : ****\n**** Program terminated abnormally. ****";
            fout<<"\n**** Time Limit Exceeded : ****\n**** Program terminated abnormally. ****";
            endprogram();
        }
        else if(TI==1)
        {
            cout<<"\n**** Line Limit Exceeded : ****\n**** Program terminated abnormally. ****";
            fout<<"\n**** Line Limit Exceeded : ****\n**** Program terminated abnormally. ****";
            endprogram();
        }
    }
}



void examine()
{
    char ch;
    ch=IR[0]; //initial input character with first location of instruction Register (IR)
    PI=0;

    switch(ch)

    {
    case 'G':
        cout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
        cout<<"TTC = "<<TTC<<"  LLC="<<LLC<<endl;

        if(IR[1] !='D')
        {
            PI=1; //if not GD ,like GX then operation code error and call MOs
            MOS();
        }

        else
        {
            TTC = TTC+2;
            if(TTC <= ttl)
            {
                SI=1; // read as no time limit exceeded
                MOS();
            }
            else
            {
                TI=2; // time limit exceeded
                MOS();
            }
        }
        cout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
        cout<<"TTC = "<<TTC<<"  LLC="<<LLC<<endl;
        break;

    case 'P':
    SI = 2; // Write operation
    cout << "\nSI = " << SI << "  TI =" << TI << "  PI=" << PI << endl;
    cout << "TTC = " << TTC << "  LLC=" << LLC << endl;
    if (IR[1] != 'D')
    {
        PI = 1; // Opcode error for invalid instructions
        MOS();
    }
    else
    {
        LLC = LLC + 1; // Increment line limit counter
        TTC = TTC + 1;
        cout << LLC << " :llc" << endl;
        cout << TTC << " :ttc" << endl;
        if (LLC <= tll) // Check if line limit exceeded TLL
        {
            TI = 0; // No line limit exceeded
            MOS();
        }
        else
        {
            TI = 1; // Line limit exceeded
            MOS();
        }
    }
    cout << "\nTTC = " << TTC << "  LLC=" << LLC << endl;
    cout << "SI = " << SI << "  TI =" << TI << "  PI=" << PI << endl;
    break;


    case 'H':

        SI=3;  //termination state
        MOS();
        break;
    }
}

void executeProgram() {
    int no;
    char a[3]; // Declare a character array of size 3 for the page table
    int i, j, k;

    // Count the number of data cards present in the job
    int dataCardCount = 0;
    for (i = 0; i < kio; i++) {
        if (M[PTR + i][0] == '$' && M[PTR + i][1] == 'D' && M[PTR + i][2] == 'T' && M[PTR + i][3] == 'A') {
            dataCardCount++;
        }
    }

    // Check for Out of Data error
    if (IC > dataCardCount) {
        cout << "Out of data error: Number of GD instructions exceeds actual data cards" << endl;
        endprogram();
        return; // Exit function immediately
    }

    // Loop through each instruction
    for (IC = 0; IC <= kio; IC++) {
        // Get the page table entry from memory
        a[0] = M[PTR + IC][2];
        a[1] = M[PTR + IC][3];
        a[2] = '\0';
        no = ((a[0] - 48) * 10) + (a[1] - 48); // Convert from character to integer

        // Loop through each instruction in the page
        for (j = 0; j < 10; j++) {
            // Load the instruction into the instruction register
            for (k = 0; k < 4; k++) {
                IR[k] = M[(no * 10) + j][k];
            }

            // Extract the virtual address from the instruction
            VA = ((IR[2] - 48) * 10) + (IR[3] - 48); // Virtual address

            // Map virtual address to physical address
            AddMap();

            // Increment timer count after execution of instruction
            TTC++;

            // Check for time limit exceeded
            if (TTC > ttl) {
                TI = 2; // Time limit exceeded
                MOS();
                return; // Exit function immediately
            }

            // Examine the instruction
            examine();

            // Check for Out of Data error
            if (IC > dataCardCount) {
                cout << "Out of data error: Number of GD instructions exceeds actual data cards" << endl;
                endprogram();
                return; // Exit function immediately
            }
        }
    }
}

void startExecution()
{
    IC=0;   // initialize IC to 0 and start execution
    executeProgram();
}

//init function initializes the data from the block into the memory
void init()
{
    int i,j;
    PTR=(rand()%29)*10;
    cout<<"PTR   "<<PTR<<endl;
    for(i=0;i<30;i++)
    {
        flag[i]=0;
    }
    flag[PTR/10]=1;  //check whether he randomly choosesn block is occupied or not
    for(i=0;i<300;i++)
    {
        for(j=0;j<4;j++)
        {
            M[i][j]='_';
        }
    }
    for(i=PTR;i<PTR+10;i++)  // load content of block in memory
    {
        for(j=0;j<4;j++)
        {
            M[i][j]='#';
        }
    }

}


void load() {
    printf("IN LOAD\n");
    int i, j, k;

    while (fin) {
        cout << "In while" << endl;

        getline(fin, line);
        cout << line << "line1" << endl;
        if (line[0] == '$' && line[1] == 'A' && line[2] == 'M' && line[3] == 'J') {
            // Handle $AMJ control card
            // Increment the actualDataCardCount for each data card encountered
            actualDataCardCount = 0; // Reset the count for each job
            SI = 0;
            TI = 0;
            TTC = 0;
            LLC = 0;
            for (i = 4, j = 0; i < 8; i++, j++) {
                pcb.job[j] = line[i];
            }
            for (i = 8, j = 0; i < 12; i++, j++) {
                pcb.TTL[j] = line[i];
            }
            for (i = 12, j = 0; i < 16; i++, j++) {
                pcb.TLL[j] = line[i];
            }
            ttl = (pcb.TTL[0] - 48) * 1000 + (pcb.TTL[1] - 48) * 100 + (pcb.TTL[2] - 48) * 10 + (pcb.TTL[3] - 48);
            tll = (pcb.TLL[0] - 48) * 1000 + (pcb.TLL[1] - 48) * 100 + (pcb.TLL[2] - 48) * 10 + (pcb.TLL[3] - 48);
            cout << " TTL = " << ttl << endl;
            cout << " TLL = " << tll << endl;
            TTC = 0;
            LLC = 0;
            init();
            allocate();
        }
        if (line[0] == '$' && line[1] == 'D' && line[2] == 'T' && line[3] == 'A') {
            // Handle $DTA control card
            // Increment the actualDataCardCount for each data card encountered
            actualDataCardCount++;
            startExecution();
        }
    }
}
int main()
{
    fin.open("input.txt");
    fout.open("output.txt");
    load();
    fin.close();
    fout.close();
    return 0;
}


// $AMJ000100060001
// GD10PD10H
// $DTA
// HELLO WORLD!
// $END0002


// **** Operand error : ****
// **** Program terminated abnormally. ****
// SI = 0  TI =0  PI=2
// TTC = 0  LLC=0
