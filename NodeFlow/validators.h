/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#ifndef VALIDATORS_H
#define VALIDATORS_H

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif


struct  Validators {

    static wxString *validAlphaNumericCharacters();
    static wxString *validIPCharacters();
    static wxString *validNumberCharacters();
    static wxString *validHexCharacters();
    // Text Validators
    static wxTextValidator &AlphaNumeric();
    static wxTextValidator &IP();
    static wxTextValidator &Numbers();
    static wxTextValidator &Hex();
};
#endif // VALIDATORS_H
