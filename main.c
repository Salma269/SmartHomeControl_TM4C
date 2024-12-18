#include "Lamp_Plug.h"
#include "stdint.h"
#include "InputButton.h"
#include "tm4c123gh6pm.h"

//pins used
//PF1 for Lamp
//PF2 for plug
//PD0 for Plug Button
//PD1 for Lamp Button



int main(){

  
InitializePortF_Lamp_Plug_timer_Button(void);   //initialize PortF
configure_PD0_PD1_as_input_portD(void);         //Initialize portD

while (1) {
  TriggerButtonForPlug();
  TriggerButtonForLamp();
}


}