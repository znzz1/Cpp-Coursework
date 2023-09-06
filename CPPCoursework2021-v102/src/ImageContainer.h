#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include <vector>
#include "ImageManager.h"

class ImageContainer
{
private:
	std::vector<SimpleImage> m_vecImages;
public:
	ImageContainer()
		: m_vecImages(0)
	{}

	void setSize(int size)
	{
		m_vecImages.resize(size);
	}

	void storeImage(int index, SimpleImage image)
	{
		m_vecImages[index] = image;
	}

	SimpleImage getImage(int index) const
	{
		return m_vecImages[index];
	}
};

#endif