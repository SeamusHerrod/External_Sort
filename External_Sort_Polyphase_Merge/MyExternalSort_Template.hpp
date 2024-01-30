#ifndef _MY_EXTERNAL_SORT_
#define _MY_EXTERNAL_SORT_
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <assert.h>
#include <string>
#include <fstream>
#include <limits.h>

using namespace std;

// total number of tapes you have,
// note that you should leave one tape for output during merging
const int TOTAL_TAPES = 16;

// the number of elements that can be loaded in to the main memory each time
const int BATCH_SIZE = 32;

// a list of kth order Fibonacci numbers
static std::vector<size_t> fibo_series;

// custom comparators for priority queues
auto GREATER = [](const std::pair<size_t, size_t> &a, const std::pair<size_t, size_t> &b)
{
  return a.first > b.first;
};

auto LESSER = [](const std::pair<size_t, size_t> &a, const std::pair<size_t, size_t> &b)
{
  return a.first < b.first;
};

char a;

/*------------------------------------------------------------------------------
  Function: IsSorted
    Checks if the content in input_file is sorted

  Vairables:
    input_file - the file name that you want to check if it's sorted
    reverse    -  if set true, sort in descending order; otherwise in ascending order

  -----------------------------------------------------------------------------*/
bool IsSorted(const std::string &input_file, bool reverse = false)
{

  bool sorted = true;
  std::ifstream in(input_file.c_str());
  if (!in.is_open())
  {
    //std::cout << input_file << " doesn't exist!\n";
    return false;
  }
  else
  {
    std::string buffer;
    int prev = INT_MIN, curr;
    if (reverse)
      prev = INT_MAX;
    while (!in.eof())
    {
      in >> curr;
      if ((curr < prev && !reverse) || (curr > prev && reverse))
      {
        sorted = false;
        //std::cout << "Out of order: " << prev << ", " << curr << std::endl;
        break;
      }
      prev = curr;
    }
  }
  in.close();
  return sorted;
}

bool blockSorted(const std::vector<size_t> &block, bool reverse = false)
{
  bool sorted = true;
  int prev = INT_MIN, curr;
  if (reverse)
    prev = INT_MAX;
  for (int i = 0; i < block.size(); i++)
  {
    curr = block[i];
    if ((curr < prev && !reverse) || (curr > prev && reverse))
    {
      sorted = false;
      //std::cout << "Out of order: " << prev << ", " << curr << std::endl;
      break;
    }
    prev = curr;
  }
  return sorted;
}

/*------------------------------------------------------------------------------
  Function: ComputeKthFibonacci
    Compute K-th order Fibonacci number for computing number of blocks should store in each tape.

  Note: you may call this function as many times as needed.
------------------------------------------------------------------------------*/
void ComputeKthFibonacci(void)
{
  /* formula:
    F(k)(0, 1, ..., k-1) = 0, 0, ..., 1
    F(k)(n) = F(k)(n − 1) + F(k)(n − 2) + ··· + F(k)(n − k), n >= k

    when n == k:
      F(k)(k) = F(k)(k − 1) + F(k)(k − 2) + ··· + F(k)(k − k)
              = 1           + 0           + ... + 0 = 1
    when n > k:
      F(k)(n)     = F(k)(n − 1) + F(k)(n − 2) + ··· + F(k)(n − k)
      F(k)(n)     = F(k)(n − 1) + [F(k)(n − 2) + ··· + F(k)(n − k) + F(k)(n − k-1)] - F(k)(n − k-1)
      F(k)(n − 1) = F(k)(n − 2) + F(k)(n − 3) + ··· + F(k)(n − k) + F(k)(n − k-1)
      F(k)(n)     = 2*F(k)(n − 1) - F(k)(n − k-1)
  */
  int k = TOTAL_TAPES - 1;
  if (fibo_series.empty()) // initial fibo_series
  {
    for (int i = 0; i < k - 1; i++)
      fibo_series.push_back(0);
    fibo_series.push_back(1);
    fibo_series.push_back(1); // this is fibo_series[k]
  }

  for (int i = 0; i < 100; i++) // compute next 100 items for usage
  {
    int n = fibo_series.size();
    size_t new_item = 2 * fibo_series[n - 1] - fibo_series[n - k - 1];
    fibo_series.push_back(new_item);
  }
}

/*------------------------------------------------------------------------------
  Function: GetBlocksAt
    get the number of blocks you should write to at current pass.
  Vairables:
    pass      - current pass of PolyPhaseMergePass.
    blocks_at - where you want to store the block counts of eath tape to.

  Note:
  For the t-th input tape at pass n, it should hold blocks_at[t] blocks:
    blocks_at[t] = F(k)(n+k-2) + F(k)(n+k-3) + ... + F(k)(n+t-2), t = 1, 2, ..., k
------------------------------------------------------------------------------*/
void GetBlocksAt(int pass, std::vector<int> &blocks_at)
{
  if (pass < 1)
    return;
  int k = TOTAL_TAPES - 1;
  blocks_at = std::vector<int>(k, 0);
  for (int t = 0; t < k; t++)
  {
    for (int tmp_i = pass + t - 1; tmp_i <= pass + k - 2; tmp_i++)
    {
      if (tmp_i >= fibo_series.size())
        ComputeKthFibonacci(); // compute more fibo_series
      blocks_at[t] += fibo_series[tmp_i];
    }
  }
}

/*------------------------------------------------------------------------------
  The insertion sort algorithm.

  Variables:
   a              - the input array
   left and right - the left and end indexes of the range of the elements to be sorted, inclusive
   reverse        - if set true, sort in descending order. Default: false
  (Hint: You can reuse your implementation from Lab 01)
------------------------------------------------------------------------------*/
template <typename Comparable>
void InsertionSort(std::vector<Comparable> &a, int left, int right, bool reverse = false)
{
  /*------ CODE BEGINS ------*/
  for (int i = left; i <= right; i++)
  {
    Comparable tmp = std::move(a[i]);

    int j;
    if (reverse == false)
    {
      for (j = i; j > left && tmp < a[j - 1]; --j)
      {
        a[j] = std::move(a[j - 1]);
      }
    }
    else
    {
      for (j = i; j > left && tmp > a[j - 1]; --j)
      {
        a[j] = std::move(a[j - 1]);
      }
    }
    a[j] = std::move(tmp);
  }

  /*------ CODE ENDS ------*/
}

/*------------------------------------------------------------------------------
basic function for printing vectors of any comparable type
  a[0, 1, 2, 3, 4]
  Print(a)
    0, 1, 2, 3, 4
-------------------------------------------------------------------------------*/
template <typename Comparable>
void Print(std::vector<Comparable> &a)
{
  for (int i = 0; i < a.size(); i++)
  {
    if (i + 1 < a.size())
      std::cout << a[i] << ", ";
    else
      std::cout << a[i] << std::endl;
  }
}

/*----------------------------------------------------------------------------
function for finding the number of blocks on a tape - testing purposes
----------------------------------------------------------------------------*/
int numBlockOnTape(std::string tapeName)
{
  ifstream in(tapeName.c_str());
  std::string buffer = "";
  int numBlocks = 0;
  int numElems = 0;
  while (!in.eof())
  {
    getline(in, buffer);
    numElems++;
    if (numElems == BATCH_SIZE)
    {
      numBlocks++;
      numElems = 0;
    }
  }
  if (numElems != 0)
  {
    numBlocks++;
  }
  in.close();
  return numBlocks;
}

/*----------------------------------------------------------------------------
function to get the first block from a tape
also removes the first block from that tape
----------------------------------------------------------------------------*/
std::vector<size_t> getFirstBlock(std::string tapeName)
{
  ifstream in(tapeName.c_str());
  ofstream out("temp.txt");
  std::string buffer = "";
  std::vector<size_t> blockArr;
  for (int i = 0; i < BATCH_SIZE; i++)
  {
    if (!(in.is_open()))
    {
      std::cout << "ERROR" << std::endl;
    }
    getline(in, buffer);
    blockArr.push_back(size_t(atoi(buffer.c_str())));
  }
  while (in.peek() != EOF)
  {
    getline(in, buffer);
    out << buffer << endl;
  }
  in.close();
  out.close();

  // remove original file, rename temp file to original file name
  remove(tapeName.c_str());
  rename("temp.txt", tapeName.c_str());
  return blockArr;
}

/*
  function that counts the total number of elements in all of the tapes
*/
int countTotalElements(std::vector<std::string> &ext_arrays)
{
  int totalElements = 0;
  for (int i = 0; i < ext_arrays.size() - 1; i++)
  {
    ifstream in(ext_arrays[i].c_str());
    std::string buffer = "";
    if (!in.is_open())
    {
      std::cout << "ERROR" << std::endl;
    }
    else
    {
      while (!in.eof())
      {
        getline(in, buffer);
        if (buffer != "$" && buffer != "")
        {
          totalElements++;
        }
      }
    }
    in.close();
  }
  return totalElements;
}
int countElemsOnTape(string tapeName)
{
  int totalElements = 0;

  ifstream in(tapeName.c_str());
  std::string buffer = "";
  while (!in.eof())
  {
    getline(in, buffer);
    if (buffer != "$" && buffer != "")
      totalElements++;
  }
  in.close();
  return totalElements;
}

bool EmptyTapes(vector<fstream> &fileStreams, int outIndex)
{
  for (int i = 0; i < fileStreams.size(); i++)
  {
    if (i != outIndex)
    {
      if (fileStreams[i].eof())
      {
        //cout << i << " is empty" << endl;
        return true;
      }
    }
  }
  return false;
}

int numEmptyTapes(vector<fstream> &fileStreams, int outIndex)
{
  int result = 0;
  for (int i = 0; i < fileStreams.size(); i++)
  {
    if (i != outIndex)
    {
      if (fileStreams[i].eof())
      {
        result++;
      }
    }
  }
  return result;
}

int getEmptyTape(vector<fstream> &fileStreams, int outIndex)
{
  int result = -1;
  for (int i = 0; i < fileStreams.size(); i++)
  {
    if (i != outIndex)
    {
      if (fileStreams[i].eof())
      {
        //cout << "in getEmptyTape() " << i << endl;
        if (result == -1)
        {
          result = i;
        }
        else
        {
          //cout << "More than one empty tape!!" << endl;
        }
      }
    }
  }

  //cout << "getEmptyTape() returning " << result << endl;
  return result;
}

/*------------------------------------------------------------------------------
   Function: LoadSortInputBlocks
   You should:
     X (1) Loads input data as blocks;
     X (2) sort each block internally;
     X (3) distribute blocks to the tapes for PolyPhaseMerge;
     X (4) return how many passes you need to perform the PolyPhaseMerge.

   Vairables:
     input_file  - the file name that contains the inputs
     ext_arrays  - names of the files that serve as the external tapes
     reverse     - if set true, sort in descending order; otherwise in ascending order
------------------------------------------------------------------------------*/

int LoadSortInputBlocks(
    const std::string &input_file,
    const std::vector<std::string> &ext_arrays,
    bool reverse = false)
{
  int block = BATCH_SIZE;
  int cnt_pass = 1; // initializing to one - this will represent the number of passes needed for polyphase merge
                    // internally to this function, this will represent the current pass of distributing blocks to tapes
  std::vector<int> blocksPerTape;
  // initialize blocksPerTape for first pass
  GetBlocksAt(cnt_pass, blocksPerTape);

  /*------ CODE BEGINS ------*/
  /*
      open all the tapes
      tape_count = 0;
      read block, sort block, write to first tape,
        if # blocks on a tape is > getBlocksAt( that tape )
          then go to next tape
          increment tape_count
        otherwise
          keep writing blocks to same tape
      increase cnt_pass IF tape_count > TOTAL_TAPES - 1 (15)
  */
  // OPEN ALL THE TAPES
  // DistBlocks < stream object, # of blocks on that tape >
  vector<pair<ofstream, size_t>> DistBlocks(ext_arrays.size());
  size_t tape_count = 0;
  ifstream in(input_file.c_str());
  for (int i = 0; i < ext_arrays.size() - 1; i++)
  {                                                  // DistBlocks is vector of ofstream objects
    DistBlocks[i].first.open(ext_arrays[i].c_str()); // open stream object
    DistBlocks[i].second = 0;                        // initialize # of blocks on each tape to 0
  }

  // blockArr will hold the values of the current block
  vector<size_t> blockArr;
  std::string buffer = "";
  while (!in.eof())
  {
    // read block from input file into blockArr
    for (int i = 0; i < BATCH_SIZE; i++)
    {
      if (!(in.is_open()))
      {
        //std::cout << "LoadInputSort Error" << std::endl;
      }
      else
      {
        if (!in.eof())
        {
          getline(in, buffer);
          if (buffer != "")
          {
            blockArr.push_back((stoi(buffer)));
          }
        }
      }
    }
    // sort blockArr
    InsertionSort(blockArr, 0, blockArr.size() - 1, reverse);

    // write blockArr to tape ext_arrays[tape_count]
    for (int i = 0; i < blockArr.size(); i++)
    {
      DistBlocks[tape_count].first << blockArr[i] << endl; // write block to tape
    }
    DistBlocks[tape_count].second++;             // increment # of blocks on that tape
    DistBlocks[tape_count].first << "$" << endl; // write delimiter to tape";

    // if # of blocks on tape is >= getBlocksAt(tape_count)
    if (DistBlocks[tape_count].second >= blocksPerTape[tape_count])
    {
      // go to next tape
      tape_count++;
    }
    if (tape_count >= TOTAL_TAPES - 1)
    {
      // increment cnt_pass
      cnt_pass++;
      // reset tape_count
      tape_count = 0;
      // get blocksPerTape for next pass
      GetBlocksAt(cnt_pass, blocksPerTape);
    }
    // clear blockArr
    blockArr.clear();
  }
  for( int i = 0; i < DistBlocks.size(); i++){
    while(DistBlocks[i].second < blocksPerTape[i]){
      DistBlocks[i].first << "$" << endl;
      DistBlocks[i].second++;
    }
  }
  // now, since the input file has been distributed, we should check that the number of blocks on every tape
  // == the number of blocks that should be on that tape
  // if not, write "$" to delimit a "fake" dummy block
  //std::cout << "cnt_pass: " << cnt_pass << endl;
  return cnt_pass;
}

/*------------------------------------------------------------------------------
   Function: PolyPhaseMerge
   You should
     (1) load data from k external tapes
     (2) repeat (3) until you have merged all blocks in any one of the tapes:
     (3) merge one block from each tape into one block, and write the merged block into an external tape.
     (4) clean up intermediate files.

   Vairables:
     cnt_pass    - total number of passes you need to perform PolyPhaseMerges,
                   this is computed from LoadSortInputBlocks
     ext_arrays  - the names of the list of files that serve as the external arrays
     reverse     - if set true, sort in descending order; otherwise in ascending order

------------------------------------------------------------------------------*/
void PolyPhaseMerge(
    const int cnt_pass,
    std::vector<std::string> &ext_arrays,
    bool reverse)
{
  /*------ CODE BEGINS ------*/
  // open ext_arrays for reading

  int cur_pass = 0;
  priority_queue<pair<size_t, size_t>, vector<pair<size_t, size_t>>, greater<pair<size_t, size_t>>> minQueue;
  priority_queue<pair<size_t, size_t>, vector<pair<size_t, size_t>>, less<pair<size_t, size_t>>> maxQueue;

  /*-------------------------------------------------------------------------------------------
  pseudocode, () is plain English description. Comments are explanation.
  1. (initialize file streams here)
    // if you reopen all file streams inside the while(cnt_pass > 0)
    // then you move the filestream back to the beginning of files
    // which reads duplicated input from the tapes / files that have not consumed all blocks
  2. while(cnt_pass > 0)
  {
    3.
    while(NONE OF THE TAPES have not been fully consumed)
    {
      4.
      (initialize the PriorityQueue to read the first entry of each block of all input tapes)
      5.
      while(PriorityQueue is not empty)
      {
        5.a
        pop from the queue
        5.b
        write to the output tape
        5.c
        if (not at end of a block for the popped value)
          5.c.i
          push next value, from the tape which was just popped
      }
      3.a
      (update whatever you designed for checking if NONE OF THE TAPES have not been fully consumed)
    }
  6.
    6.a
    (close the output tape and make it input file stream)
    6.b
    (close the empty input tape and make it output file stream)
    6.c
    (update the cnt_least_input and cnt_output)
    2.a
    cnt_pass--;
  } // end of while(cnt_pass > 0)
  -------------------------------------------------------------------------------------------*/

  auto passes_remaining = cnt_pass+1;
  int prev_output = ext_arrays.size() - 1;
  int cnt_output = ext_arrays.size() - 1;
  vector<fstream> fileStreams(ext_arrays.size());
  for (int i = 0; i < ext_arrays.size(); i++)
  {
    if (i != cnt_output)
    { // don't open the output tape as input fstream
      fileStreams[i].open(ext_arrays[i].c_str(), ios::in);
    }
    else
    {
      fileStreams[i].open(ext_arrays[i].c_str(), ios::out);
    }
  }
  while (passes_remaining >= 0)
  {

    for (int i = 0; i < fileStreams.size(); i++)
    {
      if (i != cnt_output)
      {
        if (fileStreams[i].eof())
        {
          fileStreams[i].close();
          fileStreams[i].clear();
          fileStreams[i].open(ext_arrays[i].c_str(), ios::out | ios::trunc);
          fileStreams[i].seekg(0, ios::beg);
        }
      }
    }


    bool emptyTapePresent = false;
    while (!emptyTapePresent)

    {
      // initialize the queue ( first entry of each tape, with index relating to which tape val is associated with)
      for (int i = 0; i < fileStreams.size(); i++)
      {
        if (i != cnt_output)
        {
          string buf = "";
          if(!fileStreams[i].eof()){
            getline(fileStreams[i], buf);
          }
          else{
            continue;
          }
          if (reverse)
          {
            if (buf != "" && buf != "$")
              maxQueue.push(make_pair(stoi(buf), i));
          }
          else
          {
            if (buf != "\n" && buf != "" && buf != "$")
            {
              minQueue.push(make_pair(size_t(stoi(buf)), i));
            }
          }
        }
      }
      if (reverse){
        while (!maxQueue.empty())
        {
          pair<size_t, size_t> val = maxQueue.top();
          maxQueue.pop();
          fileStreams[cnt_output] << val.first << endl;
          string buf = "";
          if (!fileStreams[val.second].eof())
          {
            getline(fileStreams[val.second], buf);
            if (buf != "" && buf != "$")
            {
              maxQueue.push(make_pair(size_t(stoi(buf)), val.second));
            }
          }
        }
      }
      else{
        while (!minQueue.empty())
        {
          pair<size_t, size_t> val = minQueue.top();
          minQueue.pop();
          fileStreams[cnt_output] << val.first << endl;
          string buf = "";
          if (!fileStreams[val.second].eof())
          {
            getline(fileStreams[val.second], buf);
            if (buf != "" && buf != "$")
            {
              minQueue.push(make_pair(size_t(stoi(buf)), val.second));
            }
          }
        }
      }
      if(passes_remaining <= 0){
        break;
      }
      fileStreams[cnt_output] << "$" << endl; // delimit new block
      emptyTapePresent = EmptyTapes(fileStreams, cnt_output);

    // reached an empty tape - will be tape with index cnt_least_input
    }
    if (EmptyTapes(fileStreams, cnt_output))
    {
      // get empty tape index from fileStreams
      int emptyTapeIndex = getEmptyTape(fileStreams, cnt_output); // empty tape index may be reporting incorrectly - check this
      // close the output tape
      fileStreams[cnt_output].close();
      // close the empty tape
      fileStreams[emptyTapeIndex].close();
      // open ext_arrays[emptyTapeIndex] as output fstream as it's empty now
      fileStreams[emptyTapeIndex].open(ext_arrays[emptyTapeIndex].c_str(), ios::out);
      //cout << "just closed: " << emptyTapeIndex << endl;
      fileStreams[emptyTapeIndex].seekg(0, ios::beg);
      // open ext_arrays[cnt_output] as input fstream as it's the new output tape
      fileStreams[cnt_output].open(ext_arrays[cnt_output].c_str(), ios::in);
      fileStreams[cnt_output].seekg(0, ios::beg);
      prev_output = cnt_output; // sets prev_output, only used for final pass to rename the final output tape to output.txt
      cnt_output = emptyTapeIndex;

    }
    passes_remaining--;
  }

  // close all file streams
  for (int i = 0; i < fileStreams.size(); i++)
  {
    if ( i != prev_output){
      fileStreams[i].close();
      remove(ext_arrays[i].c_str());
    }
    else{
      fileStreams[i].close();
      rename(ext_arrays[i].c_str(), "output.txt");
    }
  }
  return;
}
/*------ CODE ENDS ------*/

/*------------------------------------------------------------------------------
 The driver external sort function function
   input_file   - the file name that contains the inputs
   output_file  - the file name that contains the final sorted elements
   reverse      - if set true, sort in descending order; otherwise in ascending order
------------------------------------------------------------------------------*/
void ExternalSort(const std::string &input_file, const std::string &output_file, bool reverse = false)
{

  /*------ CODE BEGINS ------*/
  std::vector<std::string> files;
  for (int i = 1; i <= TOTAL_TAPES; i++){
      files.push_back("test_" + std::to_string(i) + ".txt");
  }
  int cnt_pass = LoadSortInputBlocks(input_file, files, reverse);
  PolyPhaseMerge(cnt_pass, files, reverse);
  rename("output.txt", output_file.c_str());
  /*------ CODE ENDS ------*/
}

#endif
