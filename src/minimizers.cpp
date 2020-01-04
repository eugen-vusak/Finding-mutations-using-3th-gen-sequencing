/**
*
* This is a test file for reading FASTA files and finding representative k-mer from a group 
* of adjecent k-mers.
*
*/

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<list>
#include<algorithm>

using namespace std;

void updateMinimizers(map<char*, list <int>> minimizers, vector<char> buffer, int w, 
                     int k, int start){
   
   list<char*> kmers;

   for (int index = 0; index < buffer.size() - k + 1; ++index) {
      char* kmer = new char[k + 1];
      copy(buffer.begin() + index, buffer.begin() + index + k, kmer);
      kmer[k] = '\0';
      
      kmers.push_back(kmer);
   }

   kmers.sort();  

   char* minimizer = kmers.front();
   if(minimizers.count(minimizer)){
      minimizers.find(minimizer)->second.push_back(start);
   }else{
      minimizers.insert(pair<char*, list <int>>(minimizer, list<int>({start})));
   }

	cout << "-> " << minimizers.find(minimizer)->first << endl;
}

void readFASTAFile(string filename, int w, int k){
   ifstream myReadFile;

   map<char*, list <int>> minimizers;

   myReadFile.open(filename);
   if (!myReadFile.is_open()) {
      cout << "ERROR: the file did not open!\n";
      exit(1);
   }

   string firstLine;
   getline(myReadFile, firstLine);
   cout << firstLine + '\n';

   int i = 0;
   int n = 0;
   int iter_num = 2;
   const int INPUT_SIZE = w + k - 1;
   vector<char> buffer (INPUT_SIZE, 0);
   
   while (!myReadFile.eof()) {
      myReadFile.read(buffer.data(), buffer.size());

      streamsize s = ((myReadFile) ? INPUT_SIZE : myReadFile.gcount());
   	buffer[s] = 0;

      //cout << buffer.data() << endl;

      updateMinimizers(minimizers, buffer, w, k, i);

      i += INPUT_SIZE;

      if(n++ >= iter_num) break;
   }

   /*
   for(auto elem : minimizers) {
      cout << elem.first << " " << elem.second.back() << "\n";
   }*/

   myReadFile.close();
}

int main(){
   string filename = "lambda.fasta";   //dir name
   int w = 10;
   int k = 10;
   
   readFASTAFile(filename, w, k);

   return 0;
}
