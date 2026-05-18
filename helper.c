#include "servis.h"

int usporediPoID(
    const void* a,
    const void* b
)
{
    const RadniNalog* prvi =
        (const RadniNalog*)a;

    const RadniNalog* drugi =
        (const RadniNalog*)b;

    return prvi->id - drugi->id;
}

int usporediPoCijeni(
    const void* a,
    const void* b
)
{
    const RadniNalog* prvi =
        (const RadniNalog*)a;

    const RadniNalog* drugi =
        (const RadniNalog*)b;

    if(prvi->ukupanTrosak >
       drugi->ukupanTrosak)
    {
        return 1;
    }

    if(prvi->ukupanTrosak <
       drugi->ukupanTrosak)
    {
        return -1;
    }

    return 0;
}

void rekurzivniIspis(
    Cvor* head
)
{
    if(head == NULL)
    {
        return;
    }

    printf("ID: %d\n",
           head->podatak.id);

    rekurzivniIspis(head->next);
}
