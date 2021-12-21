/**
 * @file       BitField_Sensorial.h
 * @author     Diego Anestor Coutinho
 * @license    Internal Use Only
 * @copyright  Copyright (c) 2021 BitField
 * @date       Ago 2021
 */

#include <Wire.h>


// Endereços de uso do sensor DH1750
#define BH1750_POWER_ON   (byte)0b00000001
#define BH1750_RESET      (byte)0b00000111
#define BH1750_ONE_HIHES  (byte)0b00100000
#define BH1750_ONE_LOHES  (byte)0b00010011
#define BH1750_I2C_ADRESS (byte)0x23


// <---------------------------------------> Sensores Analógicos <--------------------------------------->

/** @fn analogMedia(char pin, uint8_t samples)
 * @brief faz a leitura do sinal analógico nº vezes e retorna a média.
 * @param pin posição física do pino conectado.
 * @param samples Nº de amostras a serem colhidas.
 * @return valor adquirido no pino. 
 */
int analogMedia(char pin, uint8_t samples)
{
    int SV = 0;

    // faz a leitura do valor n vezes:
    for (uint8_t i = 0; i < samples; i++)
        SV += analogRead(pin) / samples;

    return SV;
}


// <---------------------------------------> Sensores Digitais <--------------------------------------->


/** @fn BH1750_configure(bool resolution)
 * @brief código de configuração do sensor.
 * @param resolution resolução de leitura.
 * 1 - Alta Resolução.
 * 0 - Baixa Resolução.
 */
void BH1750_configure(bool resolution)
{
    // POWER UP // RESET VALUE // SEND OPR MODE
    Wire.beginTransmission(BH1750_I2C_ADRESS);
    Wire.write(BH1750_POWER_ON);  
    Wire.write(BH1750_RESET); 
    if(resolution)    
        Wire.write(BH1750_ONE_HIHES); 
    else
        Wire.write(BH1750_ONE_LOHES); 
    Wire.endTransmission(false);
}

/** @fn BH1750_sensor()
 * @brief Realisa a leitura do sensor BH1750. (UTILIZA DELAY()).
 * @param resolution 0 - Baixa resolução, 1 - Alta resolução
 * @return Nah - Atualiza a variável Light_signal.
 */
void BH1750_sensor(bool resolution)
{
    BH1750_configure(resolution);

    // Tempo de leitura 
    if(resolution)
        delay(120);
    else
        delay(16);
    
    // Lê 2 registros a partir do BH1750_DATA_ADRESS
    Wire.requestFrom((uint8_t)BH1750_I2C_ADRESS, (uint8_t)2);
    if (2 <= Wire.available())
    {
        uint16_t aux = 0;
        aux  = (int16_t)Wire.read(); aux <<= 8;
        aux |= (int16_t)Wire.read();
        lv_value = aux;
    }
}


/** @fn WaterLevel_sensor(char pin)
 * @brief faz a leitura do sensor de nível de água.
 * @param pin posição física do pino conectado.
 * @return valor do nivel de água no reservatório. 
 */
int WaterLevel_sensor(char pin)
{
    
} 


