#include "Phalanx_ConfigDefs.hpp"
#include "Phalanx.hpp"

#include "Teuchos_RCP.hpp"
#include "Teuchos_ArrayRCP.hpp"
#include "Teuchos_TestForException.hpp"
#include "Teuchos_TimeMonitor.hpp"

int main(int argc, char *argv[]) 
{
  using namespace std;
  using namespace Teuchos;
  using namespace PHX;
  
  try {
    
    RCP<Time> total_time = TimeMonitor::getNewTimer("Total Run Time");
    TimeMonitor tm(*total_time);

    // *********************************************************************
    // Start of Allocator Testing
    // *********************************************************************
    {
      DefaultAllocator ma;
      ArrayRCP<double> vec = 
	ma.allocate<double>(4);
      cout << "Testing Default Allocator" << endl;
      cout << "vec size = " << vec.size() << ", should be 4." << endl;
      TEST_FOR_EXCEPTION(vec.size() != 4, std::runtime_error, 
			 "Allocator is broken!");
    }

    {
      ContiguousAllocator ca;
      const int size = 100;
      const int num_bytes = size * sizeof(double);
      ca.addRequiredBytes(num_bytes);
      ca.addRequiredBytes(num_bytes);
      
      const int total_bytes = ca.getTotalBytes();
      TEST_FOR_EXCEPTION(total_bytes != 2 * num_bytes, std::logic_error,
			 "addRequiredBytes() is failing!");
      
      ca.reset();
      
      
    }

    // *********************************************************************
    // *********************************************************************
    std::cout << "\nTest passed!\n" << std::endl; 
    // *********************************************************************
    // *********************************************************************

  }
  catch (const std::exception& e) {
    std::cout << "************************************************" << endl;
    std::cout << "************************************************" << endl;
    std::cout << "Exception Caught!" << endl;
    std::cout << "Error message is below\n " << e.what() << endl;
    std::cout << "************************************************" << endl;
  }
  catch (...) {
    std::cout << "************************************************" << endl;
    std::cout << "************************************************" << endl;
    std::cout << "Unknown Exception Caught!" << endl;
    std::cout << "************************************************" << endl;
  }

  TimeMonitor::summarize();
    
  return 0;
}
