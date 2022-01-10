/**
 * @file       sistema_sensorial_CubeSat1U.ino
 * @author     Diego Anestor Coutinho
 * @license    Closed - Internal Use Only
 * @copyright  Copyright (c) 2021 Inatel Cubesat Design Team
 * @date       Abr 2021
 * 
 * O algorítmo é parte do computador de bordo do CubeSat criado por: Inatel CubeSat Design Team.
 * O direito de uso é interno, e distribuível entre os integrantes do grupo.
 * 
 * Este documento é o código de leitura dos sensores magnetômetro - HMC5883 e o acelerômetro de 6 eixos - MPU6050.
 */

#include <Wire.h>

#define HmcAddress 0x1E // I2C endereço do HMC5883
#define MpuAddress 0x68 // I2C endereço do MPU6050

//Definição das variáveis necessárias para os dados
int16_t gyroX, gyroY, gyroZ, temp;
int16_t magX, magY, magZ;

void setup(){
  // Inicializa Serial com o Monitor Serial
  Serial.begin(9600);
  Wire.begin();

  // Tirar Bypass Mode do MPU
  Wire.beginTransmission(MpuAddress);
  Wire.write(0x37); // INT_PIN_CFG >>
  Wire.write(0x02); // FSYNC_INT_EN 
  Wire.endTransmission();

  Wire.beginTransmission(MpuAddress);
  Wire.write(0x6A); // USER_CTRL >> 
  Wire.write(0x00); // SIG_COND_RESET
  Wire.endTransmission();

  // Desabilita Sleep Mode no MPU
  Wire.beginTransmission(MpuAddress);
  Wire.write(0x6B); // PWR_MGMT_1 >>
  Wire.write(0x00); // DEVICE_RESET (normal state)
  Wire.endTransmission();

  // Coloca o HMC5883 no modo de operação correta
  Wire.beginTransmission(HmcAddress);
  Wire.write(0x00); // Register A >>
  Wire.write(0x54); // 'média de 4 leituras, 30hz taxa de leitura, normal mode'
  Wire.endTransmission();

  Wire.beginTransmission(HmcAddress);
  Wire.write(0x01); // Register B >>
  Wire.write(0x00); // 'ganho em ±0.88 Ga'
  Wire.endTransmission();

  Wire.beginTransmission(HmcAddress);
  Wire.write(0x02); // Register Mode >>
  Wire.write(0x00); // Seta em 'modo de medição contínua'
  Wire.endTransmission();
}

void loop(){
	
  // ---------- Leitura do sensor Magnetômetro ----------
 
  // Começando no registro 3 
  Wire.beginTransmission(HmcAddress);
  Wire.write(0x03);
  Wire.endTransmission();
  
  delay(50); // espera 50ms para garantira medição
  
  // 3*2 registros a partir do 3
  Wire.requestFrom(HmcAddress, 6);

  // Se disponível, procede com a leitura
  if (6 <= Wire.available()) 
  {
    magX = Wire.read() << 8; 
    magX |= Wire.read();     
    magZ = Wire.read() << 8; 
    magZ |= Wire.read();     
    magY = Wire.read() << 8; 
    magY |= Wire.read();     
  }
  else
    Serial.println("Erro, sem dados no MagSensor... ");
  
  // ---------- Leitura do sensor Giroscópio ----------
  
  // Começando no registro 41
  Wire.beginTransmission(MpuAddress);
  Wire.write(0x41);
  Wire.endTransmission();
  
  // 4*2 registros a partir do 41
  Wire.requestFrom(MpuAddress, 8);
  delay(20); // espera 10ms para garantira medição
  
  // Se disponível, procede com a leitura dos dados
  if (8 <= Wire.available()) 
  {
    temp = Wire.read() << 8;
    temp |= Wire.read();    
    gyroX = Wire.read() << 8;
    gyroX |= Wire.read();    
    gyroY = Wire.read() << 8;
    gyroY |= Wire.read();    
	  gyroZ = Wire.read() << 8; 
    gyroZ |= Wire.read();     
  }
  else
    Serial.println("Erro, sem dados no GyroSensor... ");
  
  // ---------- Saída de Dados ----------

  // Mostra dos dados lidos no monitor Serial
  Serial.print(magX);
  Serial.print(" ");
  Serial.print(magY);
  Serial.print(" ");
  Serial.print(magZ);
  
  Serial.print(" ");
  Serial.print(gyroX);
  Serial.print(" ");
  Serial.print(gyroY);
  Serial.print(" ");
  Serial.print(gyroZ);
  Serial.print(" ");
  Serial.println(temp);
  
  delay(30);
}
