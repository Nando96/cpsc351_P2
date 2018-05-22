#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "info.h"
#include "block.h"

using namespace std;

int available(vector<block> MM, int size)
{
  int temp=0,val = -1;
  for (int i = 0; i < MM.size(); ++i)
  {
    if (MM[i].Free)
      temp++;
    
    else
      temp = 0;

    if (temp == size)
    {
      val = (i + 1 - size);
    }
  }

  return val;
}
void MMdisplay(vector<block> &MM, int memSize, int pageSize)
{
  int num=0, pageNum = 0,processNum = 0,start = 0,end = 0;
	cout << "\tMemory Map: \n";
  

  while (num < (memSize / pageSize))
  {
    if (!MM[num].Free)
    {
      if (start != 0)
      {
        end = num - 1;
        cout << " \t" << start * pageSize << "-" << ((end + 1) * pageSize) - 1 << ": Free frame(s)\n";
        start = 0;
      }

      cout << "            " << MM[num].start << "-" << MM[num].end << ": Process "
         << MM[num].proNum << " , Page " << MM[num].pagNum << "\n";
    }
    else if (start == 0)
    {
      start = num;
    }

    num++;
  }

  if (start != 0)
  {
    end = num - 1;
    cout << "\t\t\t" << start * pageSize << "-" << ((end + 1) * pageSize) - 1 << ": Free frame(s)\n";
  }
}

bool add(vector<block> &MM, int pageSize, info &input)
{
  double Size;
  int intBSize;
  int startBlock = -1;
  bool wasAdded;
  int numPage = 1;

  //Loop
  for (int i = 0; i < input.block; ++i)
  {
    Size = input.size[i] / pageSize;

    for (int j = i; j < input.block; ++j)
    {
      Size += (input.size[j] / pageSize);
    }

    intBSize = (int) Size;
   startBlock = available(MM, intBSize);

    if (startBlock == -1)
    {
      intBSize = (int) Size;
	int num = 0;
	int space=-1;

  for (int i = 0; i < MM.size(); ++i)
  {
    if (MM[i].Free)
      num++;
    
    else
      num = 0;
    
    if (num == intBSize)
    {
      space = (i + 1 - intBSize);
    }
  }
    }
    wasAdded = false;

    if (startBlock > -1)
    {
      wasAdded = true;

   
      for (int j = startBlock; j < (startBlock + ((input.size[i] + 99) / pageSize)); ++j)
      {
        MM[j].proNum = input.number;
        MM[j].pagNum = numPage;
        MM[j].Free = false;
        numPage++;
      }
    }
  }

  return wasAdded;
}
void output(vector<info> &list, vector<int> order, int memSize, int pageSize)
{
  int temp2;
  info temp;
  vector<block> MM;
  vector<int> que;
  block Btmp;
	
  //initialize MM
  for (int i = 0; i < (memSize / pageSize); ++i)
  {
    Btmp.start = i * pageSize;
    Btmp.end = ((i + 1) * pageSize) - 1;
    Btmp.proNum = Btmp.pagNum = -1;
    Btmp.Free = true;
    MM.push_back(Btmp);
  }

  //loop
  //match display of output
  for (int i = 0; i < order.size(); ++i)
  {
    //output t
    cout << "t=" << order[i] << ": ";

    for (int j = 0; j < list.size(); ++j)
    {
      if (list[j].start == order[i])
      {
        que.push_back(list[j].number);
        cout << "processe " << list[j].number << " arrive\n";
        cout << "\t\tinput Queue: [ ";

        for (int k = 0; k < que.size(); ++k)
            cout << que[k] << " ";

        cout << "]\n";
      }
      else if (list[j].start + list[j].end == order[i])
      {
        cout << "process " << list[j].number << " complete\n";
	for (int i = 0; i < MM.size(); ++i)
  {
    if (MM[i].proNum == list[j].number)
    {
      MM[i].Free = true;
    }
  }
	//disp Mem Map
        MMdisplay(MM, memSize, pageSize);
      }
    }

    //Add  to Mem Map
    while (que.size() != 0)
    {
    
      temp = list[que.front() - 1];

      // Add 
	bool test = add(MM, pageSize, temp);
      if (test == true){
        cout << "\tMM moves Process " << temp.number << " to memory\n";
        que.erase(que.begin());
        cout << "\t\tInput Queue: [ ";
        
         for (int k = 0; k < que.size(); ++k)
            cout << que[i] << " ";

        cout << "]\n";

        // Display Memory Map...
        MMdisplay(MM, memSize, pageSize);
      }
      else
      {
        cout << "\nFULL!!\n";
       break;
      }
    }
  }
}

int main()
{
  
  int memSize = 2000;
  int pageSize= 0;
    cout<< "Page Size(1:100, 2:200, 3:400) > ";
    cin>> pageSize;
    if(pageSize == 1){
      pageSize = 100;
    }
    else if(pageSize == 2){
      pageSize = 200;
    }
    else if(pageSize == 3){
      pageSize = 400;
    }
    else{
      cout<<"incorrect input: "<<pageSize<<" output will be incorrect\n";
    }
  //Vars
  vector<info> list;
  vector<int> order,tempBsize;
bool temp1;
bool temp2;
int block;
int num_processes;
info temp;

  //Read from file
  ifstream infile;
  infile.open("in1.txt");
  infile >> num_processes;
  for (int i = 0; i < num_processes; ++i)
  {
    infile >> temp.number;
    infile >> temp.start;
    infile >> temp.end;
    infile >> temp.block;

    // Store values read
    for (int j = 0; j < temp.block; ++j)
    {
      infile >> block;
      tempBsize.push_back(block);
    }
    //input vector into info size class obj
    temp.size = tempBsize;
    //make an obj vecor
    list.push_back(temp);
  }
  infile.close();


  // calc the order
  for (int i = 0; i < list.size(); ++i)
  {
    temp1 = temp2 = false;

    for (int j = 0; j < order.size(); ++j)
    {
      if (list[i].start == order[j])
        temp1 = true;

      if (list[i].start + list[i].end == order[j])
        temp2 = true;
      
    }

    if (temp1 == false)
      order.push_back(list[i].start);

    if (temp2 == false)
      order.push_back(list[i].start + list[i].end);
    
  }
  //sort the order using std::sort() func
  //output results
  output(list, order, memSize, pageSize);
  return 0;
}



