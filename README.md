# Communication Between Arduino and PSIM using DLL Block

## Introduction

I designed a PI controller for a BLDC motor, running in PSIM, but unfortunatly I didn't find a potentiometer inside the program, for me to test the stability of the system towards a disturbance in the reference signal. This project was then created, to allow easy serial-port communication between a real potentiometer connect to an Arduino and the PSIM application. 

For that, it uses a DLL Block inside PSIM, running a C++ application that uses a Serial Communication to read the Arduino input. The C++ code is in the file _export-functions.cpp_, inside the folder _ArduinoDLL_. It can be opened with the program _Dev-C++_, version 5.11.

## Writing to Serial port

This is the code running that runs in the Arduino. In my case, it used an Arduino Nano, with the potenciometer connected to the analog port A1. It is necessary to specify the BAUD RATE of the communication, which in this case is 9600.

The value of the potentiometer is sent byte per byte to the computer.

```cpp
void setup() {
  Serial.begin(9600); // Initialize the Serial Communication

void loop() {
  float potValue = analogRead(A1); // Read the potentiometer value
  byte* bytePtr = (byte*)&potValue; // Obtain the adress of the float as an array of bytes
  
  // Send the value byte per byte
  for (int i = 0; i < sizeof(potValue); i++) {
    Serial.write(bytePtr[i]);
  }

  delay(1000);
}
```

After that, the Arduino is conected to the computer via USB, and the values are read using a C++ application inside the DLL Block.

## Initialization the DLL Block

To properly use the program, you need to specify the COM PORT in the function `CreateFile` and the BAUD RATE using the variable `dcb.BaudRate`. After the necessary modifications, the code need to be compiled inside Dev-C++ (F9). Obs: The code only compiles if the DLL Block is not opened in Psim.

```cpp
HANDLE m_hSerialComm; 
  
DLLIMPORT void SimulationBegin( 
   const char *szId, int nInputCount, int nOutputCount, 
    int nParameterCount, const char ** pszParameters, 
    int *pnError, char * szErrorMsg, 
    void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr) 
 { 
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
```

## Reading from the Serial port


```cpp    
 // FUNCTION: SimulationStep 
 DLLIMPORT void SimulationStep( 
   double t, double delt, double *in, double *out, 
    int *pnError, char * szErrorMsg, 
    void ** ptrUserData, int nThreadIndex, void * pAppPtr) 
 { 
 
  float valor_potenciometro; 
  DWORD bytesRead = 0; 
   
  // Read the Serial Port
  if (!ReadFile(m_hSerialComm, &valor_potenciometro, sizeof(valor_potenciometro), &bytesRead, NULL) || bytesRead != sizeof(valor_potenciometro)) { 
      *pnError = 1; 
      strcpy(szErrorMsg, "Erro ao ler dados da porta serial."); 
      return; 
  } 
   
  out[0] = valor_potenciometro; 
  
  *pnError = 0; //Success 
 } 
 ```

## Closing Serial port

```cpp
 // FUNCTION: SimulationEnd 
 DLLIMPORT void SimulationEnd(const char *szId, void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr) 
 { 
  CloseHandle(m_hSerialComm);  
 } 
}
```

## Inside PSIM

To use this code in PSIM, the _General DLL Block_ is used, specifying the number of inputs and outputs (for the use of a potentiometer, the block will only have one output and doesn't need inputs). It can be found going to _Elements > Other > Function Blocks > General DLL Block_. 

After that, the code is imported selecting the PsimBasicDLL.dll inside the folder ArduinoDLL of the repository.

The video below shows a demonstration of the code:

https://github.com/user-attachments/assets/f24116b3-b903-4fd9-96b4-153c01fedc99








