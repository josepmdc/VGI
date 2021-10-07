# VGI

## Instruccions

### Clonar el repositori

**IMPORTANT**: Clonar de forma recursiva per descarregar les llibreries

`git clone https://github.com/josepmdc/VGI.git --recursive`

Si ja l'havies clonat i però no ho has fet de forma recursiva, executa la següent comanda (o esborra i torna a clonar amb recursive):

`git submodule update --init`

### Windows + Visual Studio
Primer de tot has d'instalar CMake. Si fas servir Visual Studio en principi ja ve instalat, sinó el tens el pots baixar [aquí](https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-windows-x86_64.msi).

Un cop tinguis CMake has d'executar l'script `scripts\generate.bat` ***desde el directori arrel del projecte***. 

Això et generara una solució de Visual Studio que podrás obrir i executar.

### Linux i macOS
Primer de tot has d'instalar CMake. Pot ser que ja vingui instalat depen de la distro. Sinó el tens el pots baixar de la [web](https://cmake.org/download/) o fer servir el package manager de la distro.
Un cop tinguis CMake has d'executar ***desde el directori arrel del projecte***:

`./scripts/run.sh`

Això compilara el projecte i l'executara.

Per treballar pots obrir la carpeta i editar el codi desde qualsevol editor de text.

## Estructura del projecte
```
assets/             conte arxius com ara shaders, imatges, textures...
└── shaders/        conte tots els shaders
src/                conte tot el codi font
scripts/            conte scripts per generar i executar el projecte
lib/                conte les llibreries externes
```
