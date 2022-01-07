/**
 * @file       ControleNível_Arduino.c
 * @author     Diego Anestor Coutinho
 * @date       Jan 2022
 * 
 * O algoritmo é para resolução do execício 2, relatório 5 de 2021_1
 * Tema: Dispositivos de Entrada e Saída (GPIO) – Exercício: Nível
 * Contato: diego.anestor@gec.inatel.br ou +5535999631997.
*/

/* define as saídas -> válvula e indicadores */
#define CHEIO_ON PORTD = PORTD | 0b10000000
#define CHEIO_OFF PORTD = PORTD & ~(0b10000000)
#define ENCHENDO_ON PORTD = PORTD | 0b01000000
#define ENCHENDO_OFF PORTD = PORTD & ~(0b01000000)
#define VALV_ON PORTD = PORTD | 0b00100000
#define VALV_OFF PORTD = PORTD & ~(0b00100000)

int main()
{
    /* define as entradas -> sensores S.MIN e S.MAX */
    bool min = 0;
    bool max = 0;

    /* Realiza o SETUP inicial */
    int estado = 0;
    DDRD = DDRD | 0b11100000; // PINOS 7, 6, 5 -> OUTPUTS

    for (;;)
    {
        /* realiza a leitura dos pinos 3 e 4 */
        min = (PIND & 0b00001000);
        max = (PIND & 0b00010000);

        /* Navega para o estado atual da máquina */
        /* Caso permitido, realiza a operação, altera o estado e volta ao início */
        switch (estado)
        {
        case 0:
            CHEIO_ON;
            ENCHENDO_OFF;
            VALV_OFF;
            if (max == 0)
                estado++;

            break;

        case 1:
            CHEIO_OFF;
            ENCHENDO_OFF;
            VALV_OFF;
            if (min == 0)
                estado++;

            break;

        case 2:
            CHEIO_OFF;
            ENCHENDO_ON;
            VALV_ON;
            if (max == 1)
                estado = 0;

            break;

        default:
            break;
        }
    }
}
