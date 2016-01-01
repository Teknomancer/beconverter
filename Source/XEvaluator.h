//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  This file is part of BeConverter.
//
//  BeConverter is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  BeConverter is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with BeConverter.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _XEVAL_H
#define _XEVAL_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <String.h>

//#include "StrXtra.h"

// error codes for XEvaluator
#define CALC_OK 0
#define CALC_INVALID_OPERATOR 1
#define CALC_UNMATCHED_PARENS 2
#define CALC_SOMETHING_HORRIBLY_WRONG 3
#define CALC_NO_LAST_ANSWER 4
#define CALC_INVALID_EXPRESSION 5
#define CALC_UNKNOWN_RADIX 6
#define CALC_NO_EXPRESSION 7

#define CALC_TRIG_EXP 6
#define CALC_NORMAL_EXP 7
#define CALC_SOLVED_EXP 8

// Class prototype definitions
class XEvaluator
{
    public:
        XEvaluator();
        ~XEvaluator();

        int Calculate (BString *_expression, BString *_response, int &selstart, int &selstop);
        int Calculate (BString *_expression, double *_result);

        BString GetLastAnswer ();
        void SetLastAnswer (BString answer);

        void UseRadians();
        void UseDegrees();

        void SetResponseBase (int base);
        int ResponseBase ();

        void SetPrecision (int prec);
        int Precision ();

        void SetSciNotation (bool notation);
        bool SciNotation ();

        BString ErrorString ();

    private:
        BString *_operators;
        BString *_traditionaloperators;

        BString *_expression;
        BString *_lastanswer;

        BString MsgError;

        int ErrorCode;
        int _ResponseBase;
        bool _UseRadians;

        bool IsOperator (char opr);
        bool IsNegativeModifier (BString *_expression, int position);
        void CleanUpSyntax();
        void Paranthesize ();
        void FindFirstSolvable (int &start, int &stop);
        void Solve (int start, int stop);
        int ExpressionType (BString *_expression);

        int _Precision;
        bool _SciNotation;
};

#endif /* _XEVAL_H */
