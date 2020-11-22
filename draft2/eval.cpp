/*
 * This file contains the evaluator which currently only accepts the 
 * following scheme forms and functions:
 *
 * if
 * / * - +
 * < <= > >=
 *
*/

#include "types.h"

Object eval(Object expr)
{
	switch (expr.type) {
		case "Integer":
			return	

