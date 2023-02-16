/*
 * This class used only for G8 enum from "GlobalStatusErrorCodes.h"
*/
#include "g8enum.h"

QMetaEnum G8Enum::G8MetaEnum = Enum<G8Enum>();
G8Enum::G8Enum(QObject *parent) : QObject(parent) { }
G8Enum::~G8Enum() { }

