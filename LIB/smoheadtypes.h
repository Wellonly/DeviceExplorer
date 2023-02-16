#ifndef SMOHEADTYPES_H
#define SMOHEADTYPES_H

#ifndef __arm__
enum __attribute__ ((packed))
#else
enum  /* arm only!!! */
#endif
SMOHeadTypes /*: byte*/
{ //old: SmoHeadTypes
    SmoHeadType_Data /* const*/ = 0, /*old: PropertyInData...PropertyInRAM*/
    SmoHeadType_DataPointer     = 1, /*old: PropertyInCode...PropertyInFLASH*/
    SmoHeadType_Folder          = 2, /*old: FoldersOrNetsOrDevice*/
    SmoHeadType_Property        = 3  /*Method = 3,*/
};

#endif // SMOHEADTYPES_H
