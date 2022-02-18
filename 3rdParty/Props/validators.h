#ifndef VALIDATORS_H
#define VALIDATORS_H
/*!
 *
 *  Author:  B. J. Hill
 *  (c)  Air Monitors Limited 2017 -
 */

#include "ampicommon.h"
//
// Some validator helpers
//

namespace AMPI {
/*!
     * \brief The Validators struct
     * Set of validators for user interfaces
     */
    struct AMPICOMMONSHARED_EXPORT Validators {

        // Strings for validators
        static wxString *validAlphaNumericCharacters();  //!< for names
        static wxString *validIPCharacters();  //!< IP address characters
        static wxString *validNumberCharacters();  //!< characters for numbers
        static wxString *validHexCharacters();  //!< characters for numbers
        //
        // Text Validators
        static wxTextValidator &AlphaNumeric();
        static wxTextValidator &IP();
        static wxTextValidator &Numbers();
        static wxTextValidator &Hex();

        //
    };
}

#endif // VALIDATORS_H
