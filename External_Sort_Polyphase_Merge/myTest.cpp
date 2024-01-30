#include <iostream> 
#include "MyExternalSort_Template.hpp"

char abd;

int main(){

    string in = "Inputs/input_1.txt";
    string out = "output.txt";
    bool reverse = true;
    ExternalSort(in, out, reverse);
    int sorted = IsSorted(out, reverse);
    if (sorted == 1)
        cout << "output is sorted" << endl;
    else
        cout << "output is not sorted" << endl;
    //std::vector<size_t> testInsertSort;
    //for (int i = 0; i < 11; i++){
    //    testInsertSort.push_back(i);
    //}
    //std::vector<std::string> files;
    //for (int i = 1; i <= TOTAL_TAPES; i++){
    //    files.push_back("test_" + std::to_string(i) + ".txt");
    //}
//
    //int cnt_pass = LoadSortInputBlocks("Inputs/input_2.txt", files);
    //
    ////std::cout << "passes calculated by LoadSortInputBlocks" << cnt_pass << endl;
    ////int totalElems = countTotalElements(files);
    ////std::cout << "total elements: " << totalElems << endl;
    //cout << "elements on all tapes: " << countTotalElements(files) << endl;
    //cout << "polyphase merge time" << endl;
    //cin >> abd;
//
    //PolyPhaseMerge( cnt_pass, files, false);
    //std::cout << countTotalElements(files) << endl;
    //cout << "elements on output tape: " << countElemsOnTape("test_8.txt") << endl;
    //std::cout << "all data sorted? " << IsSorted(files[files.size() - 1], false) << endl;

    //std::cout << "ensure data is still of same size" << std::endl;
    //int count = 0;
    //for ( auto file: files){
    //    ifstream input(file);
    //    std::string line;
    //    while (input.peek() != EOF){
    //        getline(input, line);
    //        count++;
    //    }
    //}

    //std::cout << "total lines: " << count << std::endl;
    //int totalBlocks = 0;
    //for (auto file: files){
    //    int blockOnFile = numBlockOnTape(file);
    //    std::cout << file << " contains " << blockOnFile << " blocks" << std::endl;
    //    totalBlocks += blockOnFile;
    //}
    //std::cout << "total blocks on all tapes: " << totalBlocks << std::endl;


    
    /*------------------------
                code for testing writeBlocksToTape
    ifstream input("Inputs/input_1.txt");
    ofstream output("writeBlocksTest.txt");
    ofstream output2("writeBlocksTest2.txt");
    // should write 4 blocks from input_1.txt -> writeBlocksTest.txt
    writeBlocksToTape(input, output, 4);
    writeBlocksToTape(input, output2, 2);
    -------------------------*/

    /*------------------------
        print k-step fibonacci series
    for (auto i : fibo_series){
        std::cout << i << " ";
    }
    -------------------------*/

    //std::vector<int> blocksPerTape;
    //GetBlocksAt(4, blocksPerTape);
    //std::cout << "blocks per tape: ";
    //Print(blocksPerTape);
    //cout << endl;

    /*------------------------
        code for testing getFirstBlock
    vector<size_t> firstBlock = getFirstBlock(files[0].c_str());
    for ( auto i : firstBlock){
        std::cout << i << "\n";
    }
    return 1;
    -------------------------*/
    /*------------------------
        code for testing Polyphase merge
    -------------------------*/
    //PolyPhaseMerge( cnt_pass, files, false);
    //std::string sorted = "";
    //if ( IsSorted("poly_buffer.txt", false) == true){
    //    sorted = "true";
    //} else {
    //    sorted = "false";
    //}
    //cout << "sorted: " << sorted << endl;
//
    //std::cout << "ensure data is still of same size" << std::endl;
    //int count2 = 0;
    //for ( auto file: files){
    //    ifstream input(file);
    //    std::string line;
    //    while ((getline(input, line))){
    //        count2++;
    //    }
    //}
    //ifstream input("poly_buffer.txt");
    //while(input.peek() != EOF){
    //    std::string line;
    //    getline(input, line);
    //    count2++;
    //}
    //std::cout << "total lines: " << count2 << std::endl;
    //std::cout << "all data sorted? " << IsSorted("poly_buffer.txt", false) << endl;
}