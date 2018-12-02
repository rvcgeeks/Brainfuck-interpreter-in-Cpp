/* ___________________________________________________________
 * ____________<<<___#_R_V_C_G_E_E_K_S___>>>__________________
 * CREATED BY #RVCGEEKS @PUNE for more rvchavadekar@gmail.com
 *
 * #RVCGEEKS Brainfuck language interpreter
 * created on 18.11.2018
 *  
 * A lightweight brainfuck script evaluator / interpreter...
 * either run a source file or evaluate a brainfuck string
 * at runtime...
 * 
*/


//________________________________INTERPRETER____________________________________

#include <iostream>
#include <cstring>
using namespace std;

#ifndef __BRAINFUCK_CORE
#define __BRAINFUCK_CORE

#define MACHINE_SIZE 131072                   //size of main memory in VM...(currently set to 128kB)
#pragma pack(push,1)                          //for 1 byte perfectly packed structre alloc
template<class T>                             //template to give flexibility of cell size
class Brainfuck {
              T _BF_core_[MACHINE_SIZE],
               *_M_mem__ptr_;                 //thats why in an interpreted language everything is pointers... ;)
    const char *_M_prog_ptr_;
    
    void chk(const char *c,int& b) {
        if (!*c)
           throw runtime_error(
                      "\n UNMATCHED PARANTHESIS!!!\n"
                      "  looping paranthesis is incomplete... aborting...\n");
             if (*c == '[') b++ ;
        else if (*c == ']') b-- ;
    }
    
public:
    Brainfuck() {
      memset(_BF_core_ ,0 ,sizeof _BF_core_ );//initializing all core to 0
      _M_mem__ptr_ = _BF_core_;               //initializing memory Pointer to base address of core
    }
    
    void run(const char _prog[]) {
      int bal; _M_prog_ptr_ = _prog;          //initializing Program Pointer to base address of program string at run call
      while (*_M_prog_ptr_) {                 //program termination only when it reaches end of program string...('\0' char)
        switch (*_M_prog_ptr_) {
          case '+': (*_M_mem__ptr_) ++;break;
          case '-': (*_M_mem__ptr_) --;break;
          case '.': cout<<(char)*_M_mem__ptr_;break;
          case ',': cin>>*_M_mem__ptr_;break;
          case '>':
              if(_M_mem__ptr_ - _BF_core_ >= MACHINE_SIZE)    //abortion of interpretation on memory out of reach
                  throw runtime_error(
                      "\n Brainfuck has OVERFLOWED!!!\n"
                      "  ***out of memory***\n");
              _M_mem__ptr_ ++ ;
              break;
          case '<':
              if(_M_mem__ptr_ - _BF_core_ < 0)
                  throw runtime_error(
                      "\n Brainfuck has UNDERFLOWED!!!\n"
                      "  ***out of memory***\n");
              _M_mem__ptr_ -- ;
              break;
          case '[':
            bal = 1;                          //bal functions like paranthesis checker
            if (*_M_mem__ptr_ == '\0') {      //paranthesis indicates looping mechanism in bf
                do {
                    _M_prog_ptr_ ++; 
                    chk(_M_prog_ptr_ , bal);
                } while ( bal );
            } break;
          case ']':
            bal = 0;
            do {
                chk(_M_prog_ptr_ , bal);
                _M_prog_ptr_ --;
            } while ( bal );
        } _M_prog_ptr_++;
      }
    }
};
#pragma pack(pop)
#endif



//__________________________FILE READ AND USER INTERFACE_________________________________

void usage(char arg[]) {
  cout << "Evaluate a string or file of Brainfuck code.\n\n";
  cout << "Usage:\n\n";
  cout<<"  ~$ "<<arg<<" [filename]   evaluate a source file.\n";
  cout<<"  ~$ "<<arg<<" -e \"[string]\"  Directly evaluate a string as Brainfuck code.\n";
  cout<<"  ~$ "<<arg<<" -h           Print this usage.\n\n";
}

#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdint>

main(int argc, char **argv) {
  ifstream file;
  int length,opt;
  char *filename,*program;
  struct stat stats;
  
  if(argc<2){
      cout<<" Try\n  ~$ "<<argv[0]<<" -h\n  for help...\n";
      return 0;
  }
  opterr = 0;
  while ((opt = getopt (argc, argv, "he:")) != -1) {
    switch (opt) {
      case 'h':
        usage(argv[0]);
        exit(0);

      case 'e':
        program = (char*) optarg;
        break;

      case '?':
        if (optopt == 'e')
          cerr<<"Option '-"<<optopt<<"' requires an argument.\n";
        else
          cerr<<"Unknown option '-"<<optopt<<"'.\n";
        exit(1);

      default:
        exit(2);
      }
  }

  if(! program) {
    // not run with -e, lets look for a file
    filename = (char*) argv[optind];

    if( stat (filename, &stats) == 0 ) {
      file.open(filename);
      file.seekg(0, ios::end);
      length = file.tellg();
      file.seekg(0, ios::beg);
      program = new char[length];
      file.read(program, length);
      file.close();
    } else {
      cerr<<"Cannot open file "<<filename<<".\n";
      exit(2);
    }
  }

  Brainfuck<uint8_t> interpreter ;
  interpreter.run(program) ;
  return 0;
}
