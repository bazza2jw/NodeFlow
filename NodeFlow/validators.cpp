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

#include "validators.h"

/*!
 * \brief Validators::validAlphaNumericCharacters
 * \return
 */
wxString * Validators::validAlphaNumericCharacters() // for names
{
    static wxString s("01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_:.");
    return &s;
}

/*!
 * \brief Validators::validIPCharacters
 * \return
 */
wxString * Validators::validIPCharacters() // IP address charcaters
{
    static wxString s("01234567890.");
    return &s;
}

/*!
 * \brief Validators::validNumberCharacters
 * \return
 */
wxString * Validators::validNumberCharacters() // characters for numbers
{
    static wxString s("01234567890.-");
    return &s;
}

/*!
 * \brief Validators::validHexCharacters
 * \return
 */
wxString * Validators::validHexCharacters() // characters for Hex numbers
{
    static wxString s("01234567890ABCDEF");
    return &s;
}

/*!
 * \brief Validators::AlphaNumeric
 * \return
 */
wxTextValidator &  Validators::AlphaNumeric()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validAlphaNumericCharacters());

    return v;
}

/*!
 * \brief Validators::IP
 * \return
 */
wxTextValidator &Validators::IP()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validIPCharacters());
    return v;

}

/*!
 * \brief Validators::Numbers
 * \return
 */
wxTextValidator &  Validators::Numbers()
{
    static wxTextValidator v(wxFILTER_NUMERIC);

    return v;

}

/*!
 * \brief Validators::Hex
 * \return
 */
wxTextValidator &Validators::Hex()
{
    static wxTextValidator v(wxFILTER_INCLUDE_CHAR_LIST);
    if(v.GetIncludes().Count() < 1) v.SetCharIncludes(*validHexCharacters());
    return v;
}
