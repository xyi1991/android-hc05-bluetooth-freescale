# Resumen #
Este proyecto explica como conectarse al módulo bluetooth HC-05 con plataforma Android y cambiar la frecuencia de destello de un LED. El módulo bluetooth esta manejado por un microcontrolador freescale MCF51JM128 de 32 bits, 128 K de flash y 16 K de ram. Una vez que se empareja la aplicación Android con el HC-05, podemos cambiar la frecuencia de destello de un led conectado al microcontrolador.

# Módulo Bluetooth HC-05 #
Se utiliza el módulo Bluetooth HC-05. Este módulo tiene la antena incorporada, de muy bajas dimensiones, clase 2 (rango de transmisión hasta 10 mts.). Con respecto al método de conexión del módulo Bluetooth, utiliza el protocolo SPP (Serial Port Profile), para enlazarse con otro módulo Bluetooth que también utilice este protocolo, el cual establece una conexión serie virtual RS232 inalámbrica transparente.

![http://img.alibaba.com/img/pb/603/412/371/371412603_147.jpg](http://img.alibaba.com/img/pb/603/412/371/371412603_147.jpg)

http://img.alibaba.com/img/pb/900/075/379/379075900_395.JPG

# Microcontrolador Freescale MCF51JM128 #
Se utiliza un microcontrolador Flexis Coldfire V1 MCF51JM128 de 32 bits de la empresa Freescale con 128 KB de memoria flash, 16 KB de memoria ram y 24 MHz de frecuencia de bus. Este microcontrolador está elegido con un sobredimensionamiento posibilitando futuras ampliaciones de los siguientes desarrollos electrónicos. La conexión con el módulo Bluetooth se hace a través del puerto serie del microcontrolador con señales TTL de 3,3 V. Esta misma tensión se utiliza para alimentar al módulo Bluetooh y al microcontrolador flexis. Dos leds indican el estado del módulo Bluetooh, controlados por el firmware de este. El programa del microcontrolador incluye la inicialización del reloj del sistema a 24 MHz, la inicialización de la interfaz I/O, la inicialización del puerto SCI (Serial Communications Interface) a 115200 bps, control de tiempo, la inicialización del módulo Bluetooth HC-05 a 115200 bps, consultas al módulo Bluetooth, la creación de la conexión, la transmisión de datos y desconexión.

http://media.digikey.com/photos/Freescale%20Photos/MCF51JM128VQH.JPG