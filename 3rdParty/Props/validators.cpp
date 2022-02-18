/*!
 *
 *  Author:  B. J. Hill
 *  (c)  Air Monitors Limited 2017 -
 */

#include "validators.h"

/*!
 * \brief AMPI::Validators::validAlphaNumericCharacters
 * \return
 */
wxString * AMPI::Validators::validAlphaNumericCharacters() // for names
{
    static wxString s("01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_:.");
    return &s;
}

/*!
 * \brief AMPI::Validators::validIPCharacters
 * \return
 */
wxString * AMPI::Validators::validIPCharacters() // IP address charcaters
{
    static wxString s("01234567890.");
    return &s;
}

/*!
 * \brief AMPI::Validators::validNumberCharacters
 * \return
 */
wxString * AMPI::Validators::validNumberCharacters() // characters for numbers
{
    static wxString s("01234567890.-");
    return &s;
}

/*!
 * \brief AMPI::Validators::validHexCharacters
 * \return
 */
wxString * AMPI::Validators::validHexCharacters() // characters for Hex numbers
{
    static wxString s("01234567890ABCDEF");
    return &s;
}

/*!
 * \brief AMPI::Validators::AlphaNumeric
 * \return
 */
wxTextValidator &  AMPI::Validators::AlphaNumeric()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validAlphaNumericCharacters());

    return v;
}

/*!
 * \brief AMPI::Validators::IP
 * \return
 */
wxTextValidator &AMPI::Validators::IP()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validIPCharacters());
    return v;

}

/*!
 * \brief AMPI::Validators::Numbers
 * \return
 */
wxTextValidator &  AMPI::Validators::Numbers()
{
    static wxTextValidator v(wxFILTER_NUMERIC);

    return v;

}

/*!
 * \brief AMPI::Validators::Hex
 * \return
 */
wxTextValidator &AMPI::Validators::Hex()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validHexCharacters());
    return v;
}
