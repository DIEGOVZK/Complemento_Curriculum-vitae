/*
 * Este pedaço de código foi escrito por Diego Anestor Coutinho.
 * diego.anestor@gec.inatel.br ou +55(35)99963-1997
 * 
 * O algorítmo é parte do computador de bordo do CanSat criado por: Inatel CubeSat Design Team.
 * O direito de uso é interno, e distribuível entre os integrantes do grupo.
 * 
 * Este algorítmo é responsável pela obtenção de dados dos diferentes sensores a bordo do CanSat,
 * incluindo mas não não limitado à: LDR, MPL3115A2.
 */

#include "DDCA_sensorReading.h"

int sensorValue = 0;
bool as_s = 1;

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    pinMode(A0, INPUT);
}

void loop()
{
    // recebendo dados das funções
    sensorValue = analogMedian(A0, 5); 
    as_s = ambiente(1, 1, 5);
    
    // ambiente retorna erro se leitura falhar
    if (!as_s)
        Serial.println("erro! ");
    else
    {
        // saída de amostra de dados
        Serial.print(sensorValue);
        Serial.print(' ');
        Serial.print(alturaPressao);
        Serial.print(' ');
        Serial.println(temperatura);
    }
    
    delay(10);
}
