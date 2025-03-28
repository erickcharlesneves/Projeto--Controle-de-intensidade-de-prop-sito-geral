# Projeto: Controle de intensidade de propósito geral - com Joystick🎮 da BitDogLab / Raspberry Pi Pico W

## 📖 Introdução
Este projeto implementa uma interface interativa utilizando a **Raspberry Pi Pico W** e componentes da raspberry com os seguintes componentes:

- **Joystick analógico**
- **Display OLED SSD1306**
- **Buzzer (intensidade sonora)**
- **LED RGB**
  
A intensão é testar e implementar o nosso projeto prático utilizando as bibliotecas do DISPLAY OLED SSD1306 e Joystick analógico, da dupla designada do VIRTUS CC em sistemas embarcados.
Segue bibliotecas dos mesmos: https://github.com/Pwsousa/Sistemas_Embarcados

A movimentação vertical superior do joystick controla a intensidade do sonora do buzzer, de acordo com o movimento no eixo, e a cor do LED RGB sendo de 0 a 25% sem alteração de cor, de 25% a 50% na cor verde, de 50% a 75% na cor amarela e de 75% a 100% na cor vermelha, e quanto maior a porcentagem maior a intensidade sonora. 

Os valores de porcentagem são exibidos no **display OLED** em forma de controle visual, além da barra de porcentagem vertical retangular que acompanha o mesmo eixo Y para esse aumento.  Com isso teremos um sistema embarcado que pode ser utilizado para controlar e verificar a intensidade de diferentes equipamentos, luminosidades e etc. Como exemplo de motores que exigem um controle.

## 📜 **Breve explicação do código do projeto** 

O código básicamente é dividido em módulos:

### 🔊 **Buzzer (PWM)**

O buzzer é controlado por **PWM (modulação por largura de pulso)** na frequência de **4 kHz**.


```c
    void  pwm_init_buzzer(uint pin) {
        gpio_set_function(pin, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(pin);
        pwm_config config = pwm_get_default_config();
        pwm_config_set_clkdiv(&config, (float) clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
        pwm_init(slice_num, &config, true);
  }
```

 

### 🎮 **Leitura do Joystick**

A função **`media_movel_vrx`** faz uma filtragem de média móvel para reduzir ruídos nas leituras do joystick.

```c
   uint16_t  media_movel_vrx(uint16_t novo_valor) {
        soma_vrx -= buffer_vrx[indice_vrx];
        buffer_vrx[indice_vrx] = novo_valor;
        soma_vrx += novo_valor; return soma_vrx / MEDIA_MOVEL_N;
    }
```

### 🟢🟡🔴 **Controle do LED RGB**

Os LEDs mudam de cor de acordo com a posição do joystick verticalmente para cima.

```c
    void  set_led_color(uint red_pin, uint green_pin, uint blue_pin, bool R, bool G, bool B) {
        gpio_put(red_pin, R);
        gpio_put(green_pin, G);
        gpio_put(blue_pin, B);
    }
```

### 📟 **Display OLED (SSD1306)**

Os valores do joystick são exibidos no display:

```c
    ssd_1306_set_cursor(10, 10); 
    ssd_1306_write_string("Valor X:", Font_7x10, black);
```

(para mais detalhes verificar também comentários nos códigos fornecidos)

----------

### 📟 **Interface no Display**

A movimentação vertical superior do joystick controla a intensidade do sonora do buzzer por PWM, a porcentagem é exibida em i, e a cor do LED RGB respectivamente conforme abaixo.

##### Sendo de 0 a 25% sem alteração de cor:

<img src="https://github.com/user-attachments/assets/25dec0a6-a988-4e1c-a203-2ca22d3643f2" width="50%" alt="zerado">

##### De 25% a 50% na cor verde:

<img src="https://github.com/user-attachments/assets/5165b4b8-b4f0-4506-80a1-f576f61da563" width="50%" alt="zerado">

##### De 50% a 75% na cor amarela:

<img src="https://github.com/user-attachments/assets/1e096d9f-c5b2-4228-ac2d-a93c4862d4ee" width="50%" alt="zerado">

##### E de 50% a 100% na cor vermelha:

<img src="https://github.com/user-attachments/assets/c8f394e8-0b32-49ad-b02d-06ed15dd2b4c" width="50%" alt="zerado">

## 🎯 **Conclusão**  

Este projeto demonstra como integrar múltiplos periféricos em uma **Raspberry Pi Pico W**, utilizando **joystick, display OLED, buzzer e LED RGB** para criar uma interface interativa. A abordagem adotada inclui:  

✅ **Leitura analógica do joystick**, com filtragem por média móvel para suavizar os valores.  
✅ **Controle do LED RGB**, alterando cores conforme a posição do joystick.  
✅ **Exibição de dados no display OLED**, permitindo um feedback visual do sistema.  
✅ **Uso de PWM no buzzer**, gerando sons proporcionais ao movimento do joystick.  

O código é modular e bem estruturado, permitindo fácil expansão para novas funcionalidades. 

_______________


## Contribuições:

Caso tenha sugestões ou melhorias, sinta-se à vontade para contribuir! 🚀✨ 
Sinta-se à vontade para levantar novas questões, e abrir pull requests. Considere dar uma estrela e bifurcar este repositório!

Se você tiver alguma dúvida sobre, não hesite em entrar em contato comigo no Gmail: [erick.cassiano@ee.ufcg.edu.br](mailto:erick.cassiano@ee.ufcg.edu.br) ou abrir um problema no GitHub.

## Contributing:

Contributions are always welcomed. Feel free to raise new issues, file new PRs. Consider giving it a star and fork this repo!

If you have any question about this opinionated list, do not hesitate to contact me on Gmail: [erick.cassiano@ee.ufcg.edu.br](mailto:erick.cassiano@ee.ufcg.edu.br) or open an issue on GitHub.

[![Linkedin](https://img.shields.io/badge/linkedin-%230077B5.svg?&style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/erick-charles-neves/)

