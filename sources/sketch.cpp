#include "sketch.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTextStream>

Sketch::Sketch(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
}

void Sketch::openImage(const QString &fileName)
{
    QTextStream out{stdout};

    QImage loadedImage;
    if (!loadedImage.load(fileName)) {
        out << "Image file not found" << endl;
        image.fill(qRgb(255, 255, 255));
        return;
    }

    image = loadedImage;
    update();
}

void Sketch::saveImage(const QString &fileName)
{
    QTextStream out{stdout};

    image.save(fileName);
    out << "Image saved" << endl;
}

void Sketch::setPenColor(const QColor &newColor)
{
    penColor = newColor;
}

void Sketch::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    update();
}

void Sketch::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void Sketch::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void Sketch::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void Sketch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void Sketch::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width(), image.width());
        int newHeight = qMax(height(), image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void Sketch::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.drawLine(lastPoint, endPoint);
    
    int rad = (penWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void Sketch::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
