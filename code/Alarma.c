///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   MrCabrac      
//   26/Junio/16
//
//   Programa:    Alarma 
//   Version:     1.2
//   Dispositivo: PIC16F88
//
//   Entradas:    conductor(uno)-baul(dos)-acompañante(tres)-traseraizquierda(cuatro)-traseraderecha(cinco)
//                Capot(seis)-ignicion(siete)-valet(valet)-boton de proteccion(nueve)
//
//   Entradas RC: rcdesactivar(desactivar)-rcubicacion(ubicar)-rcarmar(armar)-rcdesarmar(desarmar)
//   
//   Salidas: Bocina(suno)-lucesestacionarias(sdos)-lucesaltas(stres)-cable de poder(scuatro)-GSM on(scinco)
//            GSM off(sseis)-led de estado(ssiete)-luz cielo, baul, capot(socho-snueve-sdiez)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <16f877a.h>
#fuses XT,NOWDT,NOPROTECT,NOWRT,NOPUT
#use delay(clock=4000000)

//DEFINE entradas
#define uno PIN_B0     //conductor
#define dos PIN_B1     //baul
#define tres PIN_B2    //acompañante
#define cuatro PIN_B3  //trasera izquierda
#define cinco PIN_B4   //trasera derecha
#define seis PIN_B5    //capot
#define siete PIN_D1   //ignicion
#define valet PIN_B7   //valet
#define nueve PIN_C0   //proteccion
#define desac PIN_C1   //desactivar
#define ubicar PIN_C2  //ubicar
#define armar PIN_C3   //armar
#define desar PIN_D0   //desarmar


//DEFINE salidas
#define suno PIN_D2    //bocina
#define sdos PIN_D3    //estacionarias
#define stres PIN_C4   //altas
#define scuatro PIN_C5 //poder
#define scinco PIN_D6  //GSM on
#define sseis PIN_D7   //GSM off
#define ssiete PIN_D5  //estado
#define socho PIN_C6   //cielo
#define snueve PIN_C7  //baul
#define sdiez PIN_D4   //capot

///DECLARACIONES DE FUNCIONES
void activar(void);               //ilumina LED,luces altas y estacionarias
void desactivar(void);            //desactiva la alarma
void desarmar(void);              //desarma la alarma
void led_estado(void);            //LED que muestra el estado de la alarma
void ubicacion(void);             //muestra la ubicacion
void cambio_de_estado(void);      //muestra por medio visual y auditivo el cambio en el estado de la alarma

//DECLARACION DE VARIABLES
int i,x;

//ALARMA ON
void main (void){
   
   disable_interrupts(GLOBAL);
   set_tris_b(0x11111111);
   set_tris_c(0x11110000);
   set_tris_d(0x11000100);
   output_b(0x00);
   output_c(0x00);
   output_d(0x00);
   
   do{
       ////////ESTADO DE ALARMA ARMADA////////////
       armado:
       output_low(scuatro);  //Desconectamos el cable de poder
       output_low(suno);     //Apagamos la bocina
       i=0;
       
       if(input(uno)==1 || input(dos)==1 ||input(tres)==1 ||input(cuatro)==1 ||input(cinco)==1 ||input(seis)==1 ||input(siete)==1){//si se detecta un cambio
  
            if(input(siete)==1 && x==1){
            	output_high(suno);
                x=2;
                activar();
                x=0;
            }

            output_high(suno);//encender bocina
            pregunta_rcdesactivar://¿se recibe señal para desactivar?
            if(input(desac)==1){//si
               output_low(suno);//apagar bocina
               goto armado; //ir a armado
            }
               
            else{//no
                if(input(desar)==1){ //si desarmado
                    cambio_de_estado();
                    goto desarmado;
                }

                else{ //no desarmado
                    output_high(scinco);//hacemos la llamada
                    delay_ms(20);
                    output_low(scinco);
                    activar();//encendemos luces
                    i++; //contamos
                    if(i==15){//si es igual a 15
                       	output_high(sseis); //cancelamos la llamada
                       	delay_ms(20);
                       	output_low(sseis);
                       	x=0;
                       	x=1;
                       	goto armado; //ir a armado   
                    }

                    else{
                     	goto pregunta_rcdesactivar;
                    }

                  	}
            }
       }
        

       else{ //no recibieron cambios
         	pregunta_rcdesarmar://¿se recibe señal de desarmado?
         	if(input(desar)==1){//si
            	cambio_de_estado();
            	goto desarmado;//desarmamos
         	}
   
         	else{//no
            	led_estado();//prendemos el led de estado armado
            	if(input(ubicar)==1){//si recibe señal de ubicaion
             	  	ubicacion();//prendemos las señales de ubicacion
               		goto armado;//ir a armado
            	}   
           
            	else{//no se recibe señal de ubicacion
               		goto armado;//ir a armado    
            	}   
         	}
       }
      
       for(;;){/////////////////////programa desarmado////////////////////
       		desarmado:
         	if(input(valet)==0){
         		output_high(scuatro);//activamos el cable de poder
         	}

         	output_low(suno);
         	output_low(sdos);
         	output_low(stres);
         	output_low(sseis);
         	output_low(ssiete);
         
         	///////////////////////////LUZ DE CIELO/////////////////////////////////////////
         	if(input(uno)== 1 || input(tres)== 1 || input(cuatro)== 1 || input(cinco)== 1){
            	output_high(socho);
         	}
         
         	else{
            	output_low(socho);
         	}
         
         	///////////////////////////LUZ DE BAUL///////////////////////////////////////////
         	if(input(dos)==1){
            	output_high(snueve);
         	}
         
         	else{
            	output_low(snueve);
         	}
         
         	///////////////////////////LUZ DE CAPOT//////////////////////////////////////////
         	if(input(seis)==1){
            	output_high(sdiez);
         	}
         
         	else{
            	output_low(sdiez);
         	}
         	/////////////////////////////////////////////////////////////////////////////////


       		if(input(nueve)==0){//si boton de proteccion esta activo //////////////////modo proteccion///////////
            	modo_interno://modo interno de proteccion              
/*entrada*/ 	if(input(uno)==1 || input(dos)==1 ||input(tres)==1 ||input(cuatro)==1 ||input(cinco)==1 ||input(seis)==1){
            		entrada_activa:
            		output_high(suno);
            		delay_ms(300);
            		output_low(suno);
               		activar();//activamos la alarma
               		if(input(valet)==0){//si valet esta activo
                		goto desarmado;//volvemos al inicio
               		}
                     
               		else{//si no se presiona valet
                		goto entrada_activa;//volvemos a pregunta si se presiona valet
               		}
            	}
         
           		else{//si no hay cambios en las entradas
               		goto desarmado;
            	}   
        	}
       
        	else{//si boton de proteccion NO esta activo
            	modo_externo://programa modo externo
            	if(input(armar)==1){//si recibe señal de armado
               		if(input(siete)==1){//si esta en movimiento
                  		goto desarmado;//volvemos a inicio
               		}
            
               		else{//si no esta en movimiento
                  		cambio_de_estado();
                  		goto armado;//armamos la alarma
               		}   
            	}
         
            	else{//si no recibe señal de armado
            		goto desarmado;
            	} 
       		}
   		}

   }while(TRUE);
}

void activar(void){
   output_high(sdos);
   output_high(stres);
   output_high(ssiete);
   delay_ms(500);
   output_low(sdos);
   output_low(stres);
   output_low(ssiete);
   delay_ms(500);
}

void desactivar(void){
   output_low(suno);
   output_low(sdos);
   output_low(stres);   
}

void led_estado(void){
   output_high(ssiete);
   delay_ms(100);
   output_low(ssiete);
   delay_ms(500);
}

void ubicacion(void){
   output_high(suno);
    output_high(sdos);
    output_high(stres);
    delay_ms(100);
    output_low(suno);
    output_low(sdos);
    output_low(stres);
}

void cambio_de_estado(void){
   output_high(suno);
   output_high(sdos);
   delay_ms(70);
   output_low(suno);
   output_low(sdos);
}
