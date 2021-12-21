/*
 * Este pedaço de código foi escrito por Diego Anestor Coutinho.
 * diego.anestor@gec.inatel.br ou +55(35)99963-1997
 * 
 * O algorítmo é parte do computador de bordo do CanSat criado por: Inatel CubeSat Design Team.
 * O direito de uso é interno, e distribuível entre os integrantes do grupo.
 * 
 * Alguns testes, quando possível, foram realizados no TinkerCad:
 * ( https://www.tinkercad.com/things/3J9V8QBASE5-testes-des-sensores-cansat/editel )
 */

#include <Wire.h>

#define MPL3115A2 0x60 // I2C endereço do MPL3115A2

float alturaPressao = 0;
float temperatura = 0;
byte oldState = 0;

/** \fn ambiente(bool altPre, bool mode)
 * \brief leitura da Altura ou pressão e temperatura do sensor MPL3115A2,
 * Altura e pressão são alocados no mesmo endereço, é preciso escolher em antemão.
 * \param altPre (1 = Altura em metros) (0 = Pressão em Pascals).
 * \param active (1 = Leitura Peródica) (0 = STANDBY)
 * \param samples Nº de amostras a serem colhidas (8ms por amostra).
 * \return 1 = Leitura realizada com sucesso. 0 = Erro na leitura ou modo STANDBY.
 */
bool ambiente(bool altPre, bool active, uint8_t samples)
{
    byte operationMode = altPre << 7 | 0 << 6 | 1 << 5 | 0 << 4 | 1 << 3 | 0 << 2 | 0 << 1 | active;

    if (!active)
    {
        // se active for 0, entra em modo de espera
        Wire.beginTransmission(MPL3115A2);
        Wire.write(0x26);
        Wire.write(0x00);
        Wire.endTransmission();
        oldState = operationMode;

        return 0;
    }
    else
    {
        // se o modo de operação for ATIVO, inicia o procedimento padrão
        int32_t altPress = 0;
        int16_t tempValue = 0; 
        alturaPressao = 0;
        temperatura = 0;   

        if (oldState != operationMode)
        {
            // CTRL_REG1
            Wire.beginTransmission(MPL3115A2);
            Wire.write(0x26);
            Wire.write(operationMode);
            Wire.endTransmission();
            oldState = operationMode;
        }

        for (uint8_t i = 0; i < samples; i++)
        {
            // Tempo de leitura 
            delay(6);

            // Começando no registro 0x01
            Wire.beginTransmission(MPL3115A2);
            Wire.write(0x01);
            Wire.endTransmission(false);

            // Lê 5 registros a partir do 0x01
            Wire.requestFrom((uint8_t)MPL3115A2, (uint8_t)5);
            if (5 <= Wire.available())
            {
                if (altPre) // se estiver lendo pressão
                {
                    altPress  = (uint32_t)Wire.read() << 24;
                    altPress |= (uint32_t)Wire.read() << 16;
                    altPress |= (uint32_t)Wire.read() << 8;
                }
                else // se estiver lendo altitude
                {
                    altPress  = (uint32_t)Wire.read(); altPress <<= 8;
                    altPress |= (uint32_t)Wire.read(); altPress <<= 8;
                    altPress |= (uint32_t)Wire.read(); altPress >>= 4;
                }
                
                // segue lendo registros de temperatura
                tempValue  = (int16_t)Wire.read(); tempValue <<= 8;
                tempValue |= (int16_t)Wire.read(); tempValue >>= 4;

                // converte os valores lidos
                if (altPre)
                    alturaPressao += (altPress / 65536.0) / samples;
                else
                    alturaPressao += (altPress / 4.0) / samples;
                
                if (tempValue & 0x800)
                    tempValue |= 0xF000;

                temperatura += (tempValue / 16.0) / samples;   
            }
            else
                // Retorna zero em caso de erro na leitura
                return 0;
        }

    }
    return 1;
}
