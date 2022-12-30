// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2001 Ramshankar (aka Teknomancer).
// All rights reserved.

#include "XEvaluator.h"

XEvaluator::XEvaluator()
{
}
	
XEvaluator::~XEvaluator()
{
}

int XEvaluator::Calculate (BString *_expression, BString *_response, int &selstart, int &selstop)
{
	return 0;
}

int XEvaluator::Calculate (BString *_expression, double *_result)
{
	return 0;
}

BString XEvaluator::GetLastAnswer ()
{
	return "";
}

void XEvaluator::SetLastAnswer (BString answer)
{
}

void XEvaluator::UseRadians()
{
}

void XEvaluator::UseDegrees()
{
}

void XEvaluator::SetResponseBase (int base)
{
}

int XEvaluator::ResponseBase ()
{
	return 0;
}

void XEvaluator::SetPrecision (int prec)
{
}

int XEvaluator::Precision ()
{
	return 0;
}

void XEvaluator::SetSciNotation (bool notation)
{
}

bool XEvaluator::SciNotation ()
{
	return false;
}

BString XEvaluator::ErrorString ()
{
	return "";
}

