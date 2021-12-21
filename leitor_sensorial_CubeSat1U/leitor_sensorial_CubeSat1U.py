import serial
import pandas as pd

# configuração para comunicação serial
ser = serial.Serial('COM4', 9600, timeout=1)
ser.flushInput()
ser.flush()

# Tenta fazer a leitura e joga fora as duas primeiras *lixo
try:
    ser_bytes = ser.readline()
except:
    print("Erro, não foi possível realizar a leitura do sensor!")
    exit()

ser_list = []
# execução principal
for x in range(200):
    try:
        ser_bytes = ser.readline()

    except:
        print("Parada obrupta, saindo...")
        break

    ser_num = []

    # converte os bytes numa lista de dados
    ser_bytes_vec = ser_bytes.split()
    for byte in ser_bytes_vec:
        ser_num.append(float(byte))

    ser_list.append(ser_num)

ser.close()

# cria pandas DataFrame com os dados
ser_list = pd.DataFrame(ser_list, columns=["luz", "altPre", "tmp"])
ser_list.index.names = ['index']

# salva os valores num arquivo csv
ser_list.to_csv(r'path\flightData.csv')
