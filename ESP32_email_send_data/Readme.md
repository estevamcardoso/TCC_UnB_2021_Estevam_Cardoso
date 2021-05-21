# Código para efetuar a amostragem dos sinais eletromiográficos 

### Será necessário
1. VSCode 
1. Platform IO
1. ESP32 DevKit v1 DOIT 
1. USB 
1. Dispositivo projetado

Caso não tenha o dispositivo, pode usar qualquer sensor que quiser amostrar e enviar os dados por email.

As duas bibliotecas necessárias estão incluídas no arquivo platformio.ini que contém as configurações de
    placa utilizada, velocidade de clock, velocidade de comunicação Serial, etc...

Na pasta include estão os módulos utilizados pelo código, podendo modificar o que desejar.
O arquivo var_handler.h contém as variáveis mais importantes para o processo de aquisição de dados
e persistência, como a taxa de amostragem, tamanho dos arquivos salvos e quantos enviar por email.

Caso queira modificar os arquivos para sua utilização, fique a vontade.