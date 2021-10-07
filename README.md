# VGI

## Instruccions

### Clonar el repositori

**IMPORTANT**: Clonar de forma recursiva per descarregar les llibreries

`git clone https://github.com/josepmdc/VGI.git --recursive`

Si ja l'havies clonat i però no ho has fet de forma recursiva, executa la següent comanda (o esborra i torna a clonar amb recursive):

`git submodule update --init`

### Windows + Visual Studio
Si fas servir Windows i Visual Studio, el primer que has de fer després de clonar és executar l'script `scripts\generate.bat`. 

Això et generara una solució de Visual Studio que podrás obrir i executar.

### Linux i macOS
Desde el directori arrel del projecte executar:

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