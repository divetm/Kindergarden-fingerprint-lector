# Lector de huellas para guarderías
Proyecto IoT de WND


1. [Introducción](#introducción)
2. [Código](#código)
3. [Losant](#losant)

Introducción
----

Este proyecto tiene como objetivo desarrollar un prototipo para guarderías que quieran tener un registro de los niños que entran cada día en su establecimiento.
El prototipo consta de un lector de huellas (Adafruit Fingerprint sensor #751), un detector de presencia de dedo (fotoresistencia) y un Devkit 2.0 de NXTIoT.
![devkit](link_hacia_imagen)
Cuando un dedo es detectado el lector se prende. Si el dedo de un niño es identificado, una alerta de entrada a la guardería es enviada a Losant.
Si el dedo identificado es el del administrador de la guardería, dos opciones son posibles: 1. Registrar una nueva huella o 2. Borrar una huella existente. Una alerta se envia a Sigfox en ambos casos.
En Losant se creo un dashboard desde el cual se pueden ver todo el historial de las alertas.

Código
----

Ver el código completo y comentado [aqui](https://github.com/divetm/Lector-de-huellas-para-guarder-as/blob/master/lector_de_huellas_para_guarderia.ino)

Losant
----

Para crear el dashboard en Losant, primero tenemos que crear un Webhook con las características siguientes:
![webhook](link)

Luego hay que crear un callback en el backend de Sigfox con las características siguientes:
![callback](callback)

    {
      "device" : "{device}",
      "data" : "{data}",
      "time" : "{time}"
    }
    
Y creamos un Device en Losant con las características siguientes:
![device](device)

El paso siguiente es construir el workflow en Losant:
![workflow](workflow)

Para el bloco "Webhook", sólo se necesita seleccionar el webhook que creamos más arriba:
![webhook_in_worflow](webhook_in_worflow)

En el bloco "Function", este es el código que utilizamos:

    payload.data.body.fingerprint_id = parseInt(payload.data.body.data.substring(0,2));
    if (payload.data.body.fingerprint_id===1){
      payload.data.body.name = "Admin";
    }
    else if (payload.data.body.fingerprint_id===2){
      payload.data.body.name = "Pepito";
    }
    else if (payload.data.body.fingerprint_id===3){
      payload.data.body.name = "Juan";
    }
    else if (payload.data.body.fingerprint_id===4){
      payload.data.body.name = "Jose";
    }
    payload.data.body.admin_choice = parseInt(payload.data.body.data.substring(2,4));
    payload.data.body.admin_id_modified = parseInt(payload.data.body.data.substring(4,6));
    if(payload.data.body.admin_choice == 1){
      payload.data.body.alert = "Added a new fingerprint. ID = "+payload.data.body.admin_id_modified;
    }
    else if(payload.data.body.admin_choice == 2){
      payload.data.body.alert = "Erased a fingerprint. ID = "+payload.data.body.admin_id_modified;
    }
    else if(payload.data.body.admin_choice == 0){
      payload.data.body.alert = "Entry";
    }
    
En el bloco "Device State", sólo se necesita seleccionar el device que creamos previamente y luego especificar que campos del "payload.data.body" corresponden a los atributos del device:

![device_state_1](device_state_1)
![device_state_2](device_state_2)

No hay que modificar nada en el bloco "Debug" y porfin en el bloco "Webhook Reply" sólo complete los campos "Response Code Template" y "Reply Body" como sigue:

![webhook_reply](webhook_reply)

Darle click a "Save & Deploy"
Por último, tenemos que crear el dashboard. Para eso, en la sección Dashboards de Losant, creamos el Dashboard "Lector de huellas para la guarderia":

![dashboard](dashboard)

Le agregamos el bloco "Device State Table":

![device_state_table](device_state_table)
