#ifndef __GETDATA_H
#define __GETDATA_H

#include "struct.h"
#include "init.h"

// renvoie le réseau d'automates codé dans le fichier filepath
// le fichier doit être au format .an et avoir un état initial
AN getData(char *filepath);

#endif
