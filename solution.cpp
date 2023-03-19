#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */ 

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class COptimizer 
{
  public:
    static bool                        usingProgtestSolver                     ( void )
    {
      return true;
    }
    static void                        checkAlgorithm                          ( AProblem                              problem )
    {
      // dummy implementation if usingProgtestSolver() returns true
    }
    void                               start                                   ( int                                   threadCount );
    void                               stop                                    ( void );
    void                               addCompany                              ( ACompany                              company );
private:
	std::vector<ACompany>  m_Companies;
	std::vector<pthread_t> m_Thread_ids;
};

// TODO: COptimizer implementation goes here
void *worker_thread(void * arg) {
	CProgtestSolver solver = createProgtestSolver();
	while(true) {
		// wait until there is problem_pack
		// add CProblems to solver from problem_pack
		// start solving
		solver.solve();
		// return solved pack to company
	}
}

void *communication_get_thread(void* arg) {
	ACompany company = arg;
	while (true) {
		AProblemPack pack = company.waitForPack();
		if (pack == nullptr) {
			break;
		}
		// Add pack to shared input list to the end
	}
}

void *communication_put_thread(void* arg) {
	ACompany company = arg;
	while (true) {
		// get solved pack from output list (may block)
		company.solvedPack();
	}
}

// Add company
void  COptimizer::addCompany(ACompany company) {
	 m_Companies . push_back ( std::move ( company ) );
}

// Start
void COptimizer::start(int threadCount) {
	// Start threadCount of working threads
	for (int i = 0; i < threadCount; i++) {
		pthread_t tid;
		if (pthread_create(&tid, nullptr, worker_thread, nullptr) != 0) {
			throw std::logic_error("cannot create thread");
	        }
		m_Thread_ids.push_back(tid);
	}
	
	for (auto company :  m_Companies) {
		pthread_t tid;
		if (pthread_create(&tid, nullptr, communication_get_thread, company) != 0) {
			throw std::logic_error("cannot create thread");
	        }
		m_Thread_ids.push_back(tid);
		if (pthread_create(&tid, nullptr, communication_put_thread, company) != 0) {
			throw std::logic_error("cannot create thread");
	        }
		m_Thread_ids.push_back(tid);
	}
}
// Stop
void COptimizer::stop(void) {
	
}

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
int                                    main                                    ( void )
{
  COptimizer optimizer;
  ACompanyTest  company = std::make_shared<CCompanyTest> ();
  optimizer . addCompany ( company );
  optimizer . start ( 4 );
  optimizer . stop  ();
  if ( ! company -> allProcessed () )
    throw std::logic_error ( "(some) problems were not correctly processsed" );
  return 0;  
}
#endif /* __PROGTEST__ */ 
