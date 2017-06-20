#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

class Constraints{
	public:
		Constraints(double minW, double maxW, double minL, double maxL, double minH, double maxH, double rH, double rO, double pminW, double pmaxW, double pminL, double pmaxL);
		Constraints(double minH, double maxH, double minS, double maxS, int steps);
		double getMinWidth();
		double getMaxWidth();
		double getMinLength();
		double getMaxLength();
		double getMinHeight();
		double getMaxHeight();
		double getRHeight();
		double getROverh();
		double getPMinWidth();
		double getPMaxWidth();
		double getPMinLength();
		double getPMaxLength();

		double bgetMinHeight();
		double bgetMaxHeight();
		double bgetMinSide();
		double bgetMaxSide();
		int bgetSteps();

	private:
		double minWidth, maxWidth;
		double minLength, maxLength;
		double minHeight, maxHeight;
		double rHeight, rOverh;
		double pminWidth, pmaxWidth;
		double pminLength, pmaxLength;

		double bminHeight, bmaxHeight;
		double bminSide, bmaxSide;
		int bSteps;
};

#endif
