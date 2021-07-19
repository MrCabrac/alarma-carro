///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   MrCabrac      
//   26/Junio/16
//
//   Programa:    Alarma 
//   Version:     1.3
//   Dispositivo: PIC16F88
//
//   Entradas:    conductor(uno)-baul(dos)-acompañante(tres)-traseraizquierda(cuatro)-traseraderecha(cinco)
//                Capot(seis)-ignicion(siete)-valet(valet)-boton de proteccion(nueve)
//
//   Entradas RC: rcdesactivar(desactivar)-rcubicacion(ubicar)-rcarmar(armar)-rcdesarmar(desarmar)
//   
//   Salidas: Bocina(suno)-lucesestacionarias(sdos)-lucesaltas(stres)-cable de poder(scuatro)-GSM on(scinco)
//            GSM off(sseis)-led de estado(ssiete)-luz cielo, baul, capot(socho-snueve-sdiez)-elevalunas(sonce)
//            bloqueo central(sdoce)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <16f887.h>
#fuses XT,NOWDT,NOPROTECT,NOWRT,NOLVP
#use delay(clock=4000000)

//DEFINE entradas
#define uno PIN_B0    //conductor
#define dos PIN_B1    //baul
#define tres PIN_B2   //acompañante
#define cuatro PIN_B3 //t.i
#define cinco PIN_B4  //t.d
#define seis PIN_B5   //capot
#define siete PIN_B6  //ignicion
#define valet PIN_B7  //valet
#define nueve PIN_C0  //proteccion
#define desac PIN_C1  //desac
#define ubicar PIN_C2 //ubicar
#define armar PIN_C3  //armar
#define desar PIN_C4  //desarmar


//define salidas
#define suno PIN_D0    //bocina
#define sdos PIN_D1    //estaciona
#define stres PIN_D2   //altas
#define scuatro PIN_D3 //poder
#define scinco PIN_D4  //gon
#define sseis PIN_D5   //goff
#define ssiete PIN_D6  //led
#define socho PIN_D7   //cielo
#define snueve PIN_A0  //baul
#define sdiez PIN_A1   //capot

///DECLARACIONES DE FUNCIONES
void activar(void);               //ilumina LED,luces altas y estacionarias
void desactivar(void);            //ilumina led"s izquierda a derecha
void desarmar(void);              //desarma la alarma
void led_estado(void);            //LED que muestra el estado de la alarma
void ubicacion(void);             //muestra la ubicacion
void llamada(void);               //hace la llamada
void colgar(void);                //anula la llamada
void cambio_estado(void);      //cuando cambia de estado

//DECLARACION DE VARIABLES
int i,y;

//ALARMA ON
void main (){
   
   disable_interrupts(GLOBAL);
   set_tris_a(0xFF); //porta como entrada
   set_tris_b(0xFF); //portb como entrada
   set_tris_c(0x00); //portc como salida
   set_tris_d(0x00); //portd como salida
   set_tris_e(0x00); //porte como salida
   output_a(0x00);   //porta off
   output_b(0x00);   //portb off
   output_c(0x00);   //portc off
   output_d(0x00);   //portd off
   output_e(0x00);   //porte off
   
   do{
       ///////////////////////////////////////PROGRAMA ARMDADO///////////////////////////////////////////
       armado:
       output_low(scuatro);  //Desconectamos el cable de poder
       i=0;
       y=0;
       
       if(input(uno)==0 || input(dos)==0 ||input(tres)==0 ||input(cuatro)==0 ||input(cinco)==0 ||input(seis)==0 || input(siete)==1){//si se detecta un cambio
            
         if(input(uno)==0){//fue la puerta del conductor
            delay_ms(4000);//esperamos 4s
            pregunta_valet://¿se presiona valet?
            if(input(valet)==1){//si se presiono valet
               cambio_estado();
               goto desarmado;
            }
            
            else{//no se presiono valet
               output_high(suno);//encender bocina
               pregunta_rcdesactivar://¿se recibe señal para desactivar?
               if(input(desac)==1){//si
                  cambio_estado();
                  output_low(suno);//apagar bocina
                  goto armado; //ir a armado
               }
               
               else{//no
                  activacion:
                  llamada();
                  activar();//encendemos luces
                  i++; //contamos
                  if(i==10){//si es igual a 10
                     colgar();
                     if(input(uno)==0 || input(dos)==0 ||input(tres)==0 ||input(cuatro)==0 ||input(cinco)==0 ||input(seis)==0 || input(siete)==1){
                        i=0;
                        goto activacion;
                     }
                     
                     else{
                        output_low(suno);
                        i=0; //restauramos contador
                        cambio_estado();
                        goto armado; //ir a armado
                     }
                     
                  }
                  
                  else{//si no es igual a 10
                     switch(y){
                        case 0: goto pregunta_valet;y=1;break;//¿se recibe señal de valet?
                        case 1: goto pregunta_rcdesactivar;y=0;break;//señal de desactivar
                     }             
                  }
               }
              }
         }
       
         else{//no fue la puerta del conductor
            goto pregunta_valet;//vamos a: preguntamos si se presiono valet
         }
       }
      
       else{ //no recibieron cambios
         pregunta_rcdesarmar://¿se recibe señal de desarmado?
         if(input(desar)==1){//si
            cambio_estado();
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
      
      /////////////////////////////////////////PROGRAMA DESARMADO/////////////////////////////////////////////
       for(;;){
         desarmado:
         output_high(scuatro);//activamos el cable de poder
         output_low(suno);
         output_low(sdos);
         output_low(stres);
         output_low(sseis);
         output_low(ssiete);
         
         ///////////////////////////LUZ DE CIELO/////////////////////////////////////////
         if(input(uno)== 0 || input(tres)== 0 || input(cuatro)== 0 || input(cinco)== 0){
            output_high(socho);
         }
         
         else{
            output_low(socho);
         }
         
         ///////////////////////////LUZ DE BAUL///////////////////////////////////////////
         if(input(dos)==0){
            output_high(snueve);
            output_high(socho);
         }
         
         else{
            output_low(snueve);
            output_low(socho);
         }
         
         ///////////////////////////LUZ DE CAPOT//////////////////////////////////////////
         if(input(seis)==0){
            output_high(sdiez);
            output_high(socho);
         }
         
         else{
            output_low(sdiez);
            output_low(socho);
         }
         /////////////////////////////////////////////////////////////////////////////////
         
         if(input(nueve)==1){//si boton de proteccion esta activo //////////////////modo proteccion///////////
            modo_interno://modo interno de proteccion              
/*entrada*/ if(input(uno)==0 || input(dos)==0 ||input(tres)==0 ||input(cuatro)==0 ||input(cinco)==0 ||input(seis)==0){
               entrada_activa:
               activar();//activamos la alarma
               if(input(valet)==1){//si valet esta activo
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
                  cambio_estado();
                  goto armado;//armamos la alarma
                  //posible estado de paso a estado armado
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

void llamada(void){
   output_high(scinco);//hacemos la llamada
   delay_ms(20);
   output_low(scinco);
}

void colgar(void){
   output_high(sseis); //cancelamos la llamada
   delay_ms(20);
   output_low(sseis);
}

void cambio_estado(void){
     output_high(sdos);
     output_high(suno);
     delay_ms(200);
     output_low(sdos);
     output_low(suno);
}
