#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

#include <mutex>

#include "input_data.h"

class ImageWidget : public QLabel
{
Q_OBJECT
public:

    explicit ImageWidget(QWidget *parent = 0);

signals:

public slots:
    void setImg(Image& m_img);
    void resizeEvent(QResizeEvent *);

private:
    Image m_img;
    QPixmap m_pix;
    std::mutex m_mutex;
};

#endif // IMAGEWIDGET_H
