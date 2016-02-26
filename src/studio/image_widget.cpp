#include "image_widget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QLabel(parent)
{
    this->setMinimumSize(1,1);
}

void ImageWidget::setImg(Image& in) {
    if (in.data != nullptr && in.bytesPerPixel == 3) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_img.updateSize(in.rows, in.cols, in.bytesPerPixel);
        m_img.swapData(&in.data);
        //QImage qimg = QImage(img.data, img.cols, img.rows, img.cols * 3, QImage::Format_RGB888);

        QImage qimg = QImage(m_img.data, m_img.cols, m_img.rows, QImage::Format_RGB888);
        m_pix = QPixmap::fromImage(qimg);
        QLabel::setPixmap(m_pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void ImageWidget::resizeEvent(QResizeEvent * e)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    QLabel::setPixmap(m_pix.scaled(this->size(),
    Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
