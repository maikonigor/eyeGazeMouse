#include "ImageWidget.h"

ImageWidget::ImageWidget(QWidget *parent, cv::Size size) : QWidget(parent) {
	if(width > 0) {
		this->setFixedSize(size.width, size.height);
	}
	
	this->width = size.width;
	this->height = size.height;
	_tmp.create(size, CV_8UC3);
}
ImageWidget::~ImageWidget() {}

QSize ImageWidget::sizeHint() { 
	return _qimage.size(); 
}

QSize ImageWidget::minimumSizeHint() { 
	return _qimage.size(); 
}

void ImageWidget::showImage(cv::Mat& image) {
	if(image.size().width == _tmp.size().width && image.size().height == _tmp.size().height) {
		cv::cvtColor(image, _tmp, CV_RGB2BGR);
	}
	else {
		cv::resize(image, _tmp, _tmp.size());
		cv::cvtColor(_tmp, _tmp, CV_RGB2BGR);		
	}

	// QImage needs the data to be stored continuously in memory
	assert(_tmp.isContinuous());

	// Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
	// (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
	// has three bytes.
	_qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);

	repaint();
}

void ImageWidget::paintEvent(QPaintEvent* /*event*/) {
	// Display the image
	QPainter painter(this);
	painter.drawImage(QPoint(0,0), _qimage);
	painter.end();
}
