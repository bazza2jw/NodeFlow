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
