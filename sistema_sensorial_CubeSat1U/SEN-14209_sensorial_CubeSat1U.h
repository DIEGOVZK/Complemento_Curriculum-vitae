/**
 * @file       AlphaCounter.h
 * @author     Diego Anestor Coutinho
 * @license    Closed - Internal Use Only
 * @copyright  Copyright (c) 2021 Inatel Cubesat Design Team
 * @date       Dec 2021
 * 
 * O algorítmo é parte do sistema sensorial do time AlphaSat. 
 * O direito de uso é interno, e distribuível entre os integrantes do time.
 * Contato: diego.anestor@gec.inatel.br ou +5535999631997.
*/

volatile uint32_t pulseCount = 0;
void IRAM_ATTR pulseHandler()
{
  pulseCount++;
}

/** @fn setupGeiger(char interruptPin)
 * @brief configura as necessidades do SEN-14209
 * @param interruptPin Pino ligado a porta SIG do sensor
 */
void setupGeiger(char interruptPin)
{
  // PullUp pois o sinal é um "PullDown on Pulse"
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pulseHandler, FALLING);
  interrupts();
}

/** @fn ppm(void)
 * @return quantidade de pulsos desde a última execução
 */
uint32_t ppm(void)
{
  // Evita pulsos durante cálculo
  noInterrupts();
  uint32_t x = pulseCount;
  pulseCount = 0;
  interrupts();

  // Redundância anti-bitFlips
  return (x > 0) ? x : 0;
}
