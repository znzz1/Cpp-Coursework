#ifndef MYFILTERPOINT_H
#define MYFILTERPOINT_H

#include "header.h"
#include "FilterPoints.h"
#include "DrawingSurface.h"

class MyFilterPoint : public FilterPoints
{
private:
	FilterPoints* pNextFilter;
	int xStretch, yStretch, xCompress, yCompress;
	int xModifier, yModifier;

public:
	MyFilterPoint(int xModifier, int yModifier, int xFactor, int yFactor, FilterPoints* pNextFilter = nullptr)
		: xModifier(xModifier), yModifier(yModifier), xStretch(1), yStretch(1), xCompress(1), yCompress(1), pNextFilter(pNextFilter)
	{
		setStretch(xFactor, yFactor);
	}

	~MyFilterPoint() {}

	virtual bool filter(DrawingSurface* surface, int& xVirtual, int& yVirtual, unsigned int& uiColour)
	{
		xVirtual += xModifier;
		yVirtual += yModifier;
		handleXthenY(surface, xVirtual, yVirtual, uiColour);
		return false;
	}

	virtual int filterConvertRealToVirtualXPosition(int xReal)
	{
		if (pNextFilter)
			xReal = pNextFilter->filterConvertRealToVirtualXPosition(xReal);

		if (xStretch > 1)
			return xReal / xStretch - xModifier;
		else
			return xReal * xCompress - xModifier;
	}

	virtual int filterConvertRealToVirtualYPosition(int yReal)
	{
		if (pNextFilter)
			yReal = pNextFilter->filterConvertRealToVirtualYPosition(yReal);

		if (yStretch > 1)
			return yReal / yStretch - yModifier;
		else
			return yReal * yCompress - yModifier;
	}

	virtual int filterConvertVirtualToRealXPosition(int xVirtual)
	{
		if (xStretch > 1)
		{
			if (pNextFilter)
				return pNextFilter->filterConvertVirtualToRealXPosition((xVirtual + xModifier) * xStretch);
			else
				return (xVirtual + xModifier) * xStretch;
		}
		else
		{
			if (pNextFilter)
				return pNextFilter->filterConvertVirtualToRealXPosition((xVirtual + xModifier) / xCompress);
			else
				return (xVirtual + xModifier) / xCompress;
		}
	}

	virtual int filterConvertVirtualToRealYPosition(int yVirtual)
	{
		if (yStretch > 1)
		{
			if (pNextFilter)
				return pNextFilter->filterConvertVirtualToRealYPosition((yVirtual + yModifier) * yStretch);
			else
				return (yVirtual + yModifier) * yStretch;
		}
		else
		{
			if (pNextFilter)
				return pNextFilter->filterConvertVirtualToRealYPosition((yVirtual + yModifier) / xCompress);
			else
				return (yVirtual + yModifier) / yCompress;
		}
	}

	void stretchX() { if (xCompress > 1) --xCompress; else ++xStretch; }
	void stretchY() { if (yCompress > 1) --yCompress; else ++yStretch; }
	void compressX() { if (xStretch > 1) --xStretch; else ++xCompress; }
	void compressY() { if (yStretch > 1) --yStretch; else ++yCompress; }

	void compress() { compressX(); compressY(); }
	void stretch() { stretchX(); stretchY(); }

	void setStretch(int xFactor, int yFactor)
	{
		if (xFactor == 0) { xStretch = 1; xCompress = 1; }
		else if (xFactor < 0) { xStretch = 1; xCompress = 1 - xFactor; }
		else if (xFactor > 0) { xStretch = 1 + xFactor; xCompress = 1; }
		if (yFactor == 0) { yStretch = 1; yCompress = 1; }
		else if (yFactor < 0) { yStretch = 1; yCompress = 1 - yFactor; }
		else if (yFactor > 0) { yStretch = 1 + yFactor; yCompress = 1; }
	}

	float getZoomX()
	{
		int v1 = xStretch; if (v1 < 1) v1 = 1;
		int v2 = xCompress; if (v2 < 1) v2 = 1;
		return (float)v1 / (float)v2;
	}

	float getZoomY()
	{
		int v1 = yStretch; if (v1 < 1) v1 = 1;
		int v2 = yCompress; if (v2 < 1) v2 = 1;
		return (float)v1 / (float)v2;
	}

	void setOffset(int offsetX, int offsetY)
	{
		xModifier = offsetX;
		yModifier = offsetY;
	}

	int getXOffset() { return xModifier; }
	int getYOffset() { return yModifier; }

	void changeOffset(int offsetXIncrement, int offsetYIncrement)
	{
		xModifier += offsetXIncrement;
		yModifier += offsetYIncrement;
	}

private:
	void handleXthenY(DrawingSurface* surface, int xVirtual, int yVirtual, unsigned int& uiColour)
	{
		if (xStretch > 1)
		{
			xVirtual *= xStretch;
			for (int i = 0; i < xStretch; i++)
				handleY(surface, xVirtual + i, yVirtual, uiColour);
		}
		else
		{
			xVirtual /= xCompress;
			handleY(surface, xVirtual, yVirtual, uiColour);
		}
	}

	void handleY(DrawingSurface* surface, int xVirtual, int yVirtual, unsigned int& uiColour)
	{
		if (yStretch > 1)
		{
			yVirtual *= yStretch;
			for (int i = 0; i < yStretch; i++)
			{
				int yTest = yVirtual + i;
				if ((pNextFilter == nullptr) || pNextFilter->filter(surface, xVirtual, yTest, uiColour))
					surface->rawSetPixel(xVirtual, yTest, uiColour);
			}
		}
		else
		{
			yVirtual /= yCompress;
			if ((pNextFilter == nullptr) || pNextFilter->filter(surface, xVirtual, yVirtual, uiColour))
				surface->rawSetPixel(xVirtual, yVirtual, uiColour);
		}
	}
};

#endif