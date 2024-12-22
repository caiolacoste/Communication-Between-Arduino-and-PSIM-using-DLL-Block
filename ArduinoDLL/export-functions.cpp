/* Replace "dll.h" with the name of your header */
#include "dll.h"
#include <windows.h>
#include <math.h>

extern "C"
{
	
	HANDLE m_hSerialComm;
	
	/////////////////////////////////////////////////////////////////////
	// FUNCTION: SimulationBegin
	//   Initialization function. This function runs once at the beginning of simulation
	//   For parameter sweep or AC sweep simulation, this function runs at the beginning of each simulation cycle.
	//   Use this function to initialize static or global variables.
	//const char *szId: (read only) Name of the C-block 
	//int nInputCount: (read only) Number of input nodes
	//int nOutputCount: (read only) Number of output nodes
	//int nParameterCount: (read only) Number of parameters is always zero for C-Blocks.  Ignore nParameterCount and pszParameters
	//int *pnError: (write only)  assign  *pnError = 1;  if there is an error and set the error message in szErrorMsg
	//    strcpy(szErrorMsg, "Error message here..."); 
	// DO NOT CHANGE THE NAME OR PARAMETERS OF THIS FUNCTION
	DLLIMPORT void SimulationBegin(
			const char *szId, int nInputCount, int nOutputCount,
			 int nParameterCount, const char ** pszParameters,
			 int *pnError, char * szErrorMsg,
			 void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
	{
	// ENTER INITIALIZATION CODE HERE...
	
		DCB dcb;
	
		m_hSerialComm = CreateFile("COM4",
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		
		if (m_hSerialComm == INVALID_HANDLE_VALUE) ; // HANDLE ERROR CONDITION
		
		GetCommState(m_hSerialComm, &dcb);
		dcb.BaudRate = 9600;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.Parity = NOPARITY;
		SetCommState(m_hSerialComm, &dcb);
	}
	
		 
	/////////////////////////////////////////////////////////////////////
	// FUNCTION: SimulationStep
	//   This function runs at every time step.
	//double t: (read only) time
	//double delt: (read only) time step as in Simulation control
	//double *in: (read only) zero based array of input values. in[0] is the first node, in[1] second input...
	//double *out: (write only) zero based array of output values. out[0] is the first node, out[1] second output...
	//int *pnError: (write only)  assign  *pnError = 1;  if there is an error and set the error message in szErrorMsg
	//    strcpy(szErrorMsg, "Error message here..."); 
	DLLIMPORT void SimulationStep(
			double t, double delt, double *in, double *out,
			 int *pnError, char * szErrorMsg,
			 void ** ptrUserData, int nThreadIndex, void * pAppPtr)
	{
		
	// Place your code here............begin

		float valor_potenciometro;
		DWORD bytesRead = 0;
		
//		ReadFile(m_hSerialComm,&valor_potenciometro,sizeof(valor_potenciometro),NULL,NULL);	
		
		// Ler o valor da porta serial
        if (!ReadFile(m_hSerialComm, &valor_potenciometro, sizeof(valor_potenciometro), &bytesRead, NULL) || bytesRead != sizeof(valor_potenciometro)) {
            *pnError = 1;
            strcpy(szErrorMsg, "Erro ao ler dados da porta serial.");
            return;
        }
		
		out[0] = valor_potenciometro;
		
	// Place your code here............end
	
		*pnError = 0; //Success
	}
	
	/////////////////////////////////////////////////////////////////////
	// FUNCTION: SimulationEnd
	//   Termination function. This function runs once at the end of simulation
	//   For parameter sweep or AC sweep simulation, this function runs at the end of each simulation cycle.
	//   Use this function to de-allocate any allocated memory or to save the result of simulation in an alternate file.
	// Ignore all parameters for C-block 
	// DO NOT CHANGE THE NAME OR PARAMETERS OF THIS FUNCTION
	DLLIMPORT void SimulationEnd(const char *szId, void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
	{
		CloseHandle(m_hSerialComm);
	
	}
	

	
}
