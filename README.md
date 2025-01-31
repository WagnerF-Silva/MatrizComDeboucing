# Matriz de LEDs WS2812 - Contador Numérico

## Descrição
Este projeto utiliza uma matriz de LEDs WS2812 controlada por um Raspberry Pi Pico para exibir números de 0 a 9. A cada pressão dos botões, o número é incrementado ou decrementado. O sistema inclui um LED vermelho piscante e um LED verde, que pode ser utilizado para sinalizações futuras.

## Funcionalidades
- Exibição de números de 0 a 9 em uma matriz de LEDs 5x5.
- Incremento do número ao pressionar o botão A.
- Decremento do número ao pressionar o botão B.
- Controle de debounce para evitar leituras incorretas dos botões.
- LED vermelho piscante para indicação de funcionamento.

## Componentes Utilizados
- **Raspberry Pi Pico**
- **Matriz de LEDs WS2812 (5x5 - 25 píxeis)**
- **2 Botões**
- **LEDs RGB (vermelho e verde)**

## Configuração dos Pinos
| Componente      | Pino Pico |
|----------------|-----------|
| LED Vermelho   | GPIO 13   |
| LED Verde      | GPIO 11   |
| Botão A       | GPIO 5    |
| Botão B       | GPIO 6    |
| Matriz WS2812  | GPIO 7    |

## Principais Funções

### `put_pixel(uint32_t pixel_grb)`
Envia um comando para a matriz WS2812 acionar um LED com a cor definida no formato GRB.

### `urgb_u32(uint8_t r, uint8_t g, uint8_t b)`
Converte valores RGB para o formato correto da matriz WS2812.

### `update_led_matrix()`
Atualiza a matriz de LEDs para exibir o número armazenado na variável `num`, com base nos padrões predefinidos.

### `debounce()`
Evita leituras erradas dos botões filtrando toques repetidos em um curto espaço de tempo.

### `button_callback(uint gpio, uint32_t events)`
Interrupção para detectar quando um botão é pressionado. Incrementa ou decrementa o valor de `num` e atualiza a matriz de LEDs.

### `setup_gpio()`
Inicializa os pinos GPIO para os LEDs e botões, habilitando as interrupções nos botões.

## Como Utilizar
1. Carregar o código no Raspberry Pi Pico.
2. Pressionar o botão A para incrementar o número.
3. Pressionar o botão B para decrementar o número.
4. Observar a matriz de LEDs exibindo os números corretamente.

## Possíveis Melhorias
- Implementar um display de 7 segmentos alternativo.
- Adicionar mais efeitos de iluminação para transições entre os números.
- Utilizar um LED RGB para indicar estados específicos.

Este projeto pode ser ampliado para outras aplicações educacionais e de interface interativa.

