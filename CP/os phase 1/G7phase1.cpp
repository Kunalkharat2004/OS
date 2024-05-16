#include<iostream>  //input-output stream operation
#include<fstream>   //file input output stream
using namespace std; //access to all standard library functions of namespace

//creating class Os 
class OS{
    //encapsulate the essential memebers and register initialization
    private:
        char M[100][4];     //Physical Memory (100 words *4 =400 bytes)
        char IR[4];         //Instruction Register (4 bytes)
        char R[4];          //General Purpose Register (4 bytes)
        int IC;             //Instruction Counter Register (2 bytes)
        int SI;             //System interrupt
        bool C;             //Toggle (1 byte) (boolean)
        char buffer[40];    //buffer (10*4=40 bytes)  
    
    //functions declaration in public to access outside
    public:
        void init(); //initialize the memory
        void LOAD(); //load the instructions to memory
        void Execute(); //execute the loaded instructions
        void MOS();  //perform the master mode operations for interrupt handling

        fstream infile;  //input file
        fstream outfile; //output file
};

//initiallise memory using init() function
//flush off everything  to 0
void OS::init()
{  
    //initialize main physical memory to 0
    for(int i = 0; i<100; i++)
    {    
        for(int j = 0; j<4; j++ )
        {  
            M[i][j]=' ';   
        }    
    }      
    //initialize instruction regsiter and register to 0 and toggle to false state
    IR[0] = ' ';
    R[0] = ' ';
    C = false;
}

//Master Mode operations (central control unit of os for interrupt handling ,)
void OS::MOS()
{
   
    //if SI-- System interrupt 
    //if SI=1 -> read mode , SI=2 -> write  mode , SI=3 ->termination mode 
    if(SI == 1)     
    {   
        //clearing buffer
        for(int i=0;i<=39;i++)
           buffer[i]='\0';
        //reading buffer 
        infile.getline(buffer,40);
        
        //read the next card from input file 
        int k = 0;
        int i = IR[2]-48; //converting character to numeric using 0's ASCII value 48
        i = i*10;
        
        //copy buffer -->memory in  read mode
        for( int l=0 ; l<10 ;  ++l)
        {   
            for(int j = 0 ; j<4; ++j)
            {
                M[i][j] = buffer[k];
                k++;                
            }
            //exit if buffer termination is reached
            if(k == 40)
            {
                break;               
            }
            i++; //incrementing by 10 to read next content
        }
    
        for(int i = 0; i<100; i++)
        { 
            cout<<"M["<<i<<"]\t";
            for(int j = 0; j<4; j++ )
            {  
                cout<<M[i][j];
            }
            cout<<endl;
        }  
    }
    //if SI==2, then write mode
    else if(SI == 2)        
    { 
      //clearing buffer
      for(int i=0;i<=39;i++)
           buffer[i]='\0';
        
        int k = 0;
        int i = IR[2]-48;
        i = i*10;
        
        //for write memeory-->buffer-->display on output
        for( int l=0;l<10;++l)
        {   
            for(int j = 0 ; j<4; ++j)
            {
                buffer[k]=M[i][j]; //assign content of memory to buffer
                outfile<<buffer[k]; //output is displayed as content of buffer
                k++;                
            }
            if(k == 40)
            {
                break;                
            }
            i++;
        }
        for(int i = 0; i<100; i++)
        { 
            cout<<"M["<<i<<"]\t";
            for(int j = 0; j<4; j++ )
            {  
                cout<<M[i][j];
            }
            cout<<endl;
        }  
        outfile<<"\n";
    }
    //if SI=3 --> termination state
    else if(SI == 3)      
    {   
        outfile<<"\n";
        outfile<<"\n";
    }
}

//Execute function
void OS::Execute()
{   
    //assign M[IC]=IR and increment IC
    while(true)
    {   
        //load in instruction register
        for(int i = 0; i<4; i++)        
        {
            IR[i] = M[IC][i];
        }
        IC++;
        
        //if GD, then,  SI=1(read mode), call mos
        if(IR[0] == 'G' && IR[1] == 'D')    
        {
            SI = 1;
            MOS();
        }
        //if PD,then, SI=2 (write mode), call mos
        else if(IR[0] == 'P' && IR[1] == 'D')      
        {
            SI = 2;
            MOS();
        }
        //if H--halt (stop the execution) (SI=3, termination mode)
        else if(IR [0] == 'H')     
        {
            SI = 3;
            MOS();
            break;
        }
        //if LR, then load register to content of R
        else if(IR[0] == 'L' && IR[1] == 'R')       
        {
            int i = IR[2]-48;
            i = i*10 + ( IR[3]-48);
            for(int j=0;j<=3;j++)  
            {
                R[j]=M[i][j];
            }
            cout<<endl; 
        }
        //if SP, then store the content of R to memory
        else if(IR[0] == 'S' && IR[1] == 'R')       
        {
            int i = IR[2]-48;
            i = i*10 +( IR[3]-48);
            for(int j=0;j<=3;j++)
            {
                M[i][j]=R[j];
            }
            cout<<endl; 
        }
        //compare the content of R with memory(IR), if equal set toggel T to true
        else if(IR[0] == 'C' && IR[1] == 'R')       
        {
            int i = IR[2]-48;
            i = i*10 + (IR[3] - 48);
            int count=0;
            for(int j=0;j<=3;j++)
            {
              if(M[i][j] == R[j])
              {
                count++;
              }
            }     
            if(count==4)
            {
                C=true;
            }
        }
        //if BT and toggle is true then load the content of instruction register to IC
        else if(IR[0] == 'B' && IR[1] == 'T')      
        {
            if(C == true)
            {
                int i = IR[2]-48;
                i = i*10 + (IR[3] - 48);
                IC = i;
            }
        }
    }
}

//Load Function 
void OS::LOAD()
{
    //reading the data
    cout<<"Reading Data..."<<endl;
    int x=0; //initially initialize memory index counter to 0
    do
    {   
        //claring the buffer, before it is being used using loop
        for(int i=0;i<=39;i++)   
        { 
           buffer[i]='\0';
        }
        infile.getline(buffer,40);
        //initializing buffer with programme/control cards..
        for(int k=0;k<=39;k++) 
        {
           cout<<buffer[k];
        }         
        //if control card-->
        //if $AMJ. then go to init , reinitialize memory and start fresh execution
        if(buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        { 
            init();
        }
        //if $DTA then set instruction counter register to 00 and call execute function to execute the data
        else if(buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            IC = 00;
            Execute();
        }
        //if $END then stop execution (end of Job), reinitialize memory loc counter to 0 and move tto next instruction
        else if(buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
        {  
            x=0;
            continue;
        }
        //else programme programme card--->
        // copy the programme card into the buffer and load into the memory
        //if buffer reaches terminal state (40th location, then stop the process)
        else
        {
            int k = 0;
            
            for(;x <100; ++x)
            {   
                for(int j = 0 ; j<4; ++j)
                {
                    
                    M[x][j] = buffer[k];
                    k++;
                }
                
                if(k == 40 || buffer[k] == ' ' || buffer[k] == '\n')
                {
                    break;
                } 
            }       
        }
    }while(!infile.eof());  // continue load function in loop until end of file is reached   
}

//main function
int main()
{ 
    OS os;  //creating the instance of object of Os class

    os.infile.open("input.txt", ios::in);   //input file
    os.outfile.open("output.txt", ios::out);  //output file

    //if not able to read input file, then show  failure
    if(!os.infile)
    {
        cout<<"Failure"<<endl;
    }
    //else print Success and call the load function
    else
    {
        cout<<"Success"<<endl;
    }
      
    //call the load function to load instruction into memeory and start the execution of programme
    os.LOAD();

    return 0;
}