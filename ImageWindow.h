#pragma once
#include "ImageWidget.h"
#include <QMainWindow>

class ImageWindow : public QMainWindow
{
public:
	ImageWindow(int screenIndex = 1, bool debugSize = false);
	~ImageWindow();
	void showImage(cv::Mat image);
	void show();

private:
	ImageWidget* imageWidget;
};
