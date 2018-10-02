#ifndef __MERGE_H
#define __MERGE_H

#include "struct.h"
#include "autolists.h"

// pas fait
// int countIntersections(AN an, BasinList *bl);

//renvoie NULL si b1 et b2 sont disjoints
//renvoie l'intersection sinon
Basin *intersect(AN an, Basin *b1, Basin *b2);

//Idée : cette fonction doit fusionner les bassins trouvés afin d'obtenir des bassins minimaux.
//Au 17/09/2018, elle n'est pas terminée.
BasinList *merge(an, BasinList *bl);

#endif
