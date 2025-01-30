<h1 align="center">EmbarcaTech - Atividade Interrupções</h1>

Objetivos:

• Compreender o funcionamento e a aplicação de interrupções em microcontroladores.
• Identificar e corrigir o fenômeno do bouncing em botões por meio de debouncing via software.
• Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
• Fixar o estudo do uso de resistores de pull-up internos em botões de acionamento.
• Desenvolver um projeto funcional que combine hardware e software.

Materiais utilizados:

Neste projeto, são utilizados os seguintes componentes conectados à placa BitDogLab:
1) Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7.
2) LED RGB, com os pinos conectados às GPIOs (11, 12 e 13).
3) Botão A conectado à GPIO 5.
4) Botão B conectado à GPIO 6.

Lista de requisitos:

- As teclas de 0 a 9 do teclado matricial, caso acionadas, o sistema deve executar algum tipo de animação na matriz de LEDs 5x5.
- Cada animação deve possuir, no mínimo, 5 frames (imagens diferentes) e o FPS (quantidade de quadros por segundo) também deve ser definido pela equipe de desenvolvimento. 
- Caso a tecla A seja acionada, todos os LEDs deverão ser desligados. 
- Caso a tecla B seja acionada, todos os LEDs deverão ser ligados na cor azul, no nível de intensidade de 100% da luminosidade máxima.
- Caso a tecla C seja acionada, todos os LEDs deverão ser ligados na cor vermelha, no nível de intensidade de 80% da luminosidade máxima.
- Caso a tecla D seja acionada, todos os LEDs deverão ser ligados na cor verde, no nível de intensidade de 50% da luminosidade máxima.
- Caso a tecla # seja acionada, todos os LEDs deverão ser ligados na cor branca, no nível de intensidade de 20% da luminosidade máxima.
- Especificação opcional: gerar sinal sonoro para uma das animações usando o componente buzzer. 
- Especificação opcional: Caso a tecla * seja acionada, o Raspberry Pi Pico W deve sair do modo de execução e habilitar o modo de gravação via software (reboot).

Tecnologias:

1. Git e Github;
2. VScode;
3. Linguagem C;
4. Simulador Wokwi;

Funcionalidades Implementadas:

Desenvolvedor:
 
<table>
  <tr>
    <td align="center"> <sub><b> Luigi Manzoli Almeida Lopez </b></td>
    </tr>
</table>

Demonstração: 
- 
