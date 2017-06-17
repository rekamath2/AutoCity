#include "Constraints.h"

Constraints::Constraints(double minW, double maxW, double minL, double maxL, double minH, double maxH, double rH, double rO, double pminW, double pmaxW, double pminL, double pmaxL) : 
	minWidth(minW),
	maxWidth(maxW),
	minLength(minL),
	maxLength(maxL),
	minHeight(minH),
	maxHeight(maxH),
	rHeight(rH),
	rOverh(rO),
	pminWidth(pminW),
	pmaxWidth(pmaxW),
	pminLength(pminL),
	pmaxLength(pmaxL)
{
}

double Constraints::getMinWidth()
{
	return minWidth;
}

double Constraints::getMaxWidth()
{
	return maxWidth;
}

double Constraints::getMinLength()
{
	return minLength;
}

double Constraints::getMaxLength()
{
	return maxLength;
}

double Constraints::getMinHeight()
{
	return minHeight;
}

double Constraints::getMaxHeight()
{
	return maxHeight;
}

double Constraints::getRHeight()
{
	return rHeight;
}

double Constraints::getROverh()
{
	return rOverh;
}

double Constraints::getPMaxWidth()
{
	return pmaxWidth;
}

double Constraints::getPMinWidth()
{
	return pminWidth;
}

double Constraints::getPMaxLength()
{
	return pmaxLength;
}

double Constraints::getPMinLength()
{
	return pminLength;
}
