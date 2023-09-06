#include "header.h"
#include "ImagePixelMapping.h"

class MyImagePixelMapping : public ImagePixelMapping
{
protected:

public:
	MyImagePixelMapping(double x, double y, double xRatio = 1, double yRatio = 1)
		: xShift(x), yShift(y), xRatio(xRatio), yRatio(yRatio)
	{
	}

	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= image.getWidth() * xRatio - 0.5)
		{
			if (x >= image.getWidth() - 0.5)
				return false;
			return true;
		}

		if (y >= (image.getHeight() * yRatio - 0.5))
		{
			if (y >= image.getHeight() - 0.5)
				return false;
			return true;
		}

		x += xShift;
		y += yShift;

		// Ensure it is within the size of the image
		while (x > image.getWidth() * xRatio)
			x -= image.getWidth() * xRatio;
		while (x < 0)
			x += image.getWidth() * xRatio;
		while (y > image.getHeight() * yRatio)
			y -= image.getHeight() * yRatio;
		while (y < 0)
			y += image.getHeight() * yRatio;

		return true;
	}

	void setXShift(double x) { xShift = x; }
	void setYShift(double y) { yShift = y; }

protected:
	double xShift;
	double yShift;
	double xRatio;
	double yRatio;
};