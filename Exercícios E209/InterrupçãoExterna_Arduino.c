/**
 * @file       InterrupçãoExterna_Arduino.c
 * @author     Diego Anestor Coutinho
 * @date       Jan 2022
 * 
 * O algoritmo é para resolução do execício 3, relatório 7 de 2021_1
 * Tema: Interrupção Externa e "sleep modes" – Exercício: Interrupção Externa
 * Contato: diego.anestor@gec.inatel.br ou +5535999631997.
 
 * ---> CIRCUITO EM: https://www.tinkercad.com/things/ftL9kai7N9I-e209-rel-7-exemplo-3
 
*/

// Na interrupção INT0
ISR(INT0_vect)
{
    PORTD = PORTD | 0b01000000;    // Ativa PD6
    _delay_ms(200);                // Espera 200ms
    PORTD = PORTD & ~(0b01000000); // Desatvia PD6
}

// Na interrupção INT1
ISR(INT1_vect)
{
    // Alterna o BIT_0 mudando INT0 -> ON/OFF
    EIMSK ^= 0b00000001;
}

int main(void)
{

    // Habilita pull-up no PD3
    PORTD = 0b00001000;

    // PINOS 7, 6 -> OUTPUTS
    DDRD = DDRD | 0b11000000;

    // INT0 -> RISING_EDGE
    // INT1 -> FALLING_EDGE
    EICRA = 0b00001011;

    // Habilita INT nos pinos PD2 e PD3
    EIMSK = 0b00000011;

    // Habilita Interrupções globais
    sei();

    for (;;)
    {
        PORTD ^= 0b10000000; // Inverte PD7
        _delay_ms(2000);     // Espera 2s
    }
}