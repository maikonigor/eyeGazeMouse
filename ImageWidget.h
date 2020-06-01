#pragma once
#include <QObject>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>

class ImageWidget : public QWidget
{
	Q_OBJECT

public:
	ImageWidget(QWidget *parent = 0, cv::Size size=cv::Size(0, 0));
	~ImageWidget();
	QSize sizeHint();
	QSize minimumSizeHint();
	void paintGL();
	void initializeGL();
	void resizeGL(int,int);

public slots:
	void showImage(cv::Mat&);

private:
	void paintEvent(QPaintEvent* /*event*/);
	QImage _qimage;
	cv::Mat _tmp;
	int width;
	int height;
};


