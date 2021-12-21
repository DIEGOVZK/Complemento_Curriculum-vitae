/**
 * @file       BitField_Atuador.h
 * @author     Diego Anestor Coutinho
 * @license    Internal Use Only
 * @copyright  Copyright (c) 2021 BitField
 * @date       Ago 2021
 */

#define UP_THRESHOLD_HUMIDITY (int)500
#define UP_IRRIGATION_VALUE (int)600
#define LO_IRRIGATION_VALUE (int)400
#define UP_THRESHOLD_EC (int)500
#define LO_THRESHOLD_EC (int)600
#define EC_NORMAL_VALUE (int)550
#define EC_ADD_REGULATION (int)10
#define PH_NORMAL_VALUE (int)700
#define PH_VARIATION_VALUE (int)50
#define WATER_NORMAL_VALUE (int)700
#define MAXIMUN_WATERPUMP_TIME (int)600000

#define INTERRUPT_SENSOR (byte)0
#define INTERRUPT_PIN (char)2
#define GH_SENSOR_PIN (char)A1
#define EC_SENSOR_PIN (char)A2
#define PH_SENSOR_PIN (char)A0
#define IRRIGATION_VALVE_PIN (char)4
#define WATER_PUMP_PIN (char)5
#define WATER_LEVEL_SENSOR_PIN (char)6
#define WATER_REPOSITION_PUMP_PIN (char)8
#define EC_REPOSITION_VALVE_PIN (char)9


int pulseCount = 0;

// <---------------------------------------> Sequências de atuação <--------------------------------------->

/** @fn pulseCounter()
 * @brief função chamada pelo interrupt pin
 */
void pulseCounter()
{
    pulseCount++;
}
    
/** @fn irrigation_Sequence(char valve_pin)
 * @brief Inicia o procedimento de irrigação semi-hidropônica
 * @param valve_pin Pino do relay da valvula
 * @param pump_pin Pino do relay da bomba de água
 */
void irrigation_Sequence(char valve_pin, char pump_pin)
{
    digitalWrite(valve_pin, 1);
    digitalWrite(pump_pin, 1);
    attachInterrupt(INTERRUPT_SENSOR, pulseCounter, FALLING);
    
    while(gh_value < UP_THRESHOLD_HUMIDITY)
    {
        gh_value = analogMedia(GH_SENSOR_PIN, 10);
        attachInterrupt(INTERRUPT_SENSOR, pulseCounter, FALLING);
    }

    digitalWrite(valve_pin, 0);
    digitalWrite(pump_pin, 0);
    sendValues();
    pulseCount = 0;
}

/** @fn WaterRepo_Sequence(char valve_pin)
 * @brief Inicia o procedimento de reposição da água
 * @param valve_pin Pino do relay da valvula
 * @param pump_pin Pino do relay da bomba de água
 */
bool WaterRepo_Sequence(char pump_pin)
{
    int watelValue = WaterLevel_sensor(WATER_LEVEL_SENSOR_PIN);

    if(watelValue < WATER_NORMAL_VALUE)
    {
        digitalWrite(pump_pin, 1);
        do
        {
            watelValue = WaterLevel_sensor(WATER_LEVEL_SENSOR_PIN);
        } while (watelValue < WATER_NORMAL_VALUE);

        digitalWrite(pump_pin, 0);   
        return 1; 
    }
    else
        return 0;
}

/** @fn FertilRepo_Sequence(char valve_pin)
 * @brief Inicia o procedimento de reposição e regulagem dos sensores
 * @param valve_pin Pino do relay da valvula
 * @param pump_pin Pino do relay da bomba de água
 */
bool FertilRepo_Sequence(char valve_pin, char pump_pin)
{
    int pour_amount = 0;
    ec_value = analogMedia(EC_SENSOR_PIN, 20);
    if(ec_value > LO_THRESHOLD_EC)
    {
        digitalWrite(valve_pin, 1);
        digitalWrite(pump_pin, 0);
        pour_amount = (ec_value - EC_NORMAL_VALUE) * EC_ADD_REGULATION;
        do
        {
            attachInterrupt(INTERRUPT_SENSOR, pulseCounter, FALLING);
        } while (pulseCount < pour_amount);

        digitalWrite(valve_pin, 0);
        digitalWrite(pump_pin, 1);
        sendValues();
        pulseCount = 0;
        return 1;
    }
    else if(ec_value < UP_THRESHOLD_EC)
    {
        if(WaterRepo_Sequence(WATER_PUMP_PIN))
            return 1;
        else
            return 0;
    }
    else
    return 1;
}
