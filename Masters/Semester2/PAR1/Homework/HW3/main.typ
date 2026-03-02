#import "@preview/grape-suite:3.1.0": exercise
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 1",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
    Presentamos un manual paso a paso del acceso al equipo de supercómputo, y
    ejemplos de ejecución dentro del mismo, usando OpenMPI.

  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Manual técnico paso a paso
A continuación, presentamos una guía paso a paso para acceder y ejecutar programas en
el equipo de supercómputo de Guanajuato: El Insurgente.
== Acceso mediante SSH
Primero debemos realizar una conexión al equipo de cómputo mediante Secure Shell (SSH).
En Linux, abrimos una terminal y ejecutamos el siguiente comando:
#figure(
  ```sh
  ssh -p <port> <username>@<host>
  ```,
  caption: [Formato de comando para conexión mediante ssh.],
)
donde `port`, `username` y `host` son proporcionados personalmente por medio de
un correo en respuesta a la solicitud de acceso al equipo de supercómputo. En el caso
de El Insurgente, tenemos 2 alternativas. Si deseamos conectarnos desde la red del CIMAT,

#figure(
  ```sh
  ssh -p 22 est_posgrado_pedro.llerenas@el-insurgente.cimat.mx
  ```,
  caption: [Ejemplo de conexión desde red del CIMAT.],
)

Si deseamos conectarnos desde una red externa, debemos utilizar un puerto y host distintos:

#figure(
  ```sh
  ssh -p 2284 est_posgrado_pedro.llerenas@148.207.185.31
  ```,
  caption: [Ejemplo de conexión desde red externa.],
)
Una vez ejecutado el comando, nos aparecerá en la terminal una interfaz similar
a la siguiente:
#figure(
  ```
     ╦  ┌─┐┌┐ ┌─┐┬─┐┌─┐┌┬┐┌─┐┬─┐┬┌─┐
     ║  ├─┤├┴┐│ │├┬┘├─┤ │ │ │├┬┘││ │
     ╩═╝┴ ┴└─┘└─┘┴└─┴ ┴ ┴ └─┘┴└─┴└─┘
  ╔═╗┬ ┬┌─┐┌─┐┬─┐╔═╗┌─┐┌┬┐┌─┐┬ ┬┌┬┐┌─┐
  ╚═╗│ │├─┘├┤ ├┬┘║  │ ││││├─┘│ │ │ │ │
  ╚═╝└─┘┴  └─┘┴└─╚═╝└─┘┴ ┴┴  └─┘ ┴ └─┘
  ╔═╗╦╔╦╗╔═╗╔╦╗  ╔═╗╔═╗       ╔═╗╔╦╗╔═╗
  ║  ║║║║╠═╣ ║   ╠═╣║    ───  ║ ╦ ║ ║ ║
  ╚═╝╩╩ ╩╩ ╩ ╩   ╩ ╩╚═╝       ╚═╝ ╩ ╚═╝

  Este es el punto de entrada para acceder a diferentes recursos de supercómputo del CIMAT.
  ```,
  caption: [Mensaje de bienvenida cuando nos conectamos correctamente mediante ssh.],
)
Inicialmente, nos encontraremos en nuestro directorio personal.
#figure(
  ```
  est_posgrado_pedro.llerenas@el-insurgente:~$
  ```,
  caption: [Directorio en el que nos encontramos al realizar una conexión exitosa.],
)
== Creación de archivos
En nuestro directorio, podemos crear archivos mediante `touch`. Por ejemplo,
#figure(
  ```sh
  touch HelloWorldMPI_Ins.cpp
  ```,
  caption: [Comando para crear un archivo.],
)
Una vez creado, podemos editarlo mediante un editor de texto. En este caso,
usaremos `vim`.
#figure(
  ```sh
  vim HelloWorldMPI_Ins.cpp
  ```,
  caption: [Comando para editar el archivo `HelloWorldMPI_Ins.cpp` mediante `vim`.],
)
Una vez ejecutado, podremos escribir nuestro programa.

#figure(
  ```cpp
  #include <iostream>
  #include <mpi.h>

  using namespace std;

  int main(int argc, char *argv[])
  {
     //-----------------------
     /* MPI: Initializacion*/
     MPI_Init(&argc,&argv);
     //-----------------------
     /* MPI: Number of processors */
     int numtasks;
     MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
     //-----------------------
     /* MPI: Assign ID to each processor */
     int taskid;
     MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
     //-----------------------
     char nodename[MPI_MAX_PROCESSOR_NAME+1];
     int len;
     MPI_Get_processor_name(nodename,&len);
     //-----------------------
     cout << " Hello world (C++)! desde el proceso " << taskid << " de "
          << numtasks << ". Se ejecuta en " << nodename << "\n";
     //-----------------------
     /* MPI: Completion */
     MPI_Finalize();
  }
  ```,
  caption: [Ejemplo de código de OpenMPI.],
)
== Compilación
Una vez listo, realizaremos la compilación del programa mediante `mpic++`. No
es necesario instalar nada; el equipo ya tiene disponible el comando.
#figure(
  ```sh
    mpic++.openmpi HelloWorldMPI_Ins.cpp -O2 -o HelloWorld
  ```,
  caption: [Ejemplo de comando de compilación de código OpenMPI.],
)
Esto nos genera un archivo binario `HelloWorld`.
== SLURM
Para realizar la ejecución del archivo binario, debemos generar un archivo tipo
Simple Linux Utility for Resource Management (SLURM). Esto es necesario para que
se distribuya nuestra tarea (el archivo binario) se rediriga a los procesadores
indicados.

Para visualizar los procesadores disponibles, podemos listarlos mediante el
siguiente comando:

#figure(
  ```sh
  sinfo
  ```,
  caption: [Comando para listar las particiones del cluster.],
)

#figure(
  ```
    PARTITION    AVAIL  TIMELIMIT  NODES  STATE NODELIST
  C0              up   infinite     28   idle c-0-[1-18],c-2-[1-10]
  GPU             up   infinite      1  drain g-0-11
  GPU             up   infinite      7  alloc g-0-[1-4,9-10,12]
  GPU             up   infinite      4   idle g-0-[5-8]
  ```,
  caption:[Porción de los clusters disponibles.]
)

En el mismo correo donde se comparten los datos de acceso, se nos hace saber a
qué particiones tenemos acceso. En el caso de estudiantes de posgrado, se les
da acceso a `C0`, `C1Mitad1` y `GPU`.

Dentro del mismo directorio donde comenzamos, podemos generar el archivo SLURM
mediante el comando `touch` y editarlo mediante `vim`. Por ejemplo, 

#figure(
  ```sh
  touch slurm_Hello_MPI
  vim slurm_Hello_MPI
  ```,
  caption: [Ejemplo de creación de archivo SLURM.],
)
#figure(
  ```
  #!/bin/bash

  #SBATCH --partition=C1Mitad1
  #SBATCH --job-name=pedro_llerenas
  #SBATCH --ntasks-per-node=12
  #SBATCH --ntasks=20
  #SBATCH --output=result_Hello.log
  #SBATCH --mem=0
  #SBATCH --time=0

  mpirun.openmpi -np ${SLURM_NTASKS} HelloWorld
  ```,
  caption: [Ejemplo mínimo de archivo SLURM.],
)

Notemos la siguiente línea: `#SBATCH --partition=C1Mitad1`. Esto le indica al
front-end que deaseamos utilizar la partición `C1Mitad1` del cluster. La última
línea `mpirun.openmpi -np ${SLURM_NTASKS} HelloWorld` realiza la ejecución del
archivo `HelloWorld` en la partición indicada.

Una vez creado y guardado, podemos enviar la tarea al cluster mediante el
siguiente comando:

#figure(
  ```sh
  sbatch slurm_Hello_MPI
  ```,
  caption: [Comando para el registro de la tarea.],
)
Esto nos debería regresar el siguiente mensaje:
#figure(
  ```
  Submitted batch job <job_id>
  ```,
  caption: [],
)
