# Lector de huellas para guarderías
Proyecto IoT de WND


1. [Introducción](#introducción)
2. [Código](#código)
3. [Backend](#backend)
4. [Losant](#losant)

Introducción
----

Este proyecto tiene como objetivo desarrollar un prototipo para guarderías que quieran tener un registro de los niños que entran cada día en su establecimiento.
El prototipo consta de un lector de huellas y un detector de presencia de dedo (fotoresistencia).
![imagen](link_hacia_imagen)
Cuando un dedo es detectado el lector se prende. Si el dedo de un niño es identificado, una alerta de entrada a la guardería es enviada a Losant.
Si el dedo identificado es el del administrador de la guardería, dos opciones son posibles: 1. Registrar una nueva huella o 2. Borrar una huella existente. Una alerta se envia a Sigfox en ambos casos.
En Losant se creo un dashboard desde el cual se pueden ver todo el historial de las alertas.

Código
----

Ver el [aqui]()
